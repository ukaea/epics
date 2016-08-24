set PATH=%PATH%;C:\cygwin\bin
start medm -x -macro "P=13PSL1:, R=cam1:" PSL.adl &
..\..\bin\cygwin-x86\PSLApp st.cmd
pause

