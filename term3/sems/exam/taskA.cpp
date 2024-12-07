template <typename element_t>
struct TreeNode final {
    element_t const element;
    TreeNode<element_t> *left_tree = nullptr;
    TreeNode<element_t> *right_tree = nullptr;
};


template <typename element_t>
void delete_tree(TreeNode<element_t> *root) {
    if (!root) return;
    if (root->left_tree) delete_tree(root->left_tree);
    if (root->right_tree) delete_tree(root->right_tree);
    delete root;
}
        

#include <stdexcept>
#include <iostream>
template <typename element_t> 
TreeNode<element_t>* copy(TreeNode<element_t> const *src_root)
{
    if(!src_root){return nullptr;}
    TreeNode<element_t>* result_tree = nullptr;
    try{
    result_tree = new TreeNode<element_t>{src_root->element};
    result_tree->left_tree = copy(src_root->left_tree);
    result_tree->right_tree = copy(src_root->right_tree);
    
    }
    catch(...){
        delete_tree(result_tree);
        throw;
    }

    return result_tree;
}

int main()
{
    auto my_tree_1 = new TreeNode<int>{1};
    my_tree_1->left_tree =  new TreeNode<int>{2};
    auto my_tree_2 = copy(my_tree_1);
    std::cout<< my_tree_2->left_tree->element;
    delete_tree(my_tree_1);
    delete_tree(my_tree_2);
    return 0;
}