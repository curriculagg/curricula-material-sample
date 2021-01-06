#!/bin/bash

function use_local() {
  if [ -d "$1" ]; then
    echo "using local $1"
    export PYTHONPATH="$1":$PYTHONPATH
  fi
}

ORIGINAL_PYTHONPATH=$PYTHONPATH

use_local ../curricula
use_local ../curricula-grade
use_local ../curricula-grade-cpp
use_local ../curricula-compile

# Mirror commands
python3.9 -m curricula "$@"

# Fix path after
export PYTHONPATH=$ORIGINAL_PYTHONPATH
