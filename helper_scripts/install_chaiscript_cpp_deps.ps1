New-Item -Path 'C:\dev\vcpkg' -ItemType Directory
Set-Location -Path 'C:\dev\vcpkg'
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe install chaiscript:x64-windows-static
.\vcpkg\vcpkg.exe integrate install
