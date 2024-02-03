//
// Created by griff on 3/6/2023.
//
#pragma once

#include <iostream>
#include <vector>


class Scheme {
private:
    std::vector<std::string> schemes;
public:
    Scheme(){}
    Scheme(std::vector<std::string> names){
        schemes = names;
    }

    const std::vector<std::string> &getSchemes() const {
        return schemes;
    }

    void setSchemes(const std::vector<std::string> &schemes);
    void addtoScheme(unsigned integer) {
        //return schemes.push_back(integer);
    }

};

void Scheme::setSchemes(const std::vector<std::string> &schemes) {
    Scheme::schemes = schemes;
}
