#pragma once

#include <string>
#include <vector>
#include <climits>
#include <iostream>

class Trie_suffix {
public:
    Trie_suffix();
    ~Trie_suffix();
    void make(const std::string& src);
    void add_suffix(char sym);
    void clear();
    bool substring_exist(const std::string& key);
    void print();
private:
    struct Node {
      Node (size_t _start, size_t _len, struct Node* next = nullptr) : start(_start), len(_len), suffix_ptr(next) {
        child.resize(CHAR_MAX+1, nullptr);
      }
      ~Node () {
        for (Node* elem : child)
          delete elem;
        child.clear();
        suffix_ptr = nullptr;
      }
      std::vector<struct Node*> child;
      size_t start, len;
      struct Node* suffix_ptr;
    };

    void print_node (Node* node, size_t deep) {
      if (node == nullptr) return;

      for (size_t i = 0; i < node->child.size()/2; ++i)
        print_node(node->child[i], deep+1);

      std::cout << std::string(deep*3, ' ');
      for (size_t i = 0; i < node->len; ++i)
        std::cout << text[node->start + i];
      std::cout << std::endl;

      for (size_t i = node->child.size()/2; i < child.size(); ++i)
        print_node(node->child[i], deep+1);
    }

    Node* find_prefix_suffix(size_t ind, bool& isEnd);

    std::vector<Node*> child;
    std::string text;
    Node* max_len_ptr;
};
