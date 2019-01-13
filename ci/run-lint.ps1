try
{
    & "cpplint" "--quiet" "--recursive" "src" "test"
	Exit $lastExitCode;
}
catch
{
    Exit 1
}