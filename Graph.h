//
// Created by griff on 4/4/2023.
//
#pragma once
#include <stack>
#include "Node.h"

class Graph {
private:
    std::map<unsigned,Node> nodes;
    std::stack<unsigned> postOrderStack;
    std::vector<std::set<unsigned>> scc;
    std::set<unsigned> temp;
public:
    Graph() = default;

    const std::vector<std::set<unsigned int>> &getScc() const {
        return scc;
    }

    explicit Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node(nodeID);
    }

    void addEdge(unsigned fromNodeID, unsigned toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    std::string toString(){
        std::stringstream ss;
        for (auto& pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            ss << 'R' << nodeID << ':' << node.toString() << std::endl;
        }
        return ss.str();
    }

    const std::map<unsigned, Node> &getNodes() const {
        return nodes;
    }

    void dfs(Node& node, unsigned id) {
        if (node.visited) {
            return;
        }
        node.visited = true;
        for (unsigned adjID : node.getAdjacentNodeIDs()) {
            dfs(nodes[adjID], adjID);
        }
        postOrderStack.push(id);
    }

    void dfsForest() {
        for (auto& node : nodes) {
            if (!node.second.visited) {
                dfs(node.second, node.first);
            }
        }
    }

    void dfsForward(Node& node) {
        if (node.visited) {
            return;
        }
        node.visited = true;
        for (unsigned adjID : node.getAdjacentNodeIDs()) {
            dfsForward(nodes[adjID]);
        }
    }

    void dfsForestForward(std::stack<unsigned>& postOrderStack1) {
        while (!postOrderStack1.empty()) {
            unsigned node_id = postOrderStack1.top();
            postOrderStack1.pop();
            if (nodes[node_id].visited) {
                if (!temp.empty()) {}
            } else {
                strongCC(nodes[node_id]);
                scc.push_back(temp);
                temp.clear();
            }
        }
    }

    void strongCC(Node &node) {
        if (node.visited) {
            temp.insert(node.id);
            return;
        }
        node.visited = true;
        temp.insert(node.id);
        for (auto i: node.getAdjacentNodeIDs()) {
            if (!nodes[i].visited) {
                strongCC(nodes[i]);
            }
        }
    }

    const std::stack<unsigned int> &getPostOrderStack() const {
        return postOrderStack;
    }
    bool hasEdge(unsigned src, unsigned dest) {
        if (nodes.find(src) == nodes.end()) {
            return false;
        }
        const std::set<unsigned>& adjNodes = nodes[src].getAdjacentNodeIDs();
        return adjNodes.find(dest) != adjNodes.end();
    }

};