try
{
    & ".\bin\Release\UKControllerPluginTest.exe"
    Exit $LastExitCode
}
catch
{
    Exit 1
}