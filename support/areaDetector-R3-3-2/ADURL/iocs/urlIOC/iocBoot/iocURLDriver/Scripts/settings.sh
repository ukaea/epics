caput -S 13URL1:cam1:URL1 http://192.168.100.10/cgi-bin/mjpeg
# caput -S 13URL1:cam1:URL1 http://192.168.100.10/cgi-bin/camera
caput 13URL1:cam1:URLSelect.ZRST Panasonic BL-VP101
caput 13URL1:cam1:AcquirePeriod 0.03
caput 13URL1:cam1:ArrayCallbacks 1
caput 13URL1:image1:EnableCallbacks 1
caput 13URL1:cam1:Acquire 1