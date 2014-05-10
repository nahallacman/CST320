Instructions for building:
Create project and add all files in folder besides this README file to the project
Right click the Project name in the solution explorer and go to properties.
Under Configuration Properties->C/C++->Preprocessor, add _CRT_SECURE_NO_WARNINGS to the preprocessor definitons. (If you don't do this visual studios complains about strcpy and strncpy)

Program is run simply by debugging. Target program test files are changed in main.cpp line 27.
The default program is Program12.txt.

To reach the intended output the program will promt the user to "press any key to continue..." twice. 
After the first and just before the second prompt there will be a line that will read either "parse succeeded" or "parse failed".


 -------------------------------------- List of files ---------------------------------------------- 
Header files:
DFA.h
DFAWrapper.h
LexicalCheckNode.h
Preprocessor.h
RecursiveParser.h
RecursiveParser_old.h
Symbol.h
SymbolTable.h
Token.h

.cpp files:
DFA.cpp
DFAWrapper.cpp
LexicalCheckNode.cpp
Main.cpp
Preprocessor.cpp
RecursiveParser.cpp
RecursiveParser_old.h
Symbol.cpp
SymbolTable.cpp
Token.cpp

Test program files:
Program6.txt
Program7.txt
Program8.txt
Program10.txt
Program11.txt
Program12.txt

DFA files:
LegalCVariable.txt
LegalLabel.txt
LegalNumericConstnant.txt

//keywords
char.txt
else.txt
goto.txt
if.txt
int.txt
main.txt
printf.txt
return.txt
scanf.txt

//operators
asterix.txt
asterixasterix.txt
equals.txt
equalsequals.txt
forwardslash.txt
greaterthan.txt
greaterthanorequalto.txt
lessthan.txt
lessthanorequalto.txt
minus.txt
minusminus.txt
percent.txt
plus.txt
plusplus.txt

//symbols
ampersand.txt
closebracket.txt
closeparen.txt
comma.txt
openbracket.txt
openparen.txt
pipe.txt
semicolon.txt

//intermediate files:
.ppc: PreProcessor Comments intermediate file
.ppd: PreProcessor Directive output file
.ltl: Lexically-analyzed Token List output file