# package.ps1 - build the player download: ONE file, dist\LostWorldsMQ.exe
#
# Everything the game needs (server, injector, DLLs, static game data) is embedded inside the
# launcher exe, so players download a single file and double-click it.
#
#   .\package.ps1                 package whatever is in bin\ (warns if bin\mqelserver.exe is stale)
#   .\package.ps1 -Rebuild        run build.ps1 first (recompiles the server), then package
#   .\package.ps1 -NoGameData     leave the Plugins\ game-data folders out of the exe
#
# Needs NOTHING beyond Windows itself to package: it compiles Launcher\MQLauncher.cs with the C#
# compiler that ships with Windows (csc.exe, .NET Framework 4.x). -Rebuild additionally needs the
# Visual Studio C++ toolchain that build.ps1 already requires.
param(
    [switch]$Rebuild,
    [switch]$NoGameData
)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

if ($Rebuild) {
    Write-Host "Rebuilding the server and cert-shim (build.ps1)..."
    & (Join-Path $root 'build.ps1')
}

$csc = Join-Path $env:windir 'Microsoft.NET\Framework\v4.0.30319\csc.exe'
if (-not (Test-Path $csc)) { throw "csc.exe not found at $csc" }

foreach ($f in 'mqelserver.exe','inject.exe','certpatch.dll','libssl-3.dll','libcrypto-3.dll') {
    if (-not (Test-Path (Join-Path $root "bin\$f"))) { throw "bin\$f is missing. Run build.ps1 first (or restore bin\ from git)." }
}

# Catch the classic mistake: editing the C++ and packaging without recompiling.
$srv = Get-Item (Join-Path $root 'bin\mqelserver.exe')
$srcDirs = @((Join-Path $root 'src\mqeloffline'), (Join-Path $root 'src\mqelserver')) | Where-Object { Test-Path $_ }
$newest = Get-ChildItem -Path $srcDirs -Recurse -File -Include *.cpp,*.hpp,*.h -ErrorAction SilentlyContinue |
    Sort-Object LastWriteTime -Descending | Select-Object -First 1
Write-Host ("bin\mqelserver.exe was built " + $srv.LastWriteTime.ToString('yyyy-MM-dd HH:mm'))
if ($newest -and $newest.LastWriteTime -gt $srv.LastWriteTime) {
    Write-Warning ("bin\mqelserver.exe is OLDER than source file " + $newest.Name + " (" + $newest.LastWriteTime.ToString('yyyy-MM-dd HH:mm') + "). " +
        "Run  .\package.ps1 -Rebuild  or build.ps1, otherwise players get the OLD server.")
}

# ---- stage the payload (mirrors %LOCALAPPDATA%\LostWorldsMQ) ----
$dist = Join-Path $root 'dist'
if (Test-Path $dist) { Remove-Item $dist -Recurse -Force }
$stage = Join-Path $dist 'payload'
New-Item -ItemType Directory -Force -Path "$stage\app\bin" | Out-Null

Write-Host "Staging server + cert-patch files..."
foreach ($f in 'mqelserver.exe','inject.exe','certpatch.dll','libssl-3.dll','libcrypto-3.dll','legacy.dll') {
    $src = Join-Path $root "bin\$f"
    if (Test-Path $src) { Copy-Item $src "$stage\app\bin\" -Force }
}

# Static game data the server loads from .\Plugins\ (NOT the player's save, MQELOffline.Localnet).
if (-not $NoGameData) {
    Write-Host "Staging game data..."
    foreach ($d in 'MQELOffline-Catalog','MQELOffline-RewardSettings','MQELOffline-Castles') {
        $src = Join-Path $root "Plugins\$d"
        if (Test-Path $src) {
            New-Item -ItemType Directory -Force -Path "$stage\Plugins" | Out-Null
            Copy-Item $src "$stage\Plugins\" -Recurse -Force
        } else {
            Write-Warning "Plugins\$d not found - the exe will not contain it."
        }
    }
}

Add-Type -AssemblyName System.IO.Compression.FileSystem
$payload = Join-Path $dist 'payload.zip'
[System.IO.Compression.ZipFile]::CreateFromDirectory($stage, $payload, [System.IO.Compression.CompressionLevel]::Optimal, $false)

# ---- compile the launcher with the payload embedded as a resource ----
Write-Host "Compiling LostWorldsMQ.exe..."
$out = Join-Path $dist 'LostWorldsMQ.exe'
& $csc /nologo /target:winexe /optimize+ "/out:$out" "/resource:$payload,payload.zip" `
    /reference:System.Windows.Forms.dll /reference:System.Drawing.dll `
    /reference:System.IO.Compression.dll /reference:System.IO.Compression.FileSystem.dll `
    (Join-Path $root 'Launcher\MQLauncher.cs')
if ($LASTEXITCODE -ne 0) { throw "Launcher compile failed." }

Remove-Item $stage -Recurse -Force
Remove-Item $payload -Force
Copy-Item (Join-Path $root 'PLAYER-README.txt') (Join-Path $dist 'README.txt') -Force

$mb = [math]::Round((Get-Item $out).Length / 1MB, 1)
Write-Host ""
Write-Host "Done: $out  ($mb MB)"
Write-Host "Upload that ONE file to a GitHub Release. Players just double-click it."
