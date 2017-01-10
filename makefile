
p2 : ReadFile.o Lexer.o ProjectOne.o Parser.o Stack.o Token.o TreeNode.o CSEMachine.o standardizer.o
	g++ -o p2 ReadFile.o Lexer.o ProjectOne.o Parser.o Stack.o Token.o TreeNode.o CSEMachine.o standardizer.o
CSEMachine.o: CSEMachine.cpp CSEMachine.h Token.h TreeNode.h
	g++ -c CSEMachine.cpp
standardizer.o: standardizer.cpp standardizer.h TreeNode.h
	g++ -c standardizer.cpp
ReadFile.o : ReadFile.cpp ReadFile.h
	 g++  -c ReadFile.cpp
Lexer.o : Lexer.cpp Lexer.h Token.h
	g++  -c Lexer.cpp
ProjectOne.o : ProjectOne.cpp ReadFile.h Lexer.h Parser.h
	g++ -c ProjectOne.cpp

Parser.o : Parser.cpp Parser.h Token.h
	g++ -c Parser.cpp

Stack.o : Stack.cpp Stack.h
	g++ -c Stack.cpp

Token.o : Token.cpp Token.h
	g++ -c Token.cpp

TreeNode.o : TreeNode.cpp TreeNode.h
	g++ -c TreeNode.cpp

run :
	./p2

test :
	perl testDir/difftest.pl -1 "testDir/rpal -ast -noout FILE" -2 "./p1 -ast -noout FILE" -t testDir/tests/

cl :
	rm *.o p2
