if "%EPICS_HOST_ARCH%"=="" (
	set EPICS_HOST_ARCH=win32-x86-static
)

REM caput -S 13URL1:cam1:URL1 http://192.168.23.10/cgi-bin/camera
REM caput -S 13URL1:cam1:URL1 http://192.168.23.10/cgi-bin/mjpeg
REM caput -S 13URL1:cam1:URL1 http://208.72.70.171/mjpg/1/video.mjpg
REM caput -S 13URL1:cam1:URL1 http://131.173.8.23/mjpg/video.mjpg
caput -S 13URL1:cam1:URL1 http://70.185.95.146/mjpg/video.mjpg



caput 13URL1:cam1:URLSelect.ZRST Panasonic BL-VP101
caput 13URL1:cam1:AcquirePeriod 0.0
caput 13URL1:cam1:ArrayCallbacks 1
caput 13URL1:image1:EnableCallbacks 1
caput 13URL1:cam1:Acquire 1
