@echo off
rem Batch generated by LSD
echo Processing 5 configuration files in up to 8 parallel processes...
if "%~1"=="" (set LSD_EXEC="C:\LSD\Work\MMM_v.3\lsdNW.exe") else (set LSD_EXEC="%~1")
if "%~2"=="" (set LSD_CONFIG_PATH="C:\LSD\Work\MMM_v.3") else (set LSD_CONFIG_PATH="%~2")
set LSD_EXEC=%LSD_EXEC:"=%
set LSD_CONFIG_PATH=%LSD_CONFIG_PATH:"=%
echo LSD executable: %LSD_EXEC%
echo Configuration path: %LSD_CONFIG_PATH%
echo Use Sim.bat LSD_EXEC CONFIG_PATH to change defaults
start "LSD Process 1" /B "%LSD_EXEC%" -c 1 -f "%LSD_CONFIG_PATH%\Sim_1.lsd"    1> "%LSD_CONFIG_PATH%\Sim_1.log" 2>&1
start "LSD Process 2" /B "%LSD_EXEC%" -c 1 -f "%LSD_CONFIG_PATH%\Sim_2.lsd"    1> "%LSD_CONFIG_PATH%\Sim_2.log" 2>&1
start "LSD Process 3" /B "%LSD_EXEC%" -c 1 -f "%LSD_CONFIG_PATH%\Sim_3.lsd"    1> "%LSD_CONFIG_PATH%\Sim_3.log" 2>&1
start "LSD Process 4" /B "%LSD_EXEC%" -c 1 -f "%LSD_CONFIG_PATH%\Sim_4.lsd"    1> "%LSD_CONFIG_PATH%\Sim_4.log" 2>&1
start "LSD Process 5" /B "%LSD_EXEC%" -c 1 -f "%LSD_CONFIG_PATH%\Sim_5.lsd"    1> "%LSD_CONFIG_PATH%\Sim_5.log" 2>&1
echo 5 log files being generated: Sim_1.log to Sim_5.log .
