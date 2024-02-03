//
// Created by griff on 2/14/2023.
//

#ifndef LAB1_RULE_H
#define LAB1_RULE_H
#include "Predicate.h"

class Rule {
private:
    Predicate head;
    std::vector<Predicate>body;

public:
    Rule(Predicate head ): head(head), body(std::vector<Predicate>()){}
    Rule(){}
    void setHead (Predicate header) {
        head = header;
    }
    void empty(){
        body.clear();
    }
    void setRule(std::vector<Predicate> rules) {
        for (unsigned int i = 0; i < rules.size(); i++) {
            body.push_back(rules.at(i));
        }
    }
    std::string toString(){
        std::string myRule;

        myRule = head.toString() + " :- ";
        for (unsigned int i = 0; i < body.size(); i++) {
            if (i == 0) {
                myRule = myRule + body.at(i).toString();

            } else {
                myRule = myRule + "," + body.at(i).toString();
            }
        }
        myRule = myRule;
        return myRule;
    }

    Predicate getHead() const {
        return head;
    }

    const std::vector<Predicate>& getBody() const {
        return body;
    }

    Predicate getBodyPredicate(unsigned int i) {
        return body.at(i);
    }
    void addBodyPredicate(Predicate predicate) {
        body.push_back(predicate);
    }
    unsigned int size(){
        return body.size();
    }

};


#endif //LAB1_RULE_H
