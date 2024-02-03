//
// Created by griff on 4/4/2023.
//

#pragma once
#include <set>
#include <string>
#include <sstream>

class Node {
private:
    std::set<unsigned> adjacentNodeIDs;
public:
    bool visited{};
    unsigned id{};

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    std::set<unsigned> getAdjacentNodeIDs() const {
        return adjacentNodeIDs;
    }

    std::string toString() {
        std::stringstream ss;
        bool firstItem = true;
        for (auto& nodeID : adjacentNodeIDs) {
            if (firstItem) {
                firstItem = false;
            } else {
                ss << ",";
            }
            ss << 'R' << nodeID;
        }
        return ss.str();
    }

    explicit Node(unsigned id1) {
        visited = false;
        id = id1;
    }

    Node() = default;
};