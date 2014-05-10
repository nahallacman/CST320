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

int test()
{
	int a ;
	a = 2 ;
	input ( a ) ;
}

int main() 
{	
	int b;
	b = 1;
	input ( b ) ;
} 

--------------------------------------Example output:------------------------------------------

 Starting Preprocessor
Removing Comments --
int test()
{
        int a ;
        a = 2 ;
        input ( a ) ;
}
int main()
{
        int b;
        b = 1;
        input ( b ) ;
}
Handling preprocessor directives --
int test()
{
        int a ;
        a = 2 ;
        input ( a ) ;
}
int main()
{
        int b;
        b = 1;
        input ( b ) ;
}

--------Begin Symbol Table--------
---------End Symbol Table---------
 Starting Lexical Analyzer
int             Keyword
test            Identifier
(               Symbol
)               Symbol
{               Symbol
int             Keyword
a               Identifier
;               Symbol
a               Identifier
=               Operator
2               Numeric Constant
;               Symbol
input           Keyword
(               Symbol
a               Identifier
)               Symbol
;               Symbol
}               Symbol
int             Keyword
main            Keyword
(               Symbol
)               Symbol
{               Symbol
int             Keyword
b               Identifier
;               Symbol
b               Identifier
=               Operator
1               Numeric Constant
;               Symbol
input           Keyword
(               Symbol
b               Identifier
)               Symbol
;               Symbol
}               Symbol
Press any key to continue . . .
Begin stack based rule tree. Printing from the top of the stack.
Statement
Statement_Group
S2
Primary
P2
Primary
Statement
Statement_Group
Identifier_List
Data_Definition
Data_Definition_List
Brackets
Func_Args
Function_Definition
D2
Definition
Program
Statement
Statement_Group
S2
Primary
P2
Primary
Statement
Statement_Group
Identifier_List
Data_Definition
Data_Definition_List
Brackets
Func_Args
Function_Definition
Identifier_List
Data_Definition
D2
Definition
Program
Start
End stack based rule tree.
Parse succeeded
--------Begin Symbol Table--------
Symbol: a  is a variable a defined as: 2
Symbol: b  is a variable b defined as: 1
Symbol: main  is a Function main ( )  defined as: { int b ; b = 1 ; input ( b )
; }
Symbol: test  is a Function test ( )  defined as: { int a ; a = 2 ; input ( a )
; } int
---------End Symbol Table---------
Press any key to continue . . .