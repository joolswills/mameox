@echo off


echo Prerequisites:
echo  - Run GenerateMAMEVCPROJFile.pl to add/remove files from Visual Studio
echo  - Run ChangeMallocs.pl to change *alloc calls to osd_*alloc
echo  - Build a debug version of the MAME core
echo ================================================================
perl .\Desectionize.pl
echo ================================================================
perl .\FindDependencies.pl
echo ================================================================
perl .\Sectionize.pl
echo ================================================================
echo Process completed.

