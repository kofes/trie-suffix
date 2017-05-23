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
      Node (const std::string& src, struct Node* next = nullptr) : key(src), suffix_ptr(next) {
        child.resize(CHAR_MAX+1, nullptr);
      }
      ~Node () {
        for (Node* elem : child)
          delete elem;
        child.clear();
        key.clear();
        suffix_ptr = nullptr;
      }
      void print (size_t deep) {
        for (size_t i = 0; i < child.size()/2; ++i) {
          if (child[i] != nullptr)
            child[i]->print(deep+1);
        }
        std::cout << std::string(deep*3, ' ') << key << std::endl;
        for (size_t i = child.size()/2; i < child.size(); ++i) {
          if (child[i] != nullptr)
            child[i]->print(deep+1);
        }
      }
      std::vector<struct Node*> child;
      std::string key;
      struct Node* suffix_ptr;
    };

    Node* find_prefix_suffix(const std::string& key, size_t ind, bool& isEnd);

    std::vector<Node*> child;
    std::string text;
    Node* max_len_ptr;
};
