if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

REM misc
%~dp0..\bin\%EPICS_HOST_ARCH%\testBaseException.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testBitSet.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testByteBuffer.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testByteOrder.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testMessageQueue.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testQueue.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testSerialization.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testThread.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testTimer.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testTimeStamp.exe

REM monitor
%~dp0..\bin\%EPICS_HOST_ARCH%\testMonitor.exe

REM property
%~dp0..\bin\%EPICS_HOST_ARCH%\testProperty.exe

REM pv

%~dp0..\bin\%EPICS_HOST_ARCH%\testBitSetUtil.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testConvert.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testIntrospect.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testOperators.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVAppend.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVAuxInfo.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVData.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVScalarArray.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVStructureArray.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testPVType.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testStandardField.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testStandardPVField.exe
