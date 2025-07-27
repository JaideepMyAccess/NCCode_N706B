@cls 
@echo off 
@xcopy /y /b /s configurations\releasepack-ASR1605-source\images\ASR1605S_LTEOnly_SMS_DataModule configurations\releasepack-ASR1605-source\images\ 
@arelease.exe -c configurations\releasepack-ASR1605-source -g -p ASR1605_EVB -v ASR1605_SMS_04MB -i app=app.bin nwy_open_app.zip 
