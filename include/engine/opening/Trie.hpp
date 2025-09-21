#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace engine::opening {

struct Node {
    std::unordered_map<std::string, Node *> children;
};

class Trie {
  public:
    Trie() : _root(new Node()) {
    }

    void add(std::vector<std::string> &strings) {
        Node *node = this->_root;

        for (std::string &string : strings) {
            if (node->children.find(string) == node->children.end()) {
                node->children[string] = new Node();
            }

            node = node->children[string];
        }
    }

    std::vector<std::string> getChildren(std::vector<std::string> &strings) {
        Node *node = this->_root;

        for (std::string &string : strings) {
            if (node->children.find(string) == node->children.end()) {
                return {};
            }

            node = node->children[string];
        }

        std::vector<std::string> children;

        for (const auto &[key, value] : node->children) {
            children.push_back(key);
        }

        return children;
    }

  private:
    Node *_root;
};

} // namespace engine::opening
