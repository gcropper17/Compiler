#pragma once
#include <iostream>
#include <set>
#include <utility>

class Relation {
private:
    std::string name;
    Scheme scheme;
    std::set<Tuple> tuples;
public:
    Relation( std::string name,  Scheme scheme) : name(std::move(name)), scheme(std::move(scheme)) { }
    Relation()= default;
    void setName(std::string name1){
        name = std::move(name1);
    }
    void setScheme(Scheme scheme1){
        scheme = std::move(scheme1);
    }
    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    const std::set<Tuple> &getTuples() const {
        return tuples;
    }

    std::string toString(const Relation& newRelation) const {
        std::stringstream out;
        unsigned i = 0;
        for (auto& tuple : tuples) {
            i++;
            if (scheme.getSchemes().empty()) {}
            else {
                out << " " << tuple.toString(scheme);
                if (i != tuples.size()){
                    out  << std::endl;
                }
            }
        }
        std::string string2 = out.str();
        return string2;
    }

    Relation* select1(unsigned index, const std::string& value) const {
        auto* result = new Relation(name, scheme);
        for (const auto &tuple: tuples) {
            if (tuple.getElement(index) == value) {
                result->addTuple(tuple);
            }
        }
        return result;
    }

    Relation* select2(unsigned int index, unsigned int index2) const {
        auto* relation1 = new Relation(name, scheme);
        auto* relation2 = new Relation(name, scheme);
        relation1->setName(name);
        relation1->setScheme(scheme);
        relation2->setName(name);
        relation2->setScheme(scheme);
        for (const auto& i: tuples) {
            relation2->addTuple(i);
        }
        for (auto i: tuples) {
            if (i.at(index) == i.at(index2)) {
                relation1->addTuple(i);
            }
        }
        return relation1;
    }

    Relation* project(const std::vector<unsigned int>& indexes)  {
        auto* relation = new Relation(name, scheme);
        relation->setName(name);
        relation->setScheme(scheme);
        for (Tuple tuple: tuples) {
            std::vector<std::string> newTuple1;
            Tuple newTuple(newTuple1);
            for (unsigned int indexes1 : indexes) {
                newTuple.push_back(tuple.at(indexes1));
            }
            relation->addTuple(newTuple);
        }
        std::vector<std::string> newScheme;
        for (unsigned int index : indexes) {
            newScheme.push_back(scheme.getSchemes().at(index));
        }
        relation->setScheme(Scheme(newScheme));
        return relation;
    }

    Relation* rename(std::vector<std::string> newNames){
        auto* renameRelation = new Relation(name, scheme);
        Scheme schemes(std::move(newNames));
        renameRelation->setScheme(schemes);
        for (const auto& i : tuples) {
            renameRelation->addTuple(i);
        }
        return renameRelation;
    }

    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {
        bool canJoin = true;
        for (unsigned int i = 0; i < leftScheme.getSchemes().size(); i++) {
            for (unsigned int j = 0; j < rightScheme.getSchemes().size(); j++) {
                if((leftScheme.getSchemes().at(i) == rightScheme.getSchemes().at(j)) && (leftTuple.at(i) != rightTuple.at(j))){
                    canJoin = false;
                }
            }
        }
        return canJoin;
    }

    Relation* join(const Relation& right) {
        const Relation& left = *this;
        auto *result = new Relation;
        result->setScheme(left.getScheme());
        std::vector<unsigned> sameIndexes;
        unsigned i = 0;
        unsigned j;
        for (const std::string& leftTuple : left.scheme.getSchemes()) {
            j = 0;
            for (const std::string& rightTuple : right.scheme.getSchemes()) {
                bool match = false;
                if (leftTuple == rightTuple) {
                    match = true;
                }
                if (match) {
                    sameIndexes.push_back(j);
                }
                j++;
            }
            i++;
        }
        result->setScheme(joinSchemes(right));
        for(const Tuple& leftTuple: left.tuples) {
            for(const Tuple& rightTuple: right.tuples) {
                bool match = joinable(left.scheme, right.scheme,leftTuple,rightTuple);
                if(match) {
                    Tuple joinedTuple = joinTuples(leftTuple, rightTuple, sameIndexes);
                    result->tuples.insert(joinedTuple);
                }
            }
        }
        return result;
    }

    Scheme joinSchemes(const Relation& relation1) {
        std::vector<std::string> headers = scheme.getSchemes();
        for (const auto & j : relation1.getScheme().getSchemes()) {
            bool found = false;
            for (const auto & i : scheme.getSchemes()){
                if(i == j) {
                    found = true;
                }
            }
            if (!found){
                headers.push_back(j);
            }
        }
        return {headers};
    }

    static Tuple joinTuples(const Tuple& leftTuple, const Tuple& rightTuple, const std::vector<unsigned>& indexes) {
        Tuple newTuple;
        for (unsigned i = 0; i < leftTuple.size(); ++i) {
            newTuple.push_back(leftTuple.at(i));
        }

        for (unsigned j = 0; j < rightTuple.size(); ++j) {
            bool add = true;
            for (unsigned index : indexes) {
                if (index == j) {
                    add = false;
                    break;
                }
            }
            if (add) {
                newTuple.push_back(rightTuple.at(j));
            }
        }
        return newTuple;
    }

    void unionize(const Relation& relation) {
        unsigned sizeBefore = tuples.size();
        unsigned sizeAfter = tuples.size();
        for(const Tuple& newTuple : relation.getTuples()){
            sizeBefore = tuples.size();
            this->addTuple(newTuple);
            sizeAfter = tuples.size();
            if (sizeBefore != sizeAfter) {
                std::cout << " " << newTuple.toString(scheme.getSchemes()) << std::endl;
            }
        }
    }

    const Scheme &getScheme() const {
        return scheme;
    }
};