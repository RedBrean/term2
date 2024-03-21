#include <iostream>

using std::cout;
using std::endl;

struct Node
{
    int key = 0;
    Node* next = nullptr;
};

void push_back(Node*& head_ref, int new_data)
{
    if(head_ref == nullptr)
    {
        head_ref = new Node;
        head_ref->key = new_data;
        return;
    }
    Node* node = head_ref;
    while(node->next != nullptr)
    {
        node = node->next;
    }
    node->next = new Node;
    node->next->key = new_data;
}

void push_front(Node*& head_ref, int new_key)
{
    Node* new_head = new Node;
    new_head->next = head_ref;
    new_head->key = new_key;
    head_ref = new_head;
}

void insert_after(Node*& prev_ref, int new_key){
    if(prev_ref == nullptr){
        prev_ref = new Node;
    }
    Node* new_node = new Node;
    new_node->next = prev_ref->next;
    new_node->key = new_key;
    prev_ref->next = new_node;
}

void destroy_list(Node*& head_ptr)
{
    if(head_ptr == nullptr)
    {return;}
    while(head_ptr->next != nullptr)
    {
        auto next = head_ptr->next;
        delete head_ptr;
        head_ptr = next;
    }
    delete head_ptr;
    head_ptr = nullptr;
}

void print_list(Node* head_ref){
    Node* node = head_ref;
    if(node == nullptr){
        std::cout << "\n";
        return;
    }
    std::cout << node->key;
    while (node->next != nullptr)
    {
        node = node->next;
        std::cout << " " << node->key;
    }
    std::cout << "\n";
}

void remove_key(Node*& head_ptr, int key){
    auto prev = head_ptr;
    auto next = head_ptr;

    if(next == nullptr){return;}
    while(next!=nullptr){
        if(next->key == key){
            if(next != head_ptr){
                prev->next = next->next;
            }
            else{
                head_ptr = next->next;
            }
            delete next;
            return;
        }
        auto temp = next;
        next = prev->next;
        prev = temp;
    }
}

Node* find(Node* head_ref, int key){
    if(head_ref == nullptr){return nullptr;}
    Node* current = head_ref;
    while(current->next != nullptr)
    {
        if(current->key == key){return current;}
        current = current->next;
    }
    if(current->key == key){return current;}
    return nullptr;
}

int main() {
  Node* HEAD = nullptr;
  push_back(HEAD, 0);
  push_back(HEAD, 1);
  push_back(HEAD, 2);
  remove_key(HEAD, 0);
  remove_key(HEAD, 1);
  remove_key(HEAD, 2);
  remove_key(HEAD, 3);
  if (HEAD == nullptr) {
    cout << "OK" << endl;
  }
  return 0;
}