#include "../include/Trie_suffix.h"

Trie_suffix::Trie_suffix()
{
    child.resize(CHAR_MAX, nullptr);
    max_len_str = nullptr;
}

void Trie_suffix::delete_node(Trie_suffix::Node* node) {
    if (node == nullptr)
        return;
    for (Trie_suffix::Node* elem : node->child) {
        delete_node(elem);
        delete elem;
    }
}

Trie_suffix::~Trie_suffix()
{
    for (Trie_suffix::Node* elem : child) {
        delete_node(elem);
        delete elem;
    }
    child.clear();
}

value_type* Trie_suffix::find(const std::string& key) {
    return find_node(child[key[0]], key, 0);
}

value_type* Trie_suffix::find_node(Trie_suffix::Node* node, const std::string& key, size_t ind) {
    if (node == nullptr || key.length() - ind < node->key.length())
        return nullptr;

    for (size_t i = 0; i < node->key.length(); ++i)
        if (key[ind + i] != node->key[i])
            return nullptr;

    ind += node->key.length();

    if (key.length() == ind)
        return &(node->value);

    return find_node(node->child[key[ind]], key, ind);
}

bool Trie_suffix::check_prefix_suffix(const std::string& key) {
    if (find_node_prefix_suffix(child[key[0]] , key, 0) == nullptr)
        return false;
    return true;
}

Trie_suffix::Node* Trie_suffix::find_node_prefix_suffix( Node* node, const std::string& key, size_t ind) {
    size_t curr;
    if (node == nullptr)
        return nullptr;

    for (curr = 0; curr < node->key.length() && curr < key.length() - ind; ++curr)
        if (key[ind + curr] != node->key[i])
            return nullptr;

    ind += curr;

    if (curr == key.length())
        return node;

    return find_node_prefix_suffix(node->child[key[ind]], key, ind);
}

void Trie_suffix::clear() {
    for (Trie_suffix::Node* elem : child) {
        delete_node(elem);
        delete elem;
    }
    child.resize(CHAR_MAX, nullptr);
    max_len_str = nullptr;
}

void Trie_suffix::make(const std::string& src) {
    clear();
    for (size_t i = 0; i < src.length(); ++i)
        add_suffix(src[i]);
}

//TODO: complete add_suffix:
void Trie_suffix::add_suffix(char sym) {
    Trie_suffix::Node* curr = max_len_str, prev = curr;
    if (max_len_str == nullptr) {
        child[sym] = new Trie_suffix::Node(nullptr);
        child[sym]->key.push_back(sym);
        max_len_str = child[sym];
        return;
    }
    do {
        curr->key.push_back(sym);
        prev = curr;
        curr = curr->suffix_ptr;
    } while (curr != nullptr);
    if (prev->h + prev->key.length() - 1) {
        curr = find_node_prefix_suffix(child[prev->key[1]], prev->key, 1);
        if (curr == nullptr)
    }
}
