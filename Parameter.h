//
// Created by griff on 2/14/2023.
//

#ifndef LAB1_PARAMETER_H
#define LAB1_PARAMETER_H
class Parameter {
public:
    void SetParam(std::string Toke){
        myParameter = Toke;
    }

    std::string toString() const {
        return myParameter;
    }

    const std::string &getMyParameter() const {
        return myParameter;
    }

private:
    std::string myParameter;
};
#endif //LAB1_PARAMETER_H
