Instructions for building:
Create project and add all files in folder besides this README file to the project
Right click the Project name in the solution explorer and go to properties.
Under Configuration Properties->C/C++->Preprocessor, add _CRT_SECURE_NO_WARNINGS to the preprocessor definitons. (If you don't do this visual studios complains about strcpy and strncpy)

Program is run simply by debugging. Target program test files are changed in main.cpp line 27.
The default program is Language3Test.txt and it works for my subset of my grammar.
To run the first language test, use Language1Test.txt and in file LL1Parser, comment out the propper buildRuleTable() and uncomment the first buildRuleTable().
The second language uses Language2Test.txt 

To reach the intended output the program will promt the user to "press any key to continue..." twice. 

A successful parse will show something like:
Matched a token: int
Matched a token: a1
Matched a token: (
Matched a token: a2
Matched a token: )
Matched a token: {
Matched a token: int
Matched a token: a
Matched a token: ;
Matched a token: }
Matched a token: $
Parse succeeded
Press any key to continue . . .

An unsuccessful parse will show something like:
Matched a token: int
Matched a token: a1
Matched a token: (
Parse failed
Error when trying to find rule [ ID , 2 ]
Press any key to continue . . .


 -------------------------------------- List of files ---------------------------------------------- 
Header files:
DFA.h
DFAWrapper.h
LexicalCheckNode.h
LL1Parser.h
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

Old Test program files:
Program6.txt
Program7.txt
Program8.txt
Program10.txt
Program11.txt
Program12.txt

New Test program files:
Language1Test.txt
Language2Text.txt
Language3Text.txt

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
input.txt
output.txt

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

--------------------------------------Example input:-------------------------------------------

int a1 ( 2 )
{
	int a;
}

--------------------------------------Example output:------------------------------------------
 Starting Preprocessor
Removing Comments --
int a1 ( a2 )
{
        int a;
}
Handling preprocessor directives --
int a1 ( a2 )
{
        int a;
}

--------Begin Symbol Table--------
---------End Symbol Table---------
 Starting Lexical Analyzer
int             Keyword
a1              Identifier
(               Symbol
a2              Identifier
)               Symbol
{               Symbol
int             Keyword
a               Identifier
;               Symbol
}               Symbol
Press any key to continue . . .
Matched a token: int
Matched a token: a1
Matched a token: (
Matched a token: a2
Matched a token: )
Matched a token: {
Matched a token: int
Matched a token: a
Matched a token: ;
Matched a token: }
Matched a token: $
Parse succeeded
Press any key to continue . . .