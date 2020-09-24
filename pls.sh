#!/usr/bin/env bash

# Stop on errors, print commands
# See https://vaneyckt.io/posts/safer_bash_scripts_with_set_euxo_pipefail/
set -Eeuo pipefail
set -x

if [[ $# -eq 0 ]]; then
	echo "Use a command line arg pls -h for help"
	exit 1
fi

(cd /home/sudacode/projects/c++/you-have-to/ && ./pls.exe $@)
