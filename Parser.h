//
// Created by griff on 2/2/2023.
//
#ifndef LAB1_PARSER_H
#define LAB1_PARSER_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"
#include "Rule.h"
#include "Predicate.h"
#include "Parameter.h"

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Predicate> predicates;
    std::vector<Rule> rulePredicates;
    DatalogProgram datalogProgram1;
    Parameter param;
    Predicate p;
    Rule r;
    std::string domain;
    std::set<std::string> domains;
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) { }
    void parse(std::vector<Token> token){
        try {
            token = tokens;
            datalogProgram();
        }
        catch (Token errorToken) {
            std::cout << "Failure!" << std::endl;
            std::cout << "  " << errorToken.toString() << std::endl;
        }
    }

    void datalogProgram () {
        removeComments();
        match(SCHEMES);
        match(COLON);
        scheme();
        schemeList();
        match(FACTS);
        match(COLON);
        factList();
        match(RULES);
        match(COLON);
        ruleList();
        match(QUERIES);
        match(COLON);
        query();
        queryList();
        match(END_OF_FILE);
    }

    DatalogProgram getProgram() {
        return datalogProgram1;
    }

    TokenType tokenType() const {
        return tokens.at(0).getTokenType();
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }

    void match(TokenType t) {
        if (t == tokens.at(0).getTokenType()){
            advanceToken();
        } else {
            throw tokens.at(0);
        }
    }

    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            param.SetParam(tokens.at(0).getTokenValue());
            p.pushParameter(param);
            match(ID);
            idList();
        }
        else if(tokenType() == RIGHT_PAREN){}
        else {throw tokens.at(0);}
    }

    void queryList(){
        if (tokenType() == ID) {
            query();
            queryList();
        }
        else if (tokenType() == END_OF_FILE){}
        else {throw tokens.at(0);}
    }

    void query() {
        predicate();
        match(Q_MARK);
        predicates.push_back(p);
        p.empty();
        datalogProgram1.setQueries(predicates);
        predicates.clear();
    }

    void scheme() {
        p.setName(tokens.at(0).getTokenValue());
        match(ID);
        match(LEFT_PAREN);
        param.SetParam(tokens.at(0).getTokenValue());
        match(ID);
        p.pushParameter(param);
        idList();
        match(RIGHT_PAREN);
        predicates.push_back(p);
        p.empty();
        datalogProgram1.setSchemes(predicates);
        predicates.clear();
    }

    void schemeList() {
        if (tokenType() == ID) {
            scheme();
            schemeList();
        }
        else if(tokenType() == FACTS){}
        else {throw tokens.at(0);}
    }

    void stringList(){
        if (tokenType() == COMMA) {
            match(COMMA);
            param.SetParam(tokens.at(0).getTokenValue());
            p.pushParameter(param);
            domain = tokens.at(0).getTokenValue();
            domains.insert(domain);
            datalogProgram1.setDomain(domains);
            match(STRING);
            stringList();
        }
        else if (tokenType() == RIGHT_PAREN){}
        else {throw tokens.at(0);}
    }

    void fact(){
        p.setName(tokens.at(0).getTokenValue());
        match(ID);
        match(LEFT_PAREN);
        param.SetParam(tokens.at(0).getTokenValue());
        domain = tokens.at(0).getTokenValue();
        domains.insert(domain);
        datalogProgram1.setDomain(domains);
        match(STRING);
        p.pushParameter(param);
        stringList();
        match(RIGHT_PAREN);
        match(PERIOD);
        predicates.push_back(p);
        p.empty();
        datalogProgram1.setFacts(predicates);
        predicates.clear();
    }

    void factList() {
        if (tokenType() == ID) {
            fact();
            factList();
        }
        else if (tokenType() == RULES){}
        else {throw tokens.at(0);}
    }

    void ruleList(){
        if(tokenType() == ID){
            rule();
            ruleList();
        }
        else if (tokenType() == QUERIES){}
        else {tokens.at(0);}
    }

    void rule() {
        headPredicate();
        r.setHead(p);
        match(COLON_DASH);
        predicate();
        predicates.push_back(p);
        predicateList();
        match(PERIOD);
        r.setRule(predicates);
        rulePredicates.push_back(r);
        datalogProgram1.setRules(rulePredicates);
        p.empty();
        r.empty();
        predicates.clear();

    }

    void headPredicate(){
        p.setName(tokens.at(0).getTokenValue());
        match(ID);
        match(LEFT_PAREN);
        param.SetParam(tokens.at(0).getTokenValue());
        p.pushParameter(param);
        match(ID);


        idList();
        match(RIGHT_PAREN);
    }

    void predicate(){
        p.empty();
        p.setName(tokens.at(0).getTokenValue());
        match(ID);
        match(LEFT_PAREN);
        parameter();
        parameterList();
        match(RIGHT_PAREN);
    }
    void predicateList(){
        if(tokenType() == COMMA) {
            //predicates.push_back(p);
            match(COMMA);
            predicate();
            predicates.push_back(p);
            predicateList();
        }
        else if (tokenType() == PERIOD){}
        else {throw tokens.at(0);}
    }

    void parameter() {
        if (tokenType() == STRING) {
            param.SetParam(tokens.at(0).getTokenValue());
            match(STRING);
            p.pushParameter(param);
        }
        else if (tokenType() == ID) {
            param.SetParam(tokens.at(0).getTokenValue());
            match(ID);
            p.pushParameter(param);

        }
        else { throw tokens.at(0); }
    }

    void parameterList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameter();
            parameterList();
        }
        else if (tokenType() == RIGHT_PAREN){}
        else {throw tokens.at(0);}
    }

    void removeComments() {
        for (unsigned int i = 0; i < tokens.size(); ++i) {
            if (tokens.at(i).getTokenType() == TokenType::COMMENT) {
                tokens.erase(tokens.begin() + i);
                i--;
            }
        }
    }
};
#endif //LAB1_PARSER_H
