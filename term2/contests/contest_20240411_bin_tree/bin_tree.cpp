#include <iostream>

using std::cin;
using std::cout;
using std::endl;

struct Node
{
  int key = 0;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;
};

void insert_node(Node *&tree, int new_key)
{
  if (tree == nullptr)
  {
    tree = new Node{.key = new_key};
    return;
  }
  int c_key = tree->key;

  if (new_key <= c_key)
  {
    if (tree->left == nullptr)
    {
      tree->left = new Node{.key = new_key, .parent = tree};
    }
    else
    {
      insert_node(tree->left, new_key);
    }
  }
  else
  {
    if (tree->right == nullptr)
    {
      tree->right = new Node{.key = new_key, .parent = tree};
    }
    else
    {
      insert_node(tree->right, new_key);
    }
  }
}

Node* min(Node *&tree)
{
  if (tree == nullptr)
  {
    return tree;
  }
  if (tree->left == nullptr)
  {
    return tree;
  }
  return min(tree->left);
}

Node* max(Node *&tree)
{
  if (tree == nullptr)
  {
    return tree;
  }
  if (tree->right == nullptr)
  {
    return tree;
  }
  return max(tree->right);
}

Node* find(Node *&tree, int key){
  if (tree == nullptr){ return nullptr; }
  if(tree->key == key){ return tree; }
  if(key<=tree->key){ return find(tree->left, key); }
  else{ return find( tree->right, key); }
}

Node* my_next(Node*& tree, int key){
  if (tree == nullptr){ return nullptr; }
  if (tree->key <= key){
    return my_next(tree->right, key);
  }
  else{
    auto rec_next = my_next(tree->left, key);
    if(rec_next == nullptr){return tree;}
    else{return rec_next;}
  }
}

Node* next(Node*& tree, int key){
  if(find(tree, key) == nullptr) {return nullptr;}
  return my_next(tree, key);
}


int main() {
  Node* tree = nullptr;
  insert_node(tree, 1);
  insert_node(tree, 2);
  insert_node(tree, 0);
  insert_node(tree, -1);
  if (next(tree, 1) == tree->right and next(tree, -1) == tree->left and next(tree, 2) == nullptr) {
    cout << "OK" << endl;
  }
  delete tree->left->left;
  delete tree->left;
  delete tree->right;
  delete tree;
  return 0;
}