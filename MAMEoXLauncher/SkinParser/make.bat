@echo off
cls




@echo Compiling SkinMetadataParser...

@echo BISON...
@echo on
@bison -d -p smp -l SkinMetadataParser.y -oSkinMetadataParser_yacc.cpp
@echo off



@echo FLEX...
@echo on
@flex -i -Psmp -L -oSkinMetadataParser_lex.cpp SkinMetadataParser.l
@echo off




@echo Compiling SkinParser...

@echo BISON...
@echo on
@bison -d -p sp -l SkinParser.y -oSkinParser_yacc.cpp
@echo off



@echo FLEX...
@echo on
@flex -i -Psp -L -oSkinParser_lex.cpp SkinParser.l
@echo off






@echo Compiling MacroParser...

@echo BISON...
@echo on
@bison -d -p mp -oMacroParser_yacc.cpp -l MacroParser.y 
@echo off



@echo FLEX...
@echo on
@flex -i -Pmp -L -oMacroParser_lex.cpp MacroParser.l
@echo off