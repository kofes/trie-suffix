#pragma once

#include <string>
#include <vector>
#include <climits>

typedef unsigned long long value_type;

class Trie_suffix {
public:
    struct Node {
        Node (struct Node* next) : suffix_ptr(next), h(0), value(value_type()) {
            child.resize(CHAR_MAX);
        }
        std::vector<struct Node*> child;
        std::string key;
        value_type value;
        size_t h;
        struct Node* suffix_ptr;
    };
    Trie_suffix();
    void make(const std::string& src);
    void clear();
    value_type* find(const std::string& key);
    bool check_prefix_suffix(const std::string& key);
    ~Trie_suffix();
private:
    void delete_node(Node* node);
    value_type* find_node(Node* node, const std::string& key, size_t ind);
    Node* find_node_prefix_suffix( Node* node, const std::string& key, size_t ind);
    void add_suffix(char sym);

    std::vector<Node*> child;
    Node* max_len_str;
};
