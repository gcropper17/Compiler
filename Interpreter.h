//
// Created by griff on 3/14/2023.
//
#pragma once
#include "DatalogProgram.h"
#include "Database.h"
#include "Rule.h"
#include "Graph.h"
#include <algorithm>
#include <utility>

class Interpreter {
private:
    DatalogProgram program;
    Database database;
public:
    explicit Interpreter(DatalogProgram program) {
        this->program = std::move(program);
        InterpretSchemes();
        InterpretFacts();
        std::cout << "Dependency Graph" << std::endl;
        interpretRules();
        std::cout << std::endl << "Query Evaluation" << std::endl;
        interpretQueries();
    }

    void InterpretSchemes() {
        for (Predicate scheme : program.getSchemes()) {
            std::string name = scheme.getId();
            std::vector<std::string> schemeVector;
            for (const Parameter& p : scheme.getParameters()) {
                schemeVector.push_back(p.toString());
            }
            Scheme newScheme(schemeVector);
            Relation newRelation(name, newScheme);
            database.addToDatabase(name, newRelation);
        }
    }

    void InterpretFacts() {
        for (Predicate facts : program.getFacts()){
            Relation* r = database.GetRelation(facts.getId());
            std::string name;
            Tuple tuple;
            std::vector<std::string> factVector;
            for (unsigned int i = 0; i < facts.getParameters().size();i++){
                factVector.push_back(facts.getParameters().at(i).toString());
            }
            r->addTuple(factVector);
        }
    }

    Relation* InterpretQuery(Predicate query) {
        std::map<std::string, unsigned int> duplicateMap;
        Relation* r = database.GetRelation(query.getId());
        Relation* newRelation = r;
        std::vector<Parameter> queryParams = query.getParameters();
        std::vector<unsigned int> positions;
        std::vector<std::string> seen;
        for (unsigned int i = 0; i < queryParams.size(); i++) {
            const std::string param = queryParams.at(i).getMyParameter();
            if(param.at(0) == '\'') {
                newRelation = newRelation->select1(i, param);
            } else {
                auto duplicate = duplicateMap.find(param);
                if(duplicateMap.find(param) == duplicateMap.end()) {
                    positions.push_back(i);
                    seen.push_back(param);
                    duplicateMap.insert({param, i});
                } else {
                    newRelation = newRelation->select2(duplicate->second, i);
                }
            }
        }
        newRelation = newRelation->project(positions);
        newRelation = newRelation->rename(seen);
        return newRelation;
    }

    void interpretQueries(){
        for (Predicate query : program.getQueries()) {
            std::cout  << query.toString() << "? ";
            Relation*  newRelation = InterpretQuery(query);
            if (newRelation->getTuples().empty()) {
                std::cout << "No" << std::endl;
            }
            else {
                std::cout << "Yes(" << newRelation->getTuples().size() <<")" << std::endl;
                std::string newRelationString = newRelation->toString(*newRelation);
                std::cout << newRelationString << std::endl;
            }
        }
    }

    void interpretRules() {
        std::string string1;
        Graph forward = makeGraph(program.getRules());
        Graph reverse = reverseGraph(forward);
        reverse.dfsForest();
        std::stack<unsigned> stack;
        stack = reverse.getPostOrderStack();
        forward.dfsForestForward(stack);
        std::cout << forward.toString();
        std::cout << std::endl << "Rule Evaluation" << std::endl;
        std::vector<std::set<unsigned>> sccs;
        sccs = forward.getScc();
        for (auto& rule : sccs) {
            std::vector<Rule> rules = program.getRules();
            if (rule.size() == 1 && !forward.hasEdge(*rule.begin(), *rule.begin())) {
                std::cout << "SCC: R" << *rule.begin() << std::endl;
                std::cout << rules.at(*rule.begin()).toString() << '.' << std::endl;
                interpret1Rule(rules.at(*rule.begin()));
                std::cout << "1 passes: R" << *rule.begin() << std::endl;
            } else {
                std::cout << "SCC: ";
                for (auto it = rule.begin(); it != rule.end(); ++it) {
                    std::cout << "R" << *it;
                    if (std::next(it) != rule.end()) {
                        std::cout << ",";
                    }
                }
                std::cout << std::endl;
                std::vector<unsigned> ruleIDs(rule.begin(), rule.end());
                std::sort(ruleIDs.begin(), ruleIDs.end());
                unsigned passes = 0;
                bool converged = false;
                while (!converged) {
                    passes++;
                    unsigned int tempDatabase = database.numberOfTuples();
                    for (auto elem : ruleIDs) {
                        std::cout << rules.at(elem).toString() << '.' << std::endl;
                        interpret1Rule(rules.at(elem));
                    }
                    if (database.numberOfTuples() == tempDatabase) {
                        converged = true;
                    } else {
                        converged = false;
                    }
                }
                std::cout << passes << " passes: ";
                for (auto it = ruleIDs.begin(); it != ruleIDs.end(); ++it) {
                    std::cout << "R" << *it;
                    if (it != ruleIDs.end() - 1) {
                        std::cout << ",";
                    }
                }
                std::cout << std::endl;
            }
        }
    }

    Relation* interpret1Rule(const Rule& rule){
        Relation* result;
        const std::vector<Predicate>& ruleBody = rule.getBody();
        std::vector<std::string> seen;
        unsigned int i = 0;
        result = InterpretQuery(rule.getBody().at(0));
        for (Predicate query : rule.getBody()) {
            if (i == 0) {}
            else {
                result = result->join(*InterpretQuery(rule.getBody().at(i)));
            }
            i++;
        }
        std::vector<unsigned> positions1;
        for (unsigned k = 0; k < rule.getHead().getParameters().size(); k++) {
            for (unsigned j = 0; j < result->getScheme().getSchemes().size(); j++) {
                if (rule.getHead().getParameters().at(k).toString() == result->getScheme().getSchemes().at(j)) {
                    positions1.push_back(j);
                }
            }
        }
        result  = result->project(positions1);
        database.GetRelation(rule.getHead().getId())->unionize(*result);
        return result;
    }

    static Graph makeGraph(const std::vector<Rule>& rules) {
        Graph graph(rules.size());
        for (unsigned fromID = 0; fromID < rules.size(); fromID++) {
            Rule fromRule = rules.at(fromID);
            for (unsigned pred = 0; pred < fromRule.size(); pred++) {
                Predicate bodyPred = fromRule.getBodyPredicate(pred);
                for (unsigned toID = 0; toID < rules.size(); toID++) {
                    Rule toRule = rules.at(toID);
                    if(bodyPred.getId() == toRule.getHead().getId()){
                        graph.addEdge(fromID, toID);
                    }
                }
            }
        }
        return graph;
    }

    static Graph reverseGraph(const Graph& graph) {
        Graph reversedGraph(graph.getNodes().size());
        for (unsigned i = 0; i < graph.getNodes().size(); i++) {
            for (unsigned j : graph.getNodes().at(i).getAdjacentNodeIDs()) {
                reversedGraph.addEdge(j, i);
            }
        }
        return reversedGraph;
    }
};
