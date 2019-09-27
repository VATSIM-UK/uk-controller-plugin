try
{
	# Download boost
	$source = "https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.7z"
	$destination = "C:\Libraries\boost_1_70_0.7z"
	Invoke-WebRequest $source -OutFile $destination

	# Unzip it
	Set-Location -Path C:\Libraries\
	7z x boost_1_70_0.7z -oC:\Libraries

	# Run the boost build
	Set-Location -Path C:\Libraries\boost_1_70_0
	.\bootstrap.bat
	.\b2.exe architecture=x86 address-model=32

	Exit $lastExitCode;
}
catch
{
	Exit 1;
}
