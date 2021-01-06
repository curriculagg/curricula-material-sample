@echo off

for /f "tokens=1,* delims= " %%a in ("%*") do set REST=%%b

.\curricula.bat -vvv compile "%1" ^
  --template template ^
  --site ../curricula-web-content/assignments/ ^
  --grade %REST%
