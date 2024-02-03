//
// Created by griff on 2/14/2023.
//
#ifndef LAB1_PREDICATE_H
#define LAB1_PREDICATE_H
#include "Parameter.h"
class Predicate {
private:
    std::string id;
    std::vector<Parameter> parameters;


public:
    Predicate(std::string id): id(id), parameters(std::vector<Parameter>()){}
    Predicate(){

    }
    void setName(std::string theID) {
        id = theID;
    }

    const std::string &getId() const {
        return id;
    }

    const std::vector<Parameter> getParameters()  {
        return parameters;
    }

    void pushParameter(Parameter parameter) {
        parameters.push_back(parameter);
    }

    void empty() {
        parameters.clear();
    }
    std::string toString() {
        std::string predString;
        //predString = id + "(";
        for (unsigned int i = 0; i < parameters.size(); i++) {
            if (i == 0){
                predString = id + "(" + predString + parameters.at(i).toString();
            } else {
                predString = predString + "," + parameters.at(i).toString();
            }
        }
        predString = predString + ")";
        return predString;
    }
};
#endif //LAB1_PREDICATE_H
