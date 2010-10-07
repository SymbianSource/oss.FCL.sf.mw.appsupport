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
REM Description:  Environment setup for ssmlangselcmd tests.
REM
REM
@echo on


@echo Recall environment for syserrcmdtest...
@echo Cleaning up stub...
pushd ..\..\ssmlangselcmdteststub\group
call bldmake bldfiles
call abld test reallyclean -k
popd

@echo Recall environment for syserrcmdtest... Finished.
        
