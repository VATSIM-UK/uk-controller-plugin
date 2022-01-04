Function Build-Version {
    # If we're on main, invoke semantic release to get the next version
    if (${env:GITHUB_REF} -eq "refs/heads/main")
    {
        $semanticRelease = yarn semantic-release --dry-run | Select-String -Pattern "The next release version is (.*)"
        if ($semanticRelease.Matches -eq $null)
        {
            return "non-release-build"
        }
        else
        {
            return $semanticRelease.Matches.Groups[1].value;
        }
    } else
    {
        return "non-release-build"
    }
}

$buildVersion = Build-Version;
echo "Building version $buildVersion"
$versionFile = ".\\src\\utils\\update\\PluginVersion.cpp"
(Get-Content $versionFile).replace("#VERSION_STRING#", "$buildVersion") | Set-Content $versionFile
