if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

REM misc
..\bin\%EPICS_HOST_ARCH%\testBaseException.exe
..\bin\%EPICS_HOST_ARCH%\testBitSet.exe
..\bin\%EPICS_HOST_ARCH%\testByteBuffer.exe
..\bin\%EPICS_HOST_ARCH%\testByteOrder.exe
..\bin\%EPICS_HOST_ARCH%\testMessageQueue.exe
..\bin\%EPICS_HOST_ARCH%\testQueue.exe
..\bin\%EPICS_HOST_ARCH%\testSerialization.exe
..\bin\%EPICS_HOST_ARCH%\testThread.exe
..\bin\%EPICS_HOST_ARCH%\testTimer.exe
..\bin\%EPICS_HOST_ARCH%\testTimeStamp.exe

REM monitor
..\bin\%EPICS_HOST_ARCH%\testMonitor.exe

REM property
..\bin\%EPICS_HOST_ARCH%\testProperty.exe

REM pv

..\bin\%EPICS_HOST_ARCH%\testBitSetUtil.exe
..\bin\%EPICS_HOST_ARCH%\testConvert.exe
..\bin\%EPICS_HOST_ARCH%\testIntrospect.exe
..\bin\%EPICS_HOST_ARCH%\testOperators.exe
..\bin\%EPICS_HOST_ARCH%\testPVAppend.exe
..\bin\%EPICS_HOST_ARCH%\testPVAuxInfo.exe
..\bin\%EPICS_HOST_ARCH%\testPVData.exe
..\bin\%EPICS_HOST_ARCH%\testPVScalarArray.exe
..\bin\%EPICS_HOST_ARCH%\testPVStructureArray.exe
..\bin\%EPICS_HOST_ARCH%\testPVType.exe
..\bin\%EPICS_HOST_ARCH%\testStandardField.exe
..\bin\%EPICS_HOST_ARCH%\testStandardPVField.exe
