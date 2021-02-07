#include <iostream>
#include <vector>

template <typename T>
struct Node
{
    Node* left;
    Node* right;
    T information;
    size_t height;
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
int height(Node<T>* node)
{
    if (node == nullptr)
        return 0;
    else
        return node->height;
}

template <typename T>
void fix_height(Node<T>* node)
{
    if (height(node->right) > height(node->left))
        node->height = height(node->right) + 1;
    else
        node->height = height(node->left) + 1;
}

template <typename T>
Node<T>* small_rotate_left(Node<T>* up)
{
    Node<T>* down = up->right;
    up->right = down->left;
    down->left = up;
    fix_height(up);
    fix_height(down);
    return down;
}

template <typename T>
Node<T>* small_rotate_right(Node<T>* up)
{
    Node<T>* down = up->left;
    up->left = down->right;
    down->right = up;
    fix_height(up);
    fix_height(down);
    return down;
}

template <typename T>
Node<T>* big_rotate_right(Node<T>* node)
{
    node->left = small_rotate_left(node->left);
    node = small_rotate_right(node);
    return node;
}

template <typename T>
Node<T>* big_rotate_left(Node<T>* node)
{
    node->right = small_rotate_right(node->right);
    node = small_rotate_left(node);
    return node;
}

template <typename T>
Node<T>* balance(Node<T>* node)
{
    fix_height(node);

    if ((height(node->right) - height(node->left) == 2) && ((height(node->right->right) == height(node->right->left)) ||
            (height(node->right->right) - height(node->right->left) == 1)))
        node = small_rotate_left(node);

    else if ((height(node->left) - height(node->right) == 2) && ((height(node->left->left) == height(node->left->right)) ||
            (height(node->left->left) - height(node->left->right) == 1)))
        node = small_rotate_right(node);

    else if ((height(node->right) - height(node->left) == 2) && (height(node->right->left) > height(node->right->right)))
        node = big_rotate_left(node);

    else if ((height(node->left) - height(node->right) == 2) && (height(node->left->right) > height(node->left->left)))
        node = big_rotate_right(node);

    return node;
}

template <typename T>
Node<T>* insert(Node<T>* node, T inf)
{
    if (node == nullptr)
    {
        Node<T>* element = new Node<T>;
        constructor(element, inf);
        return element;
    }
    else if (node->information > inf)
        node->left = insert(node->left, inf);
    else
        node->right = insert(node->right, inf);

    node =  balance(node);
    return node;
}

template <typename T>
Node<T>* find_max(Node<T>* node)
{
    if (node->right == nullptr)
        return node;
    else
        return find_max(node->right);
}

template <typename T>
Node<T>* find_previous_max(Node<T>* node, Node<T>* max)
{
    if (node->right == max || node == max)
        return node;
    else
        return find_previous_max(node->right, max);
}

template <typename T>
Node<T>* find_previous(Node<T>* node, T inf)
{
    if (node->left->information == inf || node->right->information == inf)
        return node;
    else if (node->information > inf)
        return find_previous(node->left, inf);
    else if (node->information < inf)
        return find_previous(node->right, inf);
}


template <typename T>
Node<T>* remove(Node<T>* node, T inf)
{

    if (node != nullptr && node->information < inf)
        node->right = remove(node->right, inf);
    else if (node != nullptr && node->information > inf)
        node->left = remove(node->left, inf);
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        else if (node->right == nullptr && node->left != nullptr)
        {
            Node<T>* next = node->left;
            delete node;
            return next;
        }
        else if (node->right != nullptr && node->left == nullptr)
        {
            Node<T>* next = node->right;
            delete node;
            return next;
        }
        else
        {
            Node<T>* max = find_max(node->left);
            Node<T>* previous_max = find_previous_max(node->left, max);

            if (previous_max == max)
            {
                node->information = max->information;
                node->left = max->left;
                delete max;
                return balance(node);
            }
            else
            {
                node->information = max->information;
                previous_max->right = max->left;
                delete max;
                balance(previous_max);
            }
        }
        return balance(node);
    }
    return balance(node);
}

std::ostream& operator<<(std::ostream& output, std::pair<int, std::string>& element)
{
    output << element.first << ':' << element.second;
    return output;
}

/*функция для печати элементов дерева в вериткальном ввиде (элемент - высота элемента на дереве)*/
template <typename T>
void print(Node<T>* node)
{
    if (node != nullptr)
    {
        print(node->left);
        std::cout << node->information << " Height: " << node->height << std::endl;
        print(node->right);
    }
}

/*функция для уничтожения всех элементов дерева*/
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
    int command;
    int type;

    std::cout << "Enter '1' to work with integer values or '2' to work with pair of elements: ";
    std::cin >> type;
    if (type == 1)
    {
        Node<int>* tree = new Node<int>;
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
                int element;
                std::cout << "The element you want to create on the AVL-tree: ";
                std::cin >> element;
                constructor(tree, element);
                std::cout << "Constructing was finished successfully \n";
            }
            else if (command == 2) {
                std::cout << "Enter the element you want to insert: ";
                int element;
                std::cin >> element;
                tree = insert(tree, element);
                std::cout << "Inserting was finished successfully \n";
            }
            else if (command == 3)
            {
                int element;
                std::cout << "Enter the element you want to delete: ";
                std::cin >> element;
                remove(tree, element);
                std::cout << "The element was deleted successfully.";
            }
            else if (command == 4)
            {
                print(tree);
            }
            else if (command == 5)
            {
                destructor(tree);
                std::cout << "AVL-tree is empty \n";
            }

        } while (command != 6);
    }
    else
    {
        Node<std::pair<int, std::string>>* tree = new Node<std::pair<int, std::string>>;
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
                std::pair<int, std::string> element;
                std::cout << "The key you want to create on the AVL-tree: ";
                std::cin >> element.first;
                std::cout << "The value you want to create on the AVL-tree: ";
                std::cin >> element.second;
                constructor(tree, element);
                std::cout << "Constructing was finished successfully \n";
            }
            else if (command == 2)
            {
                std::pair<int, std::string> element;
                std::cout << "The key: ";
                std::cin >> element.first;
                std::cout << "The value: ";
                std::cin >> element.second;
                tree = insert(tree, element);
                std::cout << "Inserting was finished successfully \n";
            }
            else if (command == 3)
            {
                std::pair<int, std::string> element;
                std::cout << "Enter the key of element you want to delete: ";
                std::cin >> element.first;
                std::cout << "Enter the element you want to delete: ";
                std::cin >> element.second;
                remove(tree, element);
                std::cout << "The element was deleted successfully.";
            }
            else if (command == 4)
            {
                print(tree);
            }
            else if (command == 5)
            {
                destructor(tree);
                std::cout << "AVL-tree is empty \n";
            }

        } while (command != 6);
    }
    std::cout << "That's all, goodbye!";

    return 0;
}
