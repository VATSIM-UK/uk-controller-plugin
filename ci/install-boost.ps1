try
{
	$previousCwd = Get-Location;

	# Download boost
	$source = "https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.7z"
	$destination = "C:\Libraries\boost_1_70_0.7z"
	Invoke-WebRequest $source -OutFile $destination

	# Unzip it
	Set-Location -Path C:\Libraries\
	7z x boost_1_70_0.7z -oC:\Libraries

	# Run the boost build
	Set-Location -Path C:\Libraries\boost_1_70_0
	$null = .\bootstrap.bat
	$null = .\b2.exe -j 2 architecture=x86 address-model=32 threading=multi variant=release

	$buildExitCode = $lastExitCode;

	Set-Location $previousCwd;
	Exit $buildExitCode;
}
catch
{
	Exit 1;
}
