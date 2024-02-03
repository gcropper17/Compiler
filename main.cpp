#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Interpreter.h"
#include "Node.h"
#include "Graph.h"

int main(int argc, char*argv[]) {
    std::string fileName = argv[1];
    std::ifstream inputStream(fileName);
    std::string input;
    std::stringstream ss;
    ss << inputStream.rdbuf();
    input = ss.str();
    if (!inputStream) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return 1;
    }
    std::vector<Token>myTokens;
    Scanner s = Scanner(input);
    Token t = s.scanToken();
    myTokens.push_back(t);
    while (t.getTokenType() != END_OF_FILE ){
        t = s.scanToken();
        myTokens.push_back(t);
    }
    inputStream.close();
    Parser p = Parser(myTokens);
    p.parse(myTokens);
    Interpreter i = Interpreter(p.getProgram());
    return 0;
}
