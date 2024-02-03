//
// Created by griff on 1/19/2023.
//
#include "Token.h"
#include "vector"
#ifndef LAB1_SCANNER_H
#define LAB1_SCANNER_H

class Scanner{
private:
    std::string input;
    TokenType type;
    std::string value;
    unsigned int lineNumber = 1;
    unsigned int endOfCounter = 0;
public:
    Scanner(const std::string& input) : input(input){}
    /*--------------------String--------------------*/
    bool checkForString() {
        std::string str;
        str = input[0];
        if (input[0] != '\'') {
            return false;
        }
        input = input.substr(1);
        while (input[0] != '\'') {
            if (input[0] == '\n'){
                endOfCounter++;
            }
            str += input[0];
            input = input.substr(1);
            if (input.empty()) {
                type = UNDEFINED;
                value = str;
                return false;
            }
        }
        str += input[0];
        input = input.substr(1);
        type = STRING;
        value = str;
        return true;
    }

    bool checkForComment (){
        std::string hashtag;
        std::string pipe;
        std::string comment;
        hashtag = input[0];
        pipe = input[1];
        /*--------------------Multi-Line Comment--------------------*/
        if ((hashtag[0] == '#') && (pipe[0] == '|')){
            input = input.substr(2);
            while ((input[0] != '|') && (input[1] != '#')) {
                if (input [0] == '\n'){
                    lineNumber++;
                }
                comment += input[0];
                input = input.substr(1);
                if (input.empty()){
                    type = UNDEFINED;
                    value = comment;
                    return false;
                }
            }
            type = COMMENT;
            value = comment;
            lineNumber = lineNumber + endOfCounter;
            input = input.substr(2);
            return true;
        }
        /*--------------------Single Line Comment--------------------*/
        if (input[0] == '#'){
            comment += input[0];
            input = input.substr(1);
            if (input.empty()) {
                type = COMMENT;
                value = comment;
                return true;
            }
            while (input[0] != '\n') {
                comment += input[0];
                input = input.substr(1);
                if (input.empty()){
                    type = COMMENT;
                    value = comment;
                    return true;
                }
            }
            type = COMMENT;
            value = comment;
            return true;
        }
        return false;
    }

    Token scanToken() {
        if (!input.empty())
            while (isspace(input.at(0))) {
                if (input.at(0) == '\n') {
                    lineNumber++;
                }
                input = input.substr(1);
                if (input.empty()){
                    type = END_OF_FILE;
                    value = "";
                    return {type, value, endOfCounter + lineNumber};
                }
            }
        if(input[0]== '\n') {
            std::cout << "hello";
        }
        if(input.empty()) {
            type = END_OF_FILE;
            value = "";
            return {type, value, endOfCounter + lineNumber};
        }

        switch (input.at(0)){
            case ',':
                type = COMMA;
                value = ",";
                input = input.substr(1);
                break;
            case '.':
                type = PERIOD;
                value = ".";
                input = input.substr(1);
                //newInput = input;
                break;
            case '?':
                type = Q_MARK;
                value = "?";
                input = input.substr(1);
                break;
            case '(':
                type = LEFT_PAREN;
                value = "(";
                input = input.substr(1);
                break;
            case ')':
                type = RIGHT_PAREN;
                value = ")";
                input = input.substr(1);
                break;
            case ':':
                char next;
                if (input.length() > 1) {
                    next = input.substr(1, 1)[0];
                    if (next == '-'){
                        type = COLON_DASH;
                        value = ":-";
                        input = input.substr(2);
                        break;
                    }
                }
                type = COLON;
                value = ":";
                input = input.substr(1);
                break;
            case '*':
                type = MULTIPLY;
                value = "*";
                input = input.substr(1);
                break;
            case '+':
                type = ADD;
                value = "+";
                input = input.substr(1);
                break;
            case '#':
                if (checkForComment()) {
                    break;
                } else {
                    break;
                }
            case '\'':
                if(checkForString()){
                    break;
                } else {
                    break;
                }
            default:
                /*--------------------IDs, Schemes, Rules, Facts, Queries--------------------*/
                if(isalpha(input.at(0))){
                    std::string id;
                    while (!input.empty() && !std::isspace(input[0])) {
                        id += input[0];
                        input = input.substr(1);
                        if(input[0] == ','){
                            type = COMMA;
                            value = ',';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == '.'){
                            type = PERIOD;
                            value = '.';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == '?'){
                            type = Q_MARK;
                            value = '?';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == '('){
                            type = LEFT_PAREN;
                            value = '(';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == ')'){
                            type = RIGHT_PAREN;
                            value = ')';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == ':'){
                            type = COLON;
                            value = ':';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == ':' && input[1] == '-'){
                            type = COLON_DASH;
                            value = ":-";
                            input.substr(2);
                            break;
                        }
                        if(input[0] == '*'){
                            type = MULTIPLY;
                            value = '*';
                            input.substr(1);
                            break;
                        }
                        if(input[0] == '+'){
                            type = ADD;
                            value = '+';
                            input.substr(1);
                            break;
                        }
                    }
                    if (id == "Schemes") {
                        type = SCHEMES;
                        value = id;
                        break;
                    } else if (id == "Rules"){
                        type = RULES;
                        value = id;
                        break;
                    } else if (id == "Queries"){
                        type = QUERIES;
                        value = id;
                        break;
                    } else if (id == "Facts"){
                        type = FACTS;
                        value = id;
                        break;
                    } else {
                        type = ID;
                        value = id;
                        break;
                    }
                }
                type = UNDEFINED;
                value = input[0];
                input = input.substr(1);
                return {type, value, lineNumber};
        }
        return {type, value, lineNumber};
    }
};
#endif //LAB1_SCANNER_H
