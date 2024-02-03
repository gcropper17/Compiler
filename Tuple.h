#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Scheme.h"

class Tuple : public std::vector<std::string> {

public:
    Tuple(){}
    Tuple(vector<std::string> values) : vector<std::string>(values) { }

    std::string getElement(int index) const {
        return this->at(index);
    }

    std::string toString(const Scheme& scheme) const {
        //const Tuple& tuple = *this;
        std::stringstream out;
        for(unsigned int i = 0; i < scheme.getSchemes().size(); i++) {
            out << " " << scheme.getSchemes().at(i) << "=" << this->at(i);
            //std::cout << "  " << scheme.getSchemes().at(i) << "=" << this->at(i) << std::endl;
            if (i < (scheme.getSchemes().size() - 1)) {
                out << ",";
            }
        }
        std::string string1 = out.str();
        return string1;
    }

};
