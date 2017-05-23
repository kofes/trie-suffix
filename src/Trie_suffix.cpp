#include "../inc/Trie_suffix.hpp"
// //DEBUG
// #include <iostream>
// //
Trie_suffix::Trie_suffix() {
    child.resize(CHAR_MAX+1, nullptr);
    text.clear();
    max_len_ptr = nullptr;
}

Trie_suffix::~Trie_suffix() {
    clear();
    child.clear();
}

void Trie_suffix::clear() {
  for (size_t i = 0; i < child.size(); ++i) {
    delete child[i];
    child[i] = nullptr;
  }
  text.clear();
  max_len_ptr = nullptr;
}

Trie_suffix::Node* Trie_suffix::find_prefix_suffix(const std::string& key, size_t ind, bool& isEnd) {
  size_t curr_ind;
  Trie_suffix::Node* curr = child[key[ind]];
  std::vector<Node*>* prev_child = &child;
  isEnd = false;
  while (curr != nullptr) {
    for (curr_ind = 0; curr_ind < curr->key.length() && curr_ind < key.length() - ind; ++curr_ind)
      if (curr->key[curr_ind] != key[ind+curr_ind]) {
        Trie_suffix::Node* tmp = new Trie_suffix::Node("");
        // //DEBUG
        // std::cout << "KEY: " << key << std::endl;
        // std::cout << "SUFF: ";
        // for (size_t i = ind; i < key.length(); ++i)
        //   std::cout << key[i];
        // std::cout << std::endl;
        // //
        for (size_t i = 0; i < curr_ind; ++i)
          tmp->key.push_back(key[ind+i]);
        // //DEBUG
        // std::cout << "TMP-KEY: " << tmp->key << std::endl;
        // std::cout << "CURR-IND: " << curr_ind << std::endl;
        // std::cout << "CURR-KEY-LEN: " << curr->key.length() << std::endl;
        // std::cout << "CURR-KEY-BEFORE: " << curr->key << std::endl;
        // //
        for (long long i = 0; i < curr->key.length()-curr_ind; ++i) {
          curr->key[i] = curr->key[i+curr_ind];
        }
        curr->key.resize(curr->key.length()-curr_ind);
        // //DEBUG
        // std::cout << "CURR-KEY-AFTER: " << curr->key << std::endl;
        // //
        (*prev_child)[key[ind]] = tmp;
        tmp->child[curr->key[0]] = curr;
        tmp->child[key[ind+curr_ind]] = new Trie_suffix::Node("");

        return tmp->child[key[ind+curr_ind]];
      }

    if (curr_ind == key.length()-ind)
      break;
    ind+=curr_ind;
    if (curr->child[key[ind]] == nullptr) {
      isEnd = true;
      break;
    }
    prev_child = &curr->child;
    curr = curr->child[key[ind]];
  }

  return curr;
}

bool Trie_suffix::substring_exist(const std::string& key) {
  size_t ind = 0, curr_ind;
  Trie_suffix::Node* curr = child[key[ind]];

  while (curr != nullptr) {
    for (curr_ind = 0; curr_ind < curr->key.length() && curr_ind < key.length() - ind; ++curr_ind)
      if (curr->key[curr_ind] != key[ind+curr_ind])
        return false;

    if (curr_ind == key.length()-ind)
      return true;

    ind+=curr_ind;
    curr = curr->child[key[ind]];
  }

  return false;
}

void Trie_suffix::make(const std::string& src) {
    clear();
    for (size_t i = 0; i < src.length(); ++i)
        add_suffix(src[i]);
}
//TODO: complete add_suffix:
void Trie_suffix::add_suffix(char sym) {
    Trie_suffix::Node *curr = max_len_ptr, *prev = nullptr;
    size_t curr_str_size;
    text.push_back(sym);
    // //DEBUG
    // std::cout << "TEXT: " << text << std::endl;
    // //
    curr_str_size = text.length();

    if (curr == nullptr) {
        child[sym] = new Trie_suffix::Node(std::string() + sym);
        max_len_ptr = child[sym];
        return;
    }
    // //DEBUG
    // std::cout << "MAX_LEN_PTR_KEY: " << max_len_ptr->key << std::endl;
    // //
    while (curr != nullptr) {
        curr->key.push_back(sym);
        prev = curr;
        curr = curr->suffix_ptr;
        --curr_str_size;
        if (curr != nullptr)
          continue;
        bool full_string;
        curr = find_prefix_suffix(text, text.length()-curr_str_size, full_string);
        if (curr == nullptr) {//suffix is not found
          child[sym] = new Trie_suffix::Node(std::string() + sym);
          prev->suffix_ptr = child[sym];
          return;
        }
        if (curr->key.length() != 0 && !full_string)//suffix is prefix
          return;
        //suffix node is fork/fullstring
    };
}

void Trie_suffix::print() {
  for (size_t i = 0; i < child.size()/2; ++i) {
    if (child[i] != nullptr)
      child[i]->print(1);
  }
  std::cout << '.' << std::endl;
  for (size_t i = child.size()/2; i < child.size(); ++i) {
    if (child[i] != nullptr)
      child[i]->print(1);
  }
}
