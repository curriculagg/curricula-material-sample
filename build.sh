#!/bin/bash

./curricula.sh -vvv compile $1 \
  --template template \
  --grade \
  "${@:2}"
