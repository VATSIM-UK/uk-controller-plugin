Function Build-Version {
    # If we're on main, invoke semantic release to get the next version
    if (${env:GITHUB_REF} -eq "refs/heads/main")
    {
        $semanticRelease = yarn semantic-release --dry-run | Select-String -Pattern "Published release ((0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)"
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
