##########################################################################################
# Script that modifies visual studio on appveyor to have MFC installed
##########################################################################################

Write-Host "Installing MFC dependencies for Visual Studio Community 2019"
&"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe" modify --installPath "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community" --add Microsoft.VisualStudio.Component.VC.ATLMFC --add Microsoft.VisualStudio.Component.VC.ATLMFC.Spectre --quiet | Out-Null
Write-Host "Finished Installing MFC dependencies for Visual Studio Community 2019"
