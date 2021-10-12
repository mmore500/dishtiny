#!/bin/bash

set -e

./ci/test_alphabetize_includes.sh && echo "✔ include alphabetization ok" || exit 1
./ci/test_alphabetize_imports.sh && echo "✔ import alphabetization ok" || exit 1
./ci/test_partition_includes.sh && echo "✔ include partitioning ok" || exit 1
./ci/test_headerguards.sh && echo "✔ headerguards ok" || exit 1
./ci/test_merge_conflict_markers.sh && echo "✔ conflict markers ok" || exit 1
./ci/test_end_of_file_newlines.sh && echo "✔ EOF newlines ok" || exit 1
./ci/test_tabs.sh && echo "✔ no-tab indentation ok" || exit 1
./ci/test_trailing_whitespace.sh && echo "✔ trailing whitespace ok" || exit 1
./ci/test_notebooks_clear.sh && echo "✔ notebooks ok" || exit 1
./ci/test_2space_indentation.sh && echo "✔ two-space indentation ok" || exit 1
./ci/test_filename_whitespace.sh && echo "✔ filename whitespace ok" || exit 1
./ci/test_modern_suffixes.sh && echo "✔ source file suffixes ok" || exit 1
editorconfig-checker && echo "✔ editorconfig rules ok" || exit 1
./ci/test_make_clean.sh && echo "✔ no compiled artifacts" || exit 1
