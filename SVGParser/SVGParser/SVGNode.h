#pragma once
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class SVGNode {
private:
    string tag;
    vector<SVGNode*> children;
    unordered_map<string, string> attributes;

public:
    SVGNode(const string& name) : tag(name) {}

    void addChild(SVGNode* node) { children.push_back(node); }
    const vector<SVGNode*>& getChildren() const { return children; }
    const string& getTag() const { return tag; }

    void setAttribute(const string& key, const string& value) {
        attributes[key] = value;
    }

    const unordered_map<string, string>& getAttributes() const {
        return attributes;
    }
};


