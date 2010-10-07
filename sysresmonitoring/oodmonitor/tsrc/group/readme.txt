ut_oodmonitor are unit test code for oodmonitor.

How to build test code
go to group directory and run
>sbs reallyclean
>sbs 

How to install test module
Copy related exe, dll, resource file to device
1) \epoc32\release\armv5\udeb\ut_oodmonitor.dll -> c:\sys\bin\ut_oodmonitor.dll
2) \epoc32\release\armv5\udeb\ut_oodsubscriber.dll -> c:\sys\bin\ut_oodsubscriber.dll
3) \epoc32\release\armv5\udeb\ut_oodmonitorv1.dll -> c:\sys\bin\ut_oodmonitorv1.dll
How to run test code at emulator
1)start symbian device emulator
2)start eshell
3)start command at eshell
  atsinterface.exe -testmodule ut_oodmonitor
  atsinterface.exe -testmodule ut_oodsubscriber 
  atsinterface.exe -testmodule ut_oodmonitorv1

How to run test code at hw
1)start symbian device
2)start eshell
3)start command at eshell
  atsinterface.exe -testmodule ut_oodmonitor
  atsinterface.exe -testmodule ut_oodsubscriber 
  atsinterface.exe -testmodule ut_oodmonitorv1
