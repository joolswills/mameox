@echo Creating MAMEoX Debug XBE file
imagebld.exe @..\imageblddbg.switch


@echo Copying file to XBOX
xbecopy.exe "Debug\MAMEoX.XBE" "xe:\MAMETEST\MAMEoX.xbe" /nologo 

