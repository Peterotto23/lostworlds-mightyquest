// MQLauncher.cs - one-click launcher for the Lost Worlds Mighty Quest private server.
// Replaces the manual host-setup.ps1 / mqelserver.exe / play.ps1 steps.
//
// Compiles with the C# compiler that ships with Windows (.NET Framework 4.x, C# 5):
//   %windir%\Microsoft.NET\Framework\v4.0.30319\csc.exe
// so neither Visual Studio nor VS Code is needed. See package.ps1.
//
// Runs as a NORMAL user (the game's Steam DRM does not like an elevated game with a
// non-elevated Steam). Only the hosts-file edit is elevated, by re-launching this same
// exe with "--hosts add <ip>" / "--hosts remove" (one Windows UAC prompt, first run only).
//
// ONE-FILE DISTRIBUTION: package.ps1 embeds payload.zip (server, injector, DLLs and the static
// game data) inside this exe as a resource. On first run, and again whenever the payload
// changes, it is unpacked into %LOCALAPPDATA%\LostWorldsMQ. Players only handle this one file.

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32;

namespace LostWorldsLauncher
{
    static class Program
    {
        [STAThread]
        static int Main(string[] args)
        {
            // Elevated helper mode: edit the hosts file, then exit.
            if (args.Length >= 2 && args[0] == "--hosts")
            {
                try
                {
                    if (args[1] == "add" && args.Length >= 3) Hosts.Apply(args[2]);
                    else if (args[1] == "remove") Hosts.Remove();
                    else return 2;
                    return 0;
                }
                catch { return 1; }
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
            return 0;
        }
    }

    // ------------------------------------------------------------------ embedded files
    static class Payload
    {
        const string ResourceName = "payload.zip";

        // Saves, logs and the unpacked files all live here (never in a read-only folder).
        public static readonly string DataDir = Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "LostWorldsMQ");
        public static readonly string AppDir = Path.Combine(DataDir, "app");

        // Short fingerprint of the embedded files (null when running without a payload).
        public static string Version;

        static Stream Open()
        {
            return System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(ResourceName);
        }

        static bool Embedded
        {
            get { using (Stream s = Open()) { return s != null; } }
        }

        // Folder holding inject.exe, certpatch.dll, mqelserver.exe and the OpenSSL DLLs.
        public static string BinDir
        {
            get
            {
                string loose = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "bin");
                // Dev builds without an embedded payload can still use a bin\ folder next to the exe.
                if (!Embedded && File.Exists(Path.Combine(loose, "inject.exe"))) return loose;
                return Path.Combine(AppDir, "bin");
            }
        }

        static string Normalize(string zipName)
        {
            return zipName.Replace('/', Path.DirectorySeparatorChar).Replace('\\', Path.DirectorySeparatorChar);
        }

        static string ComputeVersion()
        {
            using (Stream s = Open())
            using (var sha = System.Security.Cryptography.SHA256.Create())
            {
                byte[] h = sha.ComputeHash(s);
                var sb = new StringBuilder();
                for (int i = 0; i < 6; i++) sb.Append(h[i].ToString("x2"));
                return sb.ToString();
            }
        }

        // True if every Plugins\<folder> that the payload ships still exists on disk.
        static bool DataFoldersPresent()
        {
            using (Stream s = Open())
            using (var zip = new System.IO.Compression.ZipArchive(s, System.IO.Compression.ZipArchiveMode.Read))
            {
                foreach (var entry in zip.Entries)
                {
                    string[] parts = Normalize(entry.FullName).Split(Path.DirectorySeparatorChar);
                    if (parts.Length >= 3 && parts[0] == "Plugins")
                        if (!Directory.Exists(Path.Combine(DataDir, parts[0], parts[1]))) return false;
                }
            }
            return true;
        }

        // A leftover server from an earlier run would keep mqelserver.exe locked while we replace it.
        static void StopOurServer()
        {
            foreach (Process p in Process.GetProcessesByName("mqelserver"))
            {
                try
                {
                    string exe = p.MainModule.FileName;
                    if (exe.StartsWith(AppDir, StringComparison.OrdinalIgnoreCase))
                    {
                        p.Kill();
                        p.WaitForExit(3000);
                    }
                }
                catch { }
            }
        }

        // Unpacks the embedded files if they are missing or out of date.
        // Returns null on success, or a message that explains what went wrong.
        public static string Ensure(Action<string> say)
        {
            if (!Embedded) return null; // running next to a loose bin\ folder
            try
            {
                Version = ComputeVersion();
                string stamp = Path.Combine(AppDir, "payload.version");
                string bin = Path.Combine(AppDir, "bin");
                bool upToDate =
                    File.Exists(stamp) && File.ReadAllText(stamp).Trim() == Version &&
                    File.Exists(Path.Combine(bin, "inject.exe")) &&
                    File.Exists(Path.Combine(bin, "certpatch.dll")) &&
                    File.Exists(Path.Combine(bin, "mqelserver.exe")) &&
                    DataFoldersPresent();
                if (upToDate) return null;

                say("Setting up game files (first run, or after an update)...");
                Directory.CreateDirectory(DataDir);
                StopOurServer();

                string root = Path.GetFullPath(DataDir) + Path.DirectorySeparatorChar;
                using (Stream s = Open())
                using (var zip = new System.IO.Compression.ZipArchive(s, System.IO.Compression.ZipArchiveMode.Read))
                {
                    foreach (var entry in zip.Entries)
                    {
                        string rel = Normalize(entry.FullName);
                        if (rel.Length == 0 || rel.EndsWith(Path.DirectorySeparatorChar.ToString())) continue; // folder entry
                        if (rel.IndexOf("Localnet", StringComparison.OrdinalIgnoreCase) >= 0) continue;      // never touch saves
                        string dest = Path.GetFullPath(Path.Combine(DataDir, rel));
                        if (!dest.StartsWith(root, StringComparison.OrdinalIgnoreCase)) continue;             // path safety
                        Directory.CreateDirectory(Path.GetDirectoryName(dest));
                        using (Stream src = entry.Open())
                        using (var dst = new FileStream(dest, FileMode.Create, FileAccess.Write, FileShare.None))
                            src.CopyTo(dst);
                        try { File.SetLastWriteTime(dest, entry.LastWriteTime.DateTime); } catch { }
                    }
                }
                File.WriteAllText(stamp, Version);
                return null;
            }
            catch (Exception ex)
            {
                return "Couldn't unpack the game files: " + ex.Message +
                       "  (If an old Mighty Quest server is still running, close it in Task Manager and press PLAY again.)";
            }
        }
    }

    // ------------------------------------------------------------------ hosts file
    static class Hosts
    {
        public static readonly string[] Names = { "Gameserver", "gs.themightyquest.com" };
        const string Marker = "# Mighty Quest local server (Lost Worlds) - remove to revert";

        public static string Path
        {
            get { return System.IO.Path.Combine(Environment.SystemDirectory, @"drivers\etc\hosts"); }
        }

        // True if every game hostname already points at 'ip' and nothing else overrides it.
        public static bool Matches(string ip)
        {
            try
            {
                int found = 0;
                foreach (string raw in File.ReadAllLines(Path))
                {
                    string line = raw.Trim();
                    if (line.Length == 0 || line.StartsWith("#")) continue;
                    string[] parts = Regex.Split(line, @"\s+");
                    if (parts.Length < 2) continue;
                    for (int i = 1; i < parts.Length; i++)
                    {
                        if (parts[i].StartsWith("#")) break;
                        foreach (string n in Names)
                        {
                            if (string.Equals(parts[i], n, StringComparison.OrdinalIgnoreCase))
                            {
                                if (parts[0] != ip) return false;
                                found++;
                            }
                        }
                    }
                }
                return found >= Names.Length;
            }
            catch { return false; }
        }

        static string[] Filtered()
        {
            var kept = new System.Collections.Generic.List<string>();
            foreach (string line in File.ReadAllLines(Path))
            {
                if (line.Contains("Mighty Quest local server")) continue;
                bool isOurs = false;
                string t = line.Trim();
                if (t.Length > 0 && !t.StartsWith("#"))
                {
                    string[] parts = Regex.Split(t, @"\s+");
                    for (int i = 1; i < parts.Length; i++)
                    {
                        if (parts[i].StartsWith("#")) break;
                        foreach (string n in Names)
                            if (string.Equals(parts[i], n, StringComparison.OrdinalIgnoreCase)) isOurs = true;
                    }
                }
                if (!isOurs) kept.Add(line);
            }
            return kept.ToArray();
        }

        static void WriteWithRetry(string[] lines)
        {
            for (int attempt = 0; ; attempt++)
            {
                try { File.WriteAllLines(Path, lines, Encoding.ASCII); return; }
                catch (IOException)
                {
                    if (attempt >= 5) throw;
                    Thread.Sleep(500); // antivirus sometimes holds the file briefly
                }
            }
        }

        // Runs elevated.
        public static void Apply(string ip)
        {
            string backup = Path + ".lostworlds.bak";
            if (!File.Exists(backup)) File.Copy(Path, backup, false);
            var list = new System.Collections.Generic.List<string>(Filtered());
            list.Add(Marker);
            foreach (string n in Names) list.Add(ip + "\t" + n);
            WriteWithRetry(list.ToArray());
        }

        // Runs elevated.
        public static void Remove()
        {
            WriteWithRetry(Filtered());
        }

        // Re-launch this exe elevated to do the edit. Returns true on success.
        public static bool RunElevated(string arguments)
        {
            try
            {
                var psi = new ProcessStartInfo(Application.ExecutablePath, arguments);
                psi.Verb = "runas";
                psi.UseShellExecute = true;
                psi.WindowStyle = ProcessWindowStyle.Hidden;
                using (Process p = Process.Start(psi))
                {
                    p.WaitForExit();
                    return p.ExitCode == 0;
                }
            }
            catch (Win32Exception) { return false; } // user clicked "No" on the UAC prompt
        }
    }

    // ------------------------------------------------------------------ main window
    class MainForm : Form
    {
        const string AppId = "239220";
        const string GameArgs =
            "-server_url https://Gameserver -environmentName mqel-live -branchName mqel " +
            "-steamid 76561201696194782 -steamticket \"\" -token \"\"";

        // Server, injector and DLLs are unpacked from inside this exe (see Payload).
        static string BinDir { get { return Payload.BinDir; } }
        // Saves + logs live here (the server writes ./Plugins relative to its working dir,
        // so it must not live inside a read-only folder like Program Files).
        static readonly string DataDir = Payload.DataDir;
        static readonly string SettingsFile = Path.Combine(DataDir, "settings.txt");
        static readonly string LogFile = Path.Combine(DataDir, "launcher.log");

        RadioButton rbSolo, rbJoin;
        TextBox tbIp, tbLog;
        Button btnPlay, btnHosts, btnSaves;
        Label lblStatus;
        Process serverProc;
        bool weStartedServer;
        Thread worker;

        public MainForm()
        {
            Text = "Lost Worlds - The Mighty Quest";
            ClientSize = new Size(520, 420);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            MaximizeBox = false;
            StartPosition = FormStartPosition.CenterScreen;
            Font = new Font("Segoe UI", 9f);

            var title = new Label();
            title.Text = "The Mighty Quest for Epic Loot - private server";
            title.Font = new Font("Segoe UI", 12f, FontStyle.Bold);
            title.SetBounds(16, 14, 490, 28);
            Controls.Add(title);

            rbSolo = new RadioButton();
            rbSolo.Text = "Play on this PC (starts the server for me)";
            rbSolo.Checked = true;
            rbSolo.SetBounds(20, 56, 470, 22);
            Controls.Add(rbSolo);

            rbJoin = new RadioButton();
            rbJoin.Text = "Join a friend's server - their IP address:";
            rbJoin.SetBounds(20, 82, 290, 22);
            Controls.Add(rbJoin);

            tbIp = new TextBox();
            tbIp.SetBounds(316, 80, 180, 24);
            tbIp.Enabled = false;
            Controls.Add(tbIp);
            rbJoin.CheckedChanged += delegate { tbIp.Enabled = rbJoin.Checked; };

            btnPlay = new Button();
            btnPlay.Text = "PLAY";
            btnPlay.Font = new Font("Segoe UI", 11f, FontStyle.Bold);
            btnPlay.SetBounds(20, 116, 476, 44);
            btnPlay.Click += OnPlay;
            Controls.Add(btnPlay);

            lblStatus = new Label();
            lblStatus.Text = "Ready. Steam must be installed (the game is free: app 239220).";
            lblStatus.SetBounds(20, 170, 476, 20);
            Controls.Add(lblStatus);

            tbLog = new TextBox();
            tbLog.Multiline = true;
            tbLog.ReadOnly = true;
            tbLog.ScrollBars = ScrollBars.Vertical;
            tbLog.SetBounds(20, 196, 476, 160);
            Controls.Add(tbLog);

            btnHosts = new Button();
            btnHosts.Text = "Undo hosts change";
            btnHosts.SetBounds(20, 368, 150, 30);
            btnHosts.Click += OnRemoveHosts;
            Controls.Add(btnHosts);

            btnSaves = new Button();
            btnSaves.Text = "Open saves folder";
            btnSaves.SetBounds(180, 368, 150, 30);
            btnSaves.Click += delegate
            {
                Directory.CreateDirectory(DataDir);
                Process.Start("explorer.exe", "\"" + DataDir + "\"");
            };
            Controls.Add(btnSaves);

            LoadSettings();
            FormClosing += OnClosing;
        }

        // ---------------------------------------------------------- UI helpers
        void Say(string msg)
        {
            if (InvokeRequired) { BeginInvoke(new Action<string>(Say), msg); return; }
            lblStatus.Text = msg;
            Log(msg);
        }

        void Log(string msg)
        {
            string line = "[" + DateTime.Now.ToString("HH:mm:ss") + "] " + msg;
            if (InvokeRequired) { BeginInvoke(new Action<string>(Log), msg); return; }
            tbLog.AppendText(line + "\r\n");
            try { Directory.CreateDirectory(DataDir); File.AppendAllText(LogFile, line + "\r\n"); }
            catch { }
        }

        void SetBusy(bool busy)
        {
            if (InvokeRequired) { BeginInvoke(new Action<bool>(SetBusy), busy); return; }
            btnPlay.Enabled = !busy;
            rbSolo.Enabled = !busy;
            rbJoin.Enabled = !busy;
            tbIp.Enabled = !busy && rbJoin.Checked;
            btnHosts.Enabled = !busy;
        }

        void Fail(string msg)
        {
            Say("Problem: " + msg);
            if (InvokeRequired) BeginInvoke(new Action(() => MessageBox.Show(this, msg, "Lost Worlds", MessageBoxButtons.OK, MessageBoxIcon.Warning)));
        }

        void LoadSettings()
        {
            try
            {
                if (!File.Exists(SettingsFile)) return;
                foreach (string l in File.ReadAllLines(SettingsFile))
                {
                    if (l.StartsWith("mode=join")) rbJoin.Checked = true;
                    if (l.StartsWith("ip=")) tbIp.Text = l.Substring(3);
                }
            }
            catch { }
        }

        void SaveSettings()
        {
            try
            {
                Directory.CreateDirectory(DataDir);
                File.WriteAllLines(SettingsFile, new[] { rbJoin.Checked ? "mode=join" : "mode=solo", "ip=" + tbIp.Text.Trim() });
            }
            catch { }
        }

        // ---------------------------------------------------------- buttons
        void OnRemoveHosts(object s, EventArgs e)
        {
            if (Hosts.RunElevated("--hosts remove")) Say("Hosts entries removed.");
            else Say("Hosts change was cancelled or failed.");
        }

        void OnPlay(object s, EventArgs e)
        {
            bool solo = rbSolo.Checked;
            string ip = tbIp.Text.Trim();
            IPAddress parsed;
            if (!solo && !IPAddress.TryParse(ip, out parsed))
            {
                MessageBox.Show(this, "Enter the server's IP address (for example 192.168.1.50).", "Lost Worlds");
                return;
            }
            SaveSettings();
            SetBusy(true);
            worker = new Thread(() => Run(solo, ip));
            worker.IsBackground = true;
            worker.Start();
        }

        void OnClosing(object s, FormClosingEventArgs e)
        {
            StopServer();
        }

        // ---------------------------------------------------------- the actual work
        void Run(bool solo, string ip)
        {
            try
            {
                Directory.CreateDirectory(DataDir);

                // 0. unpack the files built into this exe (first run / after an update)
                string problem = Payload.Ensure(Say);
                if (problem != null) { Fail(problem); return; }
                string srvExe = Path.Combine(BinDir, "mqelserver.exe");
                Log("Files: " + (Payload.Version ?? "loose bin folder") + ", server built " +
                    (File.Exists(srvExe) ? File.GetLastWriteTime(srvExe).ToString("yyyy-MM-dd HH:mm") : "(missing)"));

                foreach (string f in new[] { "inject.exe", "certpatch.dll" })
                    if (!File.Exists(Path.Combine(BinDir, f)))
                    { Fail("Missing game file " + f + ". Delete the folder " + Payload.AppDir + " and press PLAY again."); return; }
                if (solo && !File.Exists(srvExe))
                { Fail("Missing game file mqelserver.exe. Delete the folder " + Payload.AppDir + " and press PLAY again."); return; }
                string target = solo ? "127.0.0.1" : ip;

                // 1. hosts entry (one UAC prompt, only when it needs to change)
                if (!Hosts.Matches(target))
                {
                    Say("Pointing the game at the server (approve the Windows prompt)...");
                    if (!Hosts.RunElevated("--hosts add " + target) || !Hosts.Matches(target))
                    { Fail("The hosts file was not updated. Click Yes on the Windows prompt, or check that antivirus isn't blocking it."); return; }
                }

                // 2. server or connectivity check
                if (solo)
                {
                    if (!StartServer()) return;
                }
                else
                {
                    Say("Checking connection to " + ip + "...");
                    if (!CanConnect(ip, 443))
                    { Fail("Can't reach " + ip + " on port 443. Is the host's server running, and is the port open in their firewall/router?"); return; }
                }

                // 3. Steam
                if (!EnsureSteam()) return;

                // 4. game
                string exe = FindGame();
                if (exe == null)
                {
                    BeginInvoke(new Action(() =>
                    {
                        if (MessageBox.Show(this, "The Mighty Quest for Epic Loot isn't installed. Open Steam to install it now (free)?",
                            "Lost Worlds", MessageBoxButtons.YesNo) == DialogResult.Yes)
                            Process.Start(new ProcessStartInfo("steam://install/" + AppId) { UseShellExecute = true });
                    }));
                    Say("Install the game from Steam, then press PLAY again.");
                    return;
                }
                Log("Game: " + exe);
                if (Process.GetProcessesByName("MightyQuest").Length > 0)
                { Fail("The game is already running. Close it first, then press PLAY."); return; }

                Say("Starting the game...");
                var psi = new ProcessStartInfo(exe, GameArgs);
                psi.WorkingDirectory = Path.GetDirectoryName(exe);
                psi.UseShellExecute = false;
                psi.EnvironmentVariables["SteamAppId"] = AppId;
                psi.EnvironmentVariables["SteamGameId"] = AppId;
                Process.Start(psi);

                Say("Waiting for the game to unpack...");
                Thread.Sleep(12000);
                if (Process.GetProcessesByName("MightyQuest").Length == 0)
                { Fail("The game closed immediately. Make sure Steam is running and signed in."); return; }

                // 5. cert patch injection (retry a few times)
                bool injected = false;
                for (int i = 1; i <= 3 && !injected; i++)
                {
                    Say("Connecting the game to the server (attempt " + i + ")...");
                    injected = Inject();
                    if (!injected) Thread.Sleep(3000);
                }
                if (!injected)
                { Fail("Couldn't patch the game's certificate check. See launcher.log in the saves folder."); return; }

                Say("Playing! Leave this window open; closing it stops the server.");
                while (Process.GetProcessesByName("MightyQuest").Length > 0) Thread.Sleep(2000);
                Say("Game closed.");
            }
            catch (Exception ex)
            {
                Fail("Unexpected error: " + ex.Message);
            }
            finally
            {
                StopServer();
                SetBusy(false);
            }
        }

        bool StartServer()
        {
            if (Process.GetProcessesByName("mqelserver").Length > 0)
            {
                Say("A server is already running on this PC - using it.");
                weStartedServer = false;
                return true;
            }
            Say("Starting the server...");
            var psi = new ProcessStartInfo(Path.Combine(BinDir, "mqelserver.exe"));
            psi.WorkingDirectory = DataDir; // saves/logs go here
            psi.UseShellExecute = false;
            psi.CreateNoWindow = true;
            serverProc = Process.Start(psi);
            weStartedServer = true;
            Thread.Sleep(3000);
            if (serverProc.HasExited)
            {
                Fail("The server stopped right away (exit code " + serverProc.ExitCode + "). Port 443 is probably in use by another program " +
                     "(e.g. IIS, Skype, another server). Check mqelserver.log in the saves folder.");
                return false;
            }
            Log("Server running.");
            return true;
        }

        void StopServer()
        {
            try
            {
                if (weStartedServer && serverProc != null && !serverProc.HasExited)
                {
                    serverProc.Kill();
                    serverProc.WaitForExit(3000);
                }
            }
            catch { }
            weStartedServer = false;
        }

        static bool CanConnect(string ip, int port)
        {
            try
            {
                using (var c = new TcpClient())
                {
                    IAsyncResult r = c.BeginConnect(ip, port, null, null);
                    if (!r.AsyncWaitHandle.WaitOne(4000)) return false;
                    c.EndConnect(r);
                    return true;
                }
            }
            catch { return false; }
        }

        bool EnsureSteam()
        {
            if (Process.GetProcessesByName("steam").Length > 0) return true;
            Say("Starting Steam (sign in if it asks)...");
            try { Process.Start(new ProcessStartInfo("steam://open/main") { UseShellExecute = true }); }
            catch { Fail("Steam doesn't seem to be installed. Install it from store.steampowered.com, sign in, then press PLAY."); return false; }
            for (int i = 0; i < 60; i++)
            {
                Thread.Sleep(1000);
                if (Process.GetProcessesByName("steam").Length > 0) { Thread.Sleep(8000); return true; }
            }
            Fail("Steam didn't start. Open Steam yourself, sign in, then press PLAY.");
            return false;
        }

        static string FindGame()
        {
            var libs = new System.Collections.Generic.List<string>();
            object steam = Registry.GetValue(@"HKEY_CURRENT_USER\Software\Valve\Steam", "SteamPath", null);
            if (steam == null)
                steam = Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam", "InstallPath", null);
            if (steam != null)
            {
                string root = steam.ToString().Replace('/', '\\');
                libs.Add(root);
                string vdf = Path.Combine(root, @"steamapps\libraryfolders.vdf");
                if (File.Exists(vdf))
                {
                    foreach (string line in File.ReadAllLines(vdf))
                    {
                        Match m = Regex.Match(line, "\"path\"\\s+\"([^\"]+)\"");
                        if (m.Success) libs.Add(m.Groups[1].Value.Replace(@"\\", @"\"));
                    }
                }
            }
            foreach (string l in libs)
            {
                string exe = Path.Combine(l, @"steamapps\common\The Mighty Quest For Epic Loot\GameData\Bin\MightyQuest.exe");
                if (File.Exists(exe)) return exe;
            }
            return null;
        }

        bool Inject()
        {
            try
            {
                var psi = new ProcessStartInfo(Path.Combine(BinDir, "inject.exe"),
                    "MightyQuest.exe \"" + Path.Combine(BinDir, "certpatch.dll") + "\"");
                psi.UseShellExecute = false;
                psi.CreateNoWindow = true;
                psi.RedirectStandardOutput = true;
                psi.WorkingDirectory = BinDir;
                using (Process p = Process.Start(psi))
                {
                    string output = p.StandardOutput.ReadToEnd();
                    p.WaitForExit();
                    Log("inject.exe: " + output.Replace("\r", "").Replace("\n", " | "));
                    return p.ExitCode == 0;
                }
            }
            catch (Exception ex) { Log("inject failed: " + ex.Message); return false; }
        }
    }
}
