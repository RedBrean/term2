#include <iostream>

using std::cout;
using std::endl;

struct Node
{
    int key = 0;
    Node* next;
    Node* prev;
};

struct List
{
    Node* HEAD;
    Node* TAIL;
    Node* NIL;
    size_t size;
};

List* create_empty_list(){
    List* out = new List;
    out->NIL = new Node;
    out->HEAD = out->NIL;
    out->TAIL = out->NIL;
    return out;
}

void print_list_info(List* list_ptr){
    cout << "list size = " << list_ptr->size << "\n-> : ";
    Node* node = list_ptr->HEAD;
    for(int i = 0; i < list_ptr->size; i++)
    {
        cout << node->key << " ";
        node = node->next;
    }
    cout << "\n<- : ";
    node = list_ptr->TAIL;
    for(int i = 0; i < list_ptr->size; i++)
    {
        cout << node->key << " ";
        node = node->prev;
    }
}

void print_list(List* list_ptr){
    if(list_ptr == nullptr || list_ptr->size == 0) {return;}
    Node* node = list_ptr->HEAD;
    while(node != list_ptr->TAIL){
        std::cout << node->key << " ";
        node = node->next;
    }
    std::cout << node->key << "\n";
}

List* push_front(List* list_ptr, int new_key){
    if(list_ptr == nullptr){
        list_ptr = create_empty_list();
    }
    Node* nNode = new Node;
    nNode->key = new_key;
    nNode->next = list_ptr->HEAD;
    nNode->prev = list_ptr->NIL;
    if(list_ptr->HEAD == list_ptr->NIL){
        list_ptr->HEAD = nNode;
        list_ptr->TAIL = nNode;
    }
    else{
        list_ptr->HEAD->prev = nNode;
        list_ptr->HEAD = nNode;
    }
    list_ptr->size++;
    return list_ptr;
}

List* insert_after(List* list_ptr, Node* prev_ptr, int new_key){
    if(list_ptr == nullptr || prev_ptr == list_ptr->NIL){
        return list_ptr;
    }

    Node* new_node = new Node;
    new_node->key = new_key;
    new_node->next = prev_ptr->next;
    new_node->prev = prev_ptr;
    prev_ptr->next = new_node;
    list_ptr->size++;
    if(list_ptr->TAIL == prev_ptr){
        list_ptr->TAIL = new_node;
    }
    return list_ptr;
}

List* push_back(List* list_ptr, int new_key){
    if(list_ptr == nullptr){
        list_ptr = create_empty_list();
    }
    Node* new_node = new Node;
    new_node->key = new_key;
    new_node->next = list_ptr->NIL;
    new_node->prev = list_ptr->TAIL;
    list_ptr->TAIL->next = new_node;
    list_ptr->TAIL = new_node;
    if(list_ptr->HEAD == list_ptr->NIL){
        list_ptr->HEAD = new_node;
    }
    list_ptr->size++;
    return list_ptr;
}

void clear_list(List* list_ptr){
    if(list_ptr == nullptr || list_ptr->size == 0) {return;}
    Node* node = list_ptr->HEAD;
    Node* del_node;
    while(node != list_ptr->NIL)
    {
        del_node = node;
        node = node->next;
        delete del_node;
    }
    list_ptr->size = 0;
    list_ptr->HEAD = list_ptr->NIL;
    list_ptr->TAIL = list_ptr->NIL;
}

void destroy_list(List* list_ptr){
    if(list_ptr == nullptr) {return;}
    if(list_ptr->size == 0) {
        delete list_ptr->NIL;
        delete list_ptr;
    }
    Node* node = list_ptr->HEAD;
    Node* del_node;
    while(node != list_ptr->NIL)
    {
        del_node = node;
        node = node->next;
        delete del_node;
    }
    delete list_ptr->NIL;
    delete list_ptr;
}

void remove_key(List* list_ptr, int key){
    if(list_ptr == nullptr || list_ptr->size == 0) {return;}
    if(list_ptr->HEAD == list_ptr->TAIL){
        if(list_ptr->HEAD->key == key) {
            delete list_ptr->HEAD;
            list_ptr->HEAD = list_ptr->NIL;
            list_ptr->TAIL = list_ptr->NIL;
            list_ptr->size = 0;
            return;
        }
    }
    Node* node = list_ptr->HEAD;
    for(size_t i = 0; i<list_ptr->size; i++){
        if(node->key == key){
            if(node == list_ptr->HEAD){
                list_ptr->HEAD = node->next;
                delete node;
            }
            else if(node == list_ptr->TAIL){
                list_ptr->TAIL = node->prev;
                delete node;
            }
            else{
                node->prev->next = node->next;
                node->next->prev = node->prev;
                delete node;
            }
            list_ptr->size--;
            break;
        }
        node = node->next;
    }
   
}

Node* find_key(List* list_ptr, int key){
    if(list_ptr == nullptr) {return nullptr;}
    if(list_ptr->size == 0) {return list_ptr->NIL;}
    Node* node = list_ptr->HEAD;
    for(size_t i = 0; i<list_ptr->size; i++){
        if(node->key == key) {return node;}
        node = node->next;
    }
    return list_ptr->NIL;
}

int main() {
  List* list = create_empty_list();
  Node* ptr = nullptr;
  push_back(list, 0);
  push_back(list, 1);
  push_back(list, 2);
  ptr = find_key(list, 2);
  if (ptr == list->TAIL) {
    cout << "OK" << endl;
  }
  destroy_list(list);
  return 0;
}