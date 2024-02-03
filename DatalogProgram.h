//
// Created by griff on 2/14/2023.
//
#pragma once
#include "Rule.h"
#include "Predicate.h"
#include "Parameter.h"
#include <vector>
#include <set>

class DatalogProgram {
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Predicate> queries;
    std::vector<Rule> rules;
    std::set<std::string> domain;
public:
    void setSchemes(std::vector<Predicate> scheme) {
        for (auto & i : scheme) {
            schemes.push_back(i);
        }
    }

    std::vector<Predicate> getSchemes() {
        return schemes;
    }

    void setFacts(const std::vector<Predicate>& fact) {
        for (auto & j : fact) {
            facts.push_back(j);
        }
    }

    std::vector<Predicate> getFacts() {
        return facts;
    }

    void setRules(std::vector <Rule> rule) {
        for (unsigned int i = rules.size(); i < rule.size(); i++) {
            rules.push_back(rule.at(i));
        }
    }

    std::vector<Rule> getRules() {
        return rules;
    }

    void setQueries(std::vector<Predicate> query) {
        for (unsigned int i = 0; i < query.size(); i++) {
            queries.push_back(query.at(i));
        }
    }

    std::vector<Predicate> getQueries() {
        return queries;
    }

    void setDomain (std::set<std::string> set) {
        domain = set;
    }
    void toString() {
        std::stringstream ss;
        ss << "Schemes(" << schemes.size() << "):" << std::endl;
        for (unsigned int i = 0; i < schemes.size(); i++) {
            ss <<  "  " << schemes.at(i).toString() << std::endl;
        }
        ss << "Facts(" << facts.size() << "):" << std::endl;
        for (unsigned int i = 0; i < facts.size(); i++) {
            ss << "  " << facts.at(i).toString() << "." << std::endl;
        }
        ss << "Rules(" << rules.size() << "):" << std::endl;
        for (unsigned int i = 0; i < rules.size(); i++) {
            ss << "  " << rules.at(i).toString() << "." << std::endl;
        }
        ss << "Queries(" << queries.size() << "):" << std::endl;
        for (unsigned int i = 0; i < queries.size(); i++) {
            ss << "  " << queries.at(i).toString() << "?" << std::endl;
        }
        ss << "Domain(" << domain.size() << "):" << std::endl;
        std::set<std::string>::iterator i;
        for (i = domain.begin(); i != domain.end(); i++) {
            ss << "  " << *i << "\n";
        }
        std::cout << ss.str();
    }
};
