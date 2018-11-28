REM start medm -x -macro "P=13PS1:, R=cam1:" simDetector.adl
set TOP=..\\..
set ADSIMDETECTOR=..\\..\\..\\..
set ADCORE=../../../../../ADCore
set CALC=/epics/support/calc
%ADSIMDETECTOR%\iocs\simDetectorIOC\bin\%EPICS_HOST_ARCH%\simDetectorApp st.cmd


