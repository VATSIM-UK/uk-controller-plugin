# !/bin/bash

git diff --name-only --diff-filter=AM origin/main \
| grep -E '\.(c|h|cpp|hpp|cc|cxx)$' \
| xargs -r clang-format-21 --style=file -i