if "%EPICS_HOST_ARCH%"=="" (
	set EPICS_HOST_ARCH=win32-x86-static
)

caput -S 13URL1:cam1:URL1 "http://192.168.23.4:81/videostream.cgi?user=admin&pwd=888888"
caput 13URL1:cam1:URLSelect.ZRST vstarcam
caput 13URL1:cam1:AcquirePeriod 0.0
caput 13URL1:cam1:ArrayCallbacks 1
caput 13URL1:image1:EnableCallbacks 1
caput 13URL1:cam1:Acquire 1
