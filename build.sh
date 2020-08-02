#!/bin/bash

./curricula -vvv compile $1 \
  --template template \
  --site ../curricula-web-content/assignments/ \
  --grade "${@:2}"
