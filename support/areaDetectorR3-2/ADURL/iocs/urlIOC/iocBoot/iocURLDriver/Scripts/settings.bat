if "%EPICS_HOST_ARCH%"=="" (
	set EPICS_HOST_ARCH=win32-x86-static
)

REM caput -S 13URL1:cam1:URL1 http://192.168.23.10/cgi-bin/camera
caput -S 13URL1:cam1:URL1 http://192.168.23.10/cgi-bin/mjpeg


caput 13URL1:cam1:URLSelect.ZRST Panasonic BL-VP101
caput 13URL1:cam1:AcquirePeriod 0.0
caput 13URL1:cam1:ArrayCallbacks 1
caput 13URL1:image1:EnableCallbacks 1
caput 13URL1:cam1:Acquire 1
