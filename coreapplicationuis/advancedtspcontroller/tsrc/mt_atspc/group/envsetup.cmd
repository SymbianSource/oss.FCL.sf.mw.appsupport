@echo off
REM Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
REM All rights reserved.
REM This component and the accompanying materials are made available
REM under the terms of "Eclipse Public License v1.0"
REM which accompanies this distribution, and is available
REM at the URL "http://www.eclipse.org/legal/epl-v10.html".
REM
REM Initial Contributors:
REM Nokia Corporation - initial contribution.
REM
REM Contributors:
REM
REM Description:  ?Description
REM
REM
@echo on

@echo Copying remconservercaps.mmh to safe place for envrecall
copy \epoc32\include\remcon\remconservercaps.mmh remconservercaps_orig.mmh

@echo Setting up advanced tsp test stub...
pushd ..\..\stub\group
call bldmake bldfiles
call abld test reallyclean
call abld test build
popd
@echo Setting up advanced tsp test stub... Finished
