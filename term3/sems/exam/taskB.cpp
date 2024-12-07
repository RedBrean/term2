#include <memory>

template <typename element_t>
struct TreeNode final {
    explicit TreeNode(element_t const &element): TreeNode(element, nullptr, nullptr) { }
    TreeNode(element_t const &element, std::unique_ptr<TreeNode<element_t>> left_tree, std::unique_ptr<TreeNode<element_t>> right_tree):
        element(element), left_tree(std::move(left_tree)), right_tree(std::move(right_tree)) { }

    element_t const element;
    std::unique_ptr<TreeNode<element_t>> left_tree;
    std::unique_ptr<TreeNode<element_t>> right_tree;
};    

template<typename element_t> 
std::unique_ptr<TreeNode<element_t>> flat_tree(std::unique_ptr<TreeNode<element_t>> root) {
    if(!root){return nullptr;}
    auto flatted_left_tree = flat_tree(std::move(root->left_tree));
    auto flatted_right_tree = flat_tree(std::move(root->right_tree));
    root->right_tree = std::move(flatted_right_tree);
    root->left_tree = nullptr;

    
    TreeNode<element_t>* tail = flatted_left_tree.get();
    if(tail)
    {
        while(tail->right_tree)
        {
            tail = tail->right_tree.get();
        }
        tail->right_tree = std::move(root);
        return std::move(flatted_left_tree);
    }
    else
    {
        return std::move(root);
    }
}

#include <iostream>

template <typename T>
void print_flattened_tree(std::unique_ptr<TreeNode<T>>& root) {
    auto node = root.get();
    while (node) {
        std::cout << node->element << " ";
        node = node->right_tree.get();
    }
}

int main() {
    // Строим исходное дерево
    auto root = std::make_unique<TreeNode<int>>(1);
    root->left_tree = std::make_unique<TreeNode<int>>(2);
    root->right_tree = std::make_unique<TreeNode<int>>(3);
    root->left_tree->left_tree = std::make_unique<TreeNode<int>>(4);
    root->left_tree->right_tree = std::make_unique<TreeNode<int>>(5);

    // Преобразуем дерево в "выпрямленную" структуру
    auto flattened_tree = flat_tree(std::move(root));

    // Выводим преобразованное дерево
    print_flattened_tree(flattened_tree);

    return 0;
}
