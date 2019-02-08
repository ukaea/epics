caput -S 13URL1:cam1:URL1 "http://192.168.100.4:81/videostream.cgi?user=admin&pwd=888888"
caput 13URL1:cam1:URLSelect.ZRST vstarcam
caput 13URL1:cam1:AcquirePeriod 0.0
caput 13URL1:cam1:ImageMode "Continuous"
caput 13URL1:cam1:ArrayCallbacks 1
caput -S 13URL1:cam1:NDAttributesFile "./test5_attributes.xml"
caput 13URL1:cam1:Acquire 1
caput 13URL1:HDF1:EnableCallbacks 1
caput 13URL1:image1:EnableCallbacks 1
caput -S 13URL1:HDF1:FilePath "./"
caput -S 13URL1:HDF1:FileName "JETcam1"
caput -S 13URL1:HDF1:FileTemplate "%s%s_%3.3d.h5"
caput -S 13URL1:HDF1:XMLFileName "./test5_layout.xml"
caput 13URL1:HDF1:AutoIncrement 1
caput 13URL1:HDF1:FileNumber 1
caput 13URL1:HDF1:FileWriteMode "Stream"
caput 13URL1:HDF1:NumCapture "0"
caput 13URL1:HDF1:Capture 1
caget 13URL1:HDF1:XMLValid_RBV
caget -S 13URL1:HDF1:XMLErrorMsg_RBV

sleep 3
caput 13URL1:HDF1:Capture 0
sleep 5
h5dump JETcam1.h5 > JETcam1.txt
