#!/bin/bash

./curricula -vvv compile $1 \
  --template template \
  "${@:2}"
