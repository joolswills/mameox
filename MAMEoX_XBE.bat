@echo Creating MAMEoX Release XBE file
imagebld.exe @..\imagebld.switch


@echo Copying file to XBOX
xbecopy.exe "Release\MAMEoX.XBE" "xe:\MAMETEST\MAMEoX.xbe" /nologo 

