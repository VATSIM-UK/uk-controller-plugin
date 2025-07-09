#!/usr/bin/env bash
set -e

build_version() {
    # If we're on main, alpha, or beta, use semantic-release to get the next version
    if [[ "$GITHUB_REF" == "refs/heads/main" || "$GITHUB_REF" == "refs/heads/alpha" || "$GITHUB_REF" == "refs/heads/beta" ]]; then
        semantic_release_output=$(yarn semantic-release --dry-run 2>&1)
        if [[ $semantic_release_output =~ Published\ release\ ([0-9]+\.[0-9]+\.[0-9]+(-[A-Za-z0-9.-]+)?(\+[A-Za-z0-9.-]+)?) ]]; then
            echo "${BASH_REMATCH[1]}"
        else
            echo "non-release-build"
        fi
    else
        echo "non-release-build"
    fi
}

build_version=$(build_version)
echo "Building version $build_version"
version_file="./src/utils/update/PluginVersion.cpp"
if [[ -f "$version_file" ]]; then
    sed -i "s/#VERSION_STRING#/$build_version/g" "$version_file"
else
    echo "Warning: $version_file not found. Skipping version replacement."
fi
