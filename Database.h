//
// Created by griff on 3/13/2023.
//
#pragma once
#include <iostream>
#include <map>
#include "Relation.h"

class Database {
private:
public:
    std::map <std::string, Relation> Database1;
    void addToDatabase(std::string name, Relation relation) {
        Database1.insert({name, relation});
    }

    Relation* GetRelation(std::string relationName) {
        return &Database1.at(relationName);
    }

    unsigned int numberOfTuples() {
        int size = 0;
        for (auto i: Database1) {
            size += i.second.getTuples().size();
        }
        return size;
    }

    const std::map<std::string, Relation> &getDatabase1() const {
        return Database1;
    }
};