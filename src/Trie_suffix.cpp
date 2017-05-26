#include "../inc/Trie_suffix.hpp"

Trie_suffix::Trie_suffix() {
    child.resize(CHAR_MAX+1, nullptr);
    text.clear();
    last_sym_pos = 0;
    from_pos = 0;
    last_passed = nullptr;
    last_pos_in_passed = 0;
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
  last_sym_pos = 0;
  from_pos = 0;
  last_passed = nullptr;
  last_pos_in_passed = 0;
}

Trie_suffix::Node* Trie_suffix::find_prefix_suffix(size_t ind, bool& is_fork) {
  size_t curr_ind = 0;
  Trie_suffix::Node* curr = child[text[ind]];
  //Optimization
  if (last_passed != nullptr) {
    curr = last_passed;
    curr_ind = last_pos_in_passed;
    ind = last_sym_pos - curr_ind;
    // //DEBUG
    // std::cout << "CURR-SUFF: ";
    // for (size_t i = curr->left; i < text.length(); ++i)
    //   std::cout << text[i];
    // std::cout << std::endl;
    // std::cout << "CURR-TEXT: ";
    // for (size_t i = curr->left; i < *(curr->right)+((curr->is_tmp)? 0 : 1); ++i)
    //   std::cout << text[i];
    // std::cout << std::endl;
    // std::cout << "DIFF: " << text[curr->left + curr_ind] << ':' << text[ind + curr_ind] << std::endl;
    // //
  } else {
    prev = nullptr;
    prev_child = &child;
  }
  //
  while (curr != nullptr) {
    for (; curr_ind < *(curr->right)-curr->left+((curr->is_tmp)? 0 : 1) && curr_ind < text.length() - ind; ++curr_ind)
      //Check fork
      if (text[curr->left + curr_ind] != text[ind+curr_ind]) {
        Trie_suffix::Node* tmp = new Trie_suffix::Node(curr->left, &(curr->left));
        tmp->is_tmp = true;
        curr->left = curr->left + curr_ind;
        (*prev_child)[text[tmp->left]] = tmp;
        // std::cout << "TMP-LEFT: " << text[tmp->left] << std::endl;
        tmp->child[text[curr->left]] = curr;
        if (prev != nullptr && *(prev->right) > tmp->left) prev->right = &(tmp->left);

        ind+=curr_ind;

        tmp->child[text[ind]] = new Trie_suffix::Node(ind, &last_sym_pos);
        ++from_pos;
        // //DEBUG
        // std::cout << "FROM-POS>FORK: " << from_pos << std::endl;
        // if (from_pos == 13) {
        //   std::cout << "-------------" << std::endl;
        //   print();
        //   std::cout << "-------------" << std::endl;
        // }
        // //
        is_fork = true;
        // //DEBUG
        // std::cout << "CURR-IND: " << curr_ind << std::endl;
        // //
        //Optimization
        last_passed = nullptr;
        last_pos_in_passed = 0;
        // //DEBUG
        // std::cout << "LAST-POS>FORK: " << last_pos_in_passed << std::endl;
        // //
        //
        return tmp->child[text[ind]];
      }
    ind+=curr_ind;
    //It's prefix
    if (ind == text.length()) {
      is_fork = false;
      //Optimization
      last_passed = curr;
      last_pos_in_passed = curr_ind;
      // //DEBUG
      // std::cout << "CURR-TEXT: ";
      // for (size_t i = curr->left; i < text.length(); ++i)
      //   std::cout << text[i];
      // std::cout << std::endl;
      // std::cout << "CUR-POS>PREF: " << last_pos_in_passed << std::endl;
      // std::cout << "LAST-IND>PREF: " << ind << std::endl;
      // //
      //
      return curr;
    }
    //It's new tail from node -> fork
    if (curr->is_tmp && curr->child[text[ind]] == nullptr) {
      curr->child[text[ind]] = new Trie_suffix::Node(ind, &last_sym_pos);
      ++from_pos;
      // //DEBUG
      // std::cout << "FROM-POS>TAIL: " << from_pos << std::endl;
      // //
      is_fork = true;
      //Optimization
      last_passed = nullptr;
      last_pos_in_passed = 0;
      // //DEBUG
      // std::cout << "LAST-POS>TAIL: " << last_pos_in_passed << std::endl;
      // //
      //
      return curr->child[text[ind]];
    }

    prev_child = &curr->child;
    prev = curr;
    curr = curr->child[text[ind]];
    curr_ind = 0;
    //Optimization
    last_passed = nullptr;
    last_pos_in_passed = 0;
    // //DEBUG
    // std::cout << "LAST-POS>BREAK: " << last_pos_in_passed << std::endl;
    // //
  }

  return nullptr;
}

bool Trie_suffix::substring_exist(const std::string& key) {
  size_t ind = 0, curr_ind;
  Trie_suffix::Node* curr = child[key[ind]];

  while (curr != nullptr) {
    for (curr_ind = 0; curr_ind < *(curr->right)-curr->left+((curr->is_tmp)? 0 : 1) && curr_ind < key.length() - ind; ++curr_ind)
      if (text[curr->left + curr_ind] != key[ind+curr_ind])
        return false;

    ind+=curr_ind;

    if (ind == key.length())
      return true;

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
  Trie_suffix::Node *curr = nullptr;
  text.push_back(sym);
  last_sym_pos = text.length()-1;
  do {
    bool is_fork = false;
    curr = find_prefix_suffix(from_pos, is_fork);
    if (curr == nullptr) {//suffix is not found => add sym into root->childs
      child[sym] = new Trie_suffix::Node(text.length()-1, &last_sym_pos);
      from_pos = text.length();
      // //DEBUG
      // std::cout << "FROM-POS>NEW: " << from_pos << std::endl;
      // //
      //Optimization
      last_passed = nullptr;
      last_pos_in_passed = 0;
      // //DEBUG
      // std::cout << "LAST-POS>NEW: " << last_pos_in_passed << std::endl;
      // //
      //
      break;
    }
    if (!is_fork)//suffix is prefix => do nothing
      break;
    //suffix node is fork
  } while (curr != nullptr);
}

void Trie_suffix::print(std::ofstream& f_out) {
  for (size_t i = 0; i < child.size()/2; ++i)
      print_node(child[i], 1, i, f_out);

  f_out << '.' << std::endl;

  for (size_t i = child.size()/2; i < child.size(); ++i)
      print_node(child[i], 1, i, f_out);
}

void Trie_suffix::print() {
  for (size_t i = 0; i < child.size()/2; ++i)
      print_node(child[i], 1, i);

  std::cout << '.' << std::endl;

  for (size_t i = child.size()/2; i < child.size(); ++i)
      print_node(child[i], 1, i);
}

void Trie_suffix::print_node (Node* node, size_t deep, char sym, std::ofstream& f_out) {
  if (node == nullptr) return;

  for (size_t i = 0; i < node->child.size()/2; ++i)
    print_node(node->child[i], deep+1, i, f_out);

  f_out << std::string(deep*8, ' ');
  for (size_t i = node->left; i < *(node->right)+((node->is_tmp)? 0 : 1); ++i)
    f_out << text[i];
  f_out << std::endl;

  for (size_t i = node->child.size()/2; i < child.size(); ++i)
    print_node(node->child[i], deep+1, i, f_out);
}

void Trie_suffix::print_node (Node* node, size_t deep, char sym) {
  if (node == nullptr) return;

  for (size_t i = 0; i < node->child.size()/2; ++i)
    print_node(node->child[i], deep+1, i);

  std::cout << std::string(deep*8, ' ');
  for (size_t i = node->left; i < *(node->right)+((node->is_tmp)? 0 : 1); ++i)
    std::cout << text[i];
  std::cout << std::endl;

  for (size_t i = node->child.size()/2; i < child.size(); ++i)
    print_node(node->child[i], deep+1, i);
}
