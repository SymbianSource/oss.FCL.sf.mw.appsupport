REM Create a temporary directory to store the original localization resource files

mkdir \epoc32\release\winscw\udeb\z\resource\temp


REM Move the required localization files to another directory just to simulate the "missing localization file" condition

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r01 \epoc32\release\winscw\udeb\z\resource\temp\timezones.r01

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.rsc \epoc32\release\winscw\udeb\z\resource\temp\timezones.rsc

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r02 \epoc32\release\winscw\udeb\z\resource\temp\timezones.r02