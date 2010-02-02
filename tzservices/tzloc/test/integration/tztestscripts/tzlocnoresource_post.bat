REM Move the required localization files from temporary directory to original directory

move \epoc32\release\winscw\udeb\z\resource\temp\timezones.r01 \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r01 

move \epoc32\release\winscw\udeb\z\resource\temp\timezones.r02 \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r02

move \epoc32\release\winscw\udeb\z\resource\temp\timezones.rsc \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.rsc


REM Delete the temporary directory once the test script has completed its run

rd \epoc32\release\winscw\udeb\z\resource\temp