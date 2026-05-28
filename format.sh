# !/bin/bash

# Usage: ./format.sh [target_branch] - when no target branch is provided, it defaults to "origin/main"

# Example: 
# ./format.sh
# ./format.sh origin/develop

TARGET_BRANCH="${1:-origin/main}"

git diff --name-only --diff-filter=AM "$TARGET_BRANCH" \
| grep -E '\.(c|h|cpp|hpp|cc|cxx)$' \
| xargs -r clang-format-21 --style=file -i