#include "../inc/Trie_suffix.hpp"

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

Trie_suffix::Node* Trie_suffix::find_prefix_suffix(size_t ind, bool& isEnd) {
  size_t curr_ind;
  Trie_suffix::Node* curr = child[text[ind]];
  std::vector<Node*>* prev_child = &child;
  isEnd = false;

  while (curr != nullptr) {
    for (curr_ind = 0; curr_ind < curr->len && curr_ind < text.length() - ind; ++curr_ind)
      if (text[curr->start + curr_ind] != text[ind+curr_ind]) {
        Trie_suffix::Node* tmp = new Trie_suffix::Node(ind, curr_ind);
        curr->start = curr->start + curr_ind;
        curr->len = curr->len - curr_ind;
        (*prev_child)[text[ind]] = tmp;
        tmp->child[text[curr->start]] = curr;
        tmp->child[text[ind+curr_ind]] = new Trie_suffix::Node(text.length()-1, 0);

        return tmp->child[text[ind+curr_ind]];
      }

    if (curr_ind == text.length()-ind)
      break;
    ind+=curr_ind;
    if (curr->child[text[ind]] == nullptr) {
      isEnd = true;
      break;
    }
    prev_child = &curr->child;
    curr = curr->child[text[ind]];
  }

  return curr;
}

bool Trie_suffix::substring_exist(const std::string& key) {
  size_t ind = 0, curr_ind;
  Trie_suffix::Node* curr = child[key[ind]];

  while (curr != nullptr) {
    for (curr_ind = 0; curr_ind < curr->len && curr_ind < key.length() - ind; ++curr_ind)
      if (text[curr->start + curr_ind] != key[ind+curr_ind])
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

void Trie_suffix::add_suffix(char sym) {
    Trie_suffix::Node *curr = max_len_ptr, *prev = nullptr;
    size_t curr_str_size;
    text.push_back(sym);

    curr_str_size = text.length();

    if (curr == nullptr) {
        child[sym] = new Trie_suffix::Node(0, 1);
        max_len_ptr = child[sym];
        return;
    }

    while (curr != nullptr) {
        ++curr->len;
        prev = curr;
        curr = curr->suffix_ptr;
        --curr_str_size;
        if (curr != nullptr)
          continue;
        bool full_string;
        curr = find_prefix_suffix(text.length()-curr_str_size, full_string);
        if (curr == nullptr) {//suffix is not found
          child[sym] = new Trie_suffix::Node(text.length()-1, 1);
          prev->suffix_ptr = child[sym];
          return;
        }
        if (curr->len != 0 && !full_string)//suffix is prefix
          return;
        //suffix node is fork/fullstring
    };
}

void Trie_suffix::print() {
  for (size_t i = 0; i < child.size()/2; ++i)
      print_node(child[i], 1);

  std::cout << '.' << std::endl;

  for (size_t i = child.size()/2; i < child.size(); ++i)
      print_node(child[i], 1);
}
