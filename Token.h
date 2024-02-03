//
// Created by griff on 1/19/2023.
//
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <vector>

#ifndef LAB1_TOKEN_H
#define LAB1_TOKEN_H

enum TokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, END_OF_FILE, DASH};

class Token {
private:
    TokenType type;
    std::string value;
    unsigned int lineNumber;

    std::string getType(TokenType token) {
        switch (token) {
            case COMMA:
                return "COMMA";
            case PERIOD:
                return "PERIOD";
            case Q_MARK:
                return "Q_MARK";
            case LEFT_PAREN:
                return "LEFT_PAREN";
            case RIGHT_PAREN:
                return "RIGHT_PAREN";
            case COLON:
                return "COLON";
            case COLON_DASH:
                return "COLON_DASH";
            case MULTIPLY:
                return "MULTIPLY";
            case ADD:
                return "ADD";
            case SCHEMES:
                return "SCHEMES";
            case FACTS:
                return "FACTS";
            case RULES:
                return "RULES";
            case QUERIES:
                return "QUERIES";
            case ID:
                return "ID";
            case STRING:
                return "STRING";
            case COMMENT:
                return "COMMENT";
            case UNDEFINED:
                return "UNDEFINED";
            case END_OF_FILE:
                return "EOF";
            case DASH:
                return "RETURN";
            default:
                return "";
        }
    }

public:
    Token(TokenType type, std::string value, unsigned int lineNumber){
        this->type = type;
        this->value = std::move(value);
        this->lineNumber = lineNumber;
    }
    ~Token()= default;
    std::string toString(){
        std::stringstream output;
        output << "(" << getType(type) << "," << "\"" << value << "\"" << "," << lineNumber << ")";
        return output.str();
    }
    TokenType getTokenType() const {
        return type;
    }
    std::string getTokenValue() const {
        return value;
    }
};
#endif //LAB1_TOKEN_H
