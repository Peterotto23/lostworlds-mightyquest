LOST WORLDS - THE MIGHTY QUEST FOR EPIC LOOT (private server)
==============================================================

BEFORE YOU START (one time)
  1. Install Steam and sign in:  https://store.steampowered.com
  2. Install the game from Steam. It is free. Paste this into your browser:
         steam://install/239220
     (This project does not include the game itself.)

HOW TO PLAY
  1. Unzip this whole folder somewhere (e.g. your Documents folder).
     Don't run it from inside the zip.
  2. Double-click  LostWorldsMQ.exe
  3. Pick:
       - "Play on this PC"  -> the launcher runs the server for you
       - "Join a friend's server" -> type the host's IP address
  4. Click PLAY.
     The first time, Windows asks permission to edit the "hosts" file - click Yes.
     That is how the game is pointed at the private server instead of the
     original (shut down) one. Windows Firewall may also ask about
     "mqelserver" - allow it on Private networks if you want friends to join you.
  5. Leave the launcher window open while you play. Closing it stops the server.

IF WINDOWS / ANTIVIRUS WARNS YOU
  The launcher is not code-signed, so Windows SmartScreen shows "Windows protected
  your PC" -> click "More info" -> "Run anyway".
  Some antivirus tools also flag it, because this project does two things that
  look suspicious to them even though they're how it works: it edits the hosts
  file and it patches the game's certificate check (cert-patch DLL). The full
  source is public on GitHub if you'd like to check it.

TO UNDO EVERYTHING
  Click "Undo hosts change" in the launcher, then delete the folder.
  Your saves are in  %LOCALAPPDATA%\LostWorldsMQ  ("Open saves folder" button).

HOSTING FOR FRIENDS
  Pick "Play on this PC". Friends need your IP address and TCP port 443 open
  to your PC (firewall, and port-forwarding if they're not on your network).

TROUBLESHOOTING
  - "Port 443 in use": another program is using it (IIS, Skype, another server).
  - Game closes instantly: Steam must be running, signed in, and the game installed.
  - Logs: launcher.log and mqelserver.log in the saves folder.
  - Stuck during the new-account intro (cutscene / class selection)? Close the game and the
    launcher, create an empty file named  use-starter-save.txt  in the saves folder, delete
    Plugins\MQELOffline.Localnet there, and press PLAY again. You then start as a level 1
    Archer and skip the intro.

The Mighty Quest for Epic Loot is (c) Ubisoft. This is an unofficial,
non-commercial preservation project, unaffiliated with Ubisoft. See CREDITS.md.
