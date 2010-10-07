ut_oom are unit test code for oom.

How to build test code
go to group directory and run
>sbs reallyclean
>sbs 

How to install test module
Copy related exe, dll, resource file to device
1) \epoc32\release\armv5\udeb\ut_oom.dll -> c:\sys\bin\ut_oom.dll
2) \epoc32\release\armv5\udeb\ut_oomplugin.dll -> c:\sys\bin\ut_oomplugin.dll
3) \epoc32\Data\z\resource\plugins\ut_oomplugin.RSC -> C:\resource\plugins\ut_oomplugin.RSC
4) \epoc32\release\armv5\udeb\ut_oomplugin2.dll -> c:\sys\bin\ut_oomplugin2.dll
5) \epoc32\Data\z\resource\plugins\ut_oomplugin2.RSC -> C:\resource\plugins\ut_oomplugin2.RSC
6) \epoc32\release\armv5\udeb\appfortest.exe -> c:\sys\bin\appfortest.exe
7) \epoc32\release\armv5\udeb\ut_oomwatcher.dll -> c:\sys\bin\ut_oomwatcher.dll
8) \epoc32\release\armv5\udeb\ut_mockoomserver.exe -> c:\sys\bin\ut_mockoomserver.exe
9) \epoc32\release\armv5\udeb\ut_oom_closeapp.dll -> c:\sys\bin\ut_oom_closeapp.dll
10) \epoc32\release\armv5\udeb\ut_oom_memorymonitor.dll -> c:\sys\bin\ut_oom_memorymonitor.dll 
11) \epoc32\winscw\c\testframework\oomconfig.xml -> c:\testframework\oomconfig.xml

How to run test code at emulator
1)start symbian device emulator
2)start eshell
3)start command at eshell
  atsinterface.exe  -testmodule ut_oom
  atsinterface.exe  -testmodule ut_oom_closeapp 
  atsinterface.exe  -testmodule ut_oomwatcher
  atsinterface.exe  -testmodule ut_oom_memorymonitor

How to run test code at hw
1)start symbian device
2)start eshell
3)start command at eshell
  atsinterface.exe  -testmodule ut_oom
  atsinterface.exe  -testmodule ut_oom_closeapp 
  atsinterface.exe  -testmodule ut_oomwatcher
  atsinterface.exe  -testmodule ut_oom_memorymonitor
