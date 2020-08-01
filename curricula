#!/bin/bash

# Respect development environment
ORIGINAL_PYTHONPATH=$PYTHONPATH
if [ -d ../curricula-shell ]; then
  echo "using local curricula-shell"
  export PYTHONPATH=../curricula-shell:$PYTHONPATH
fi
if [ -d ../curricula ]; then
  echo "using local curricula"
  export PYTHONPATH=../curricula:$PYTHONPATH
fi

# Mirror commands
python3.7 -m curricula_shell "$@"

# Fix path after
export PYTHONPATH=$ORIGINAL_PYTHONPATH
