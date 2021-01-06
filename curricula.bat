@echo off

REM Respect development environment
set ORIGINAL_PYTHONPATH=%PYTHONPATH%
if exist ../curricula-shell (
  echo using local curricula-shell
  set PYTHONPATH=../curricula-shell;%PYTHONPATH%
)
if exist ../curricula (
  echo using local curricula
  set PYTHONPATH=../curricula;%PYTHONPATH%
)

REM Mirror commands
python -m curricula_shell %*

REM Fix path after
set PYTHONPATH=%ORIGINAL_PYTHONPATH%
