#pragma once

#include <string>
#include <vector>
#include <climits>
#include <iostream>
#include <fstream>

class Trie_suffix {
public:
    Trie_suffix();
    ~Trie_suffix();
    void make(const std::string& src);
    void add_suffix(char sym);
    void clear();
    bool substring_exist(const std::string& key);
    void print(std::ofstream& f_out);
    void print();
private:
    struct Node {
      Node (size_t _left, size_t *_right) : left(_left), right(_right) {
        child.resize(CHAR_MAX+1, nullptr);
        is_tmp = false;
      }
      ~Node () {
        for (Node* elem : child)
          delete elem;
        child.clear();
      }
      std::vector<struct Node*> child;
      size_t left, *right;
      bool is_tmp;
    };

    void print_node (Node* node, size_t deep, char sym, std::ofstream& f_out);
    void print_node (Node* node, size_t deep, char sym);

    Node* find_prefix_suffix(size_t ind, bool& is_fork);

    std::vector<Node*> child;
    std::string text;
    Node* last_passed;
    size_t last_pos_in_passed;
    size_t last_sym_pos, from_pos;

    Trie_suffix::Node* prev;
    std::vector<Node*>* prev_child;
};
