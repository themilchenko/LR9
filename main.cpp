#include <iostream>

template <typename T>
struct Node
{
    Node* left;
    Node* right;
    T information;
    int height = 0;
};

template <typename T>
void constructor(Node<T>* node, T inf)
{
    node->left = nullptr;
    node->right = nullptr;
    node->information = inf;
    node->height = 1;
}

template <typename T>
int height(Node<T>* tree)
{
    if (!tree)
        return 1;
    else
        return tree->height;
}

template <typename T>
int delta(Node<T>* tree)
{
    return height(tree->right) - height(tree->left);
}

template <typename T>
void fix_height(Node<T>* tree)
{
    if (height(tree->left) > height(tree->right))
        tree->height = height(tree->left) + 1;
    else
        tree->height = height(tree->right) + 1;
}

template <typename T>
Node<T>* rotate_right(Node<T>* up)
{
    Node<T>* down = up->left;
    up->left = down->right;
    down->right = up;
    fix_height(down);
    fix_height(up);
    return down;
}

template <typename T>
Node<T>* rotate_left(Node<T>* up)
{
    Node<T>* down = up->right;
    up->right = down->left;
    down->left = up;
    fix_height(down);
    fix_height(up);
    return down;
}

template <typename T>
Node<T>* balance(Node<T>* node)
{
    fix_height(node);
    if (delta(node) == 2)
    {
        if (delta(node->right) < 0)
        {
            node->right = rotate_right(node);
            return rotate_left(node);
        }
    }
    else if (delta(node) == -2)
    {
        if (delta(node) > 0)
            node->left = rotate_left(node);
        return rotate_right(node);
    }
        return node;
}

template <typename T>
Node<T>* insert(Node<T> *node, T key)
{
    if (!node)
    {
        Node<T>* actual = new Node<T>;
        constructor(actual, key);
        return actual;
    }
    else if (key < node->information)
        node->left = insert(node->left, key);
    else if (key > node->information)
        node->right = insert(node->right, key);
    return balance(node);
}

template <typename T>
Node<T>* find_min(Node<T>* node)
{
    if (node->left != nullptr)
        find_min(node);
    else
        return node;
}

template <typename T>
Node<T>* remove_min(Node<T>* node)
{
    if (node->left == nullptr)
        return node->right;
    node->left = remove_min(node->left);
    return balance(node);
}

template <typename T>
Node<T>* remove(Node<T>* node, T key)
{
    if (node == nullptr)
        return 0;
    if (key < node->information)
        node->left = remove(node->left, key);
    else if (key > node->information)
        node->right = remove(node->right, key);
    else if (key == node->information)
    {
        Node<T>* l = node->left;
        Node<T>* r = node->right;
        delete node;
        if (r == nullptr)
            return l;
        Node<T>* min = find_min(r);
        min->right = remove_min(r);
        min->left = l;
        return balance(min);
    }
    return balance(node);
}

template <typename T>
void print(Node<T>* node)
{
    if (node != nullptr)
    {
        print(node->left);
        std::cout << node->information << " Height: " << height(node) << std::endl;
        print(node->right);
    }
}

template <typename T>
void destructor(Node<T>* node)
{
    if (node != nullptr)
    {
        destructor(node->left);
        destructor(node->right);
        delete node;
    }
}

int main()
{
    Node<int> *tree;

    int command;

    do
    {

        std::cout << "What do you want to do? \n"
                  << "1. Construct AVL-tree \n"
                  << "2. Insert element \n"
                  << "3. Delete element \n"
                  << "4. See AVL-tree \n"
                  << "5. Delete tree \n"
                  << "6. Finish \n"
                  << "Enter the number of command to complete this: ";

        std::cin >> command;

        if (command == 1)
        {
            constructor(tree, 1);
            std::cout << "Constructing was finished successfully \n";
        }
        else if (command == 2)
        {
            std::cout << "Enter the element you want to insert: ";
            int element;
            std::cin >> element;
            insert(tree, element);
            std::cout << "Inserting was finished successfully \n";
        }
        else if ( command == 3)
        {

        }
        else if (command == 4)
            print(tree);
        else if (command == 5) {
            destructor(tree);
            std::cout << "AVL-tree is empty \n";
        }

    } while (command != 6);

    std::cout << "That's all, goodbye!";

    return 0;
}