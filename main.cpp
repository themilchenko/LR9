#include <iostream>
#include <vector>

/*структура узла дерева*/
template <typename T>
struct Node
{
    Node* left;         /*указатель на левый элемент*/
    Node* right;        /*указатель на правый элемент*/
    T information;      /*значение элемента дерева*/
    int height = 0;     /*высота поддерева*/
};

/*конструктор дерева*/
template <typename T>
void constructor(Node<T>* node, T inf)
{
    node->left = nullptr;
    node->right = nullptr;
    node->information = inf;
    node->height = 1;
}

/*подсчет высоты поддерева*/
template <typename T>
int height(Node<T>* tree)
{
    if (!tree)                  /*если указатель поддерева указывает на пустоту*/
        return 0;               /*возвращается ноль*/
    else                        /*иначе*/
        return tree->height;    /*возвращается высота*/
}

/*разность высот правого и левого поддеревьев*/
template <typename T>
int delta(Node<T>* tree)
{
    return height(tree->right) - height(tree->left);
}

/*исправление высот узла после добавления / удаления элемента*/
template <typename T>
void fix_height(Node<T>* tree)
{
    if (height(tree->left) > height(tree->right))    /*если высота левого поддерева больше правого*/
        tree->height = height(tree->left) + 1;       /*то высота самого узла будет являться сумма левого поддерева и единицы*/
    else                                             /*иначе*/
        tree->height = height(tree->right) + 1;      /*высота узла - сумма правого поддерева и единицы*/
}

/*малый поворот вправо*/
template <typename T>
Node<T>* rotate_right(Node<T>* up)
{
    Node<T>* down = up->left;         /*ссылаемся на нижнее левое поддерево*/
    up->left = down->right;           /*связываем верхушку дерева с правым поддеревом левого поддерева*/
    down->right = up;                 /*нижний узел ссылается на бывшую верхушку*/
    fix_height(up);                   /*исправляем высоту уже нижнего поддерева после поворота*/
    fix_height(down);                 /*исправляем высоту врехнего поддерева*/
    return down;                      /*возвращаем верхний узел*/
}

/*малый поворот влево*/
template <typename T>
Node<T>* rotate_left(Node<T>* up)
{
    Node<T>* down = up->right;         /*ссылаемся на нижнее правое поддерево*/
    up->right = down->left;            /*теперь связываем верхущку дерева с левым поддеревом праого поддерева*/
    down->left = up;                   /*также нижний узел ссылается на бывшую верхушку*/
    fix_height(up);                    /*исправляем высоту уже нижнего поддерева после поворота*/
    fix_height(down);                  /*исправляем высоту верхнего поддерева*/
    return down;                       /*возвращаем верхний узел*/
}

/*выполняет балансировку: повороты и исправления высот поддеревьев*/
template <typename T>
Node<T>* balance(Node<T>* node)
{
    fix_height(node);                                    /*исправление высоты поддерева*/
    if (delta(node) == 2)                                /*если разница между праым и левым поддеревьями равна 2*/
    {                                                    /*то заходим в условию для коррекции ситуации*/
        if (delta(node->right) < 0)                      /*если разница поддеревьев у правого поддерева отрицательная (будет выполняться большой поворот влево)*/
            node->right = rotate_right(node->right);     /*выполняем поворот вправо вокруг правого поддерева*/
        return rotate_left(node);                        /*выполняем поворот влево*/
    }
    else if (delta(node) == -2)                          /*если разница -2*/
    {
        if (delta(node->left) > 0)                       /*разница положительна*/
            node->left = rotate_left(node->left);        /*выполняем поворот влево*/
        return rotate_right(node);                       /*поворачиваем вправо*/
    }
    return node;                                         /*возвращаем узел*/
}

/*функция вставки элемента в дерево*/
template <typename T>
Node<T>* insert(Node<T>* node, T key)
{
    if (!node)                                      /*если узел указывает на пустоту*/
    { 
        Node<T>* actual = new Node<T>;              /*создаем динамически структуру узла*/
        constructor(actual, key);                   /*создаем элемент и добавляем его в дерево*/
        return actual;                              /*возвращаем элемент*/
    }
    else if (key < node->information)               /*если ключ меньше значения*/
        node->left = insert(node->left, key);       /*идем влево*/
    else if (key > node->information)               /*иначе*/
        node->right = insert(node->right, key);     /*идем вправо*/
    return balance(node);                           /*балансируем дерево после вставки*/
}

/*функция для нахождения максимального элемента в поддереве*/
template <typename T>
Node<T>* find_max(Node<T>* node)
{
    if (node->right == nullptr)
        return node;
    else
        return find_max(node->right);
}

/*функция нахождения элемента, стоящего перед максимальным элементом левого поддерева*/
template <typename T>
Node<T>* previous_max(Node<T>* node, Node<T>* max)
{
    if (node->right == max)                        /*если справа находится максимальная величина*/
        return node;                               /*возвращаем узел, находящийся до максимальной величины*/
    else if (node == max)                          /*если сам узел и есть максимальный*/
        return node;                               /*также возвращаем его*/
    else                                           /*иначе*/
        return previous_max(node->right, max);     /*идем дальше*/
}

/*функция удаления элемента*/
template <typename T>
Node<T>* remove(Node<T>* node, T key)
{
    Node<T>* previous = new Node<T>;                    /*создаю динамически элемент, чтобы запомнить элемент, находящийся перед удаляемым элементом*/
    if (node == nullptr)                                /*если дошли до пустого элемента и не нашли нужный, значит его нет*/
        return 0;                                       /*выходим и функции*/
    if (key < node->information)                        /*если текущий элемент больше удаляемого*/
    {
        previous = node;                                /*запоминаем этот элемент*/
        node->left = remove(node->left, key);           /*идем влево*/
    }
    else if (key > node->information)                   /*если текущий элемент меньше удаляемого*/
    {
        previous = node;                                /*запоминаем этот элемент*/
        node->right = remove(node->right, key);         /*идем впрао*/
    }
    else if (key == node->information)                  /*если добрались до удаляемого элемента*/
    {
        Node<T>* left = node->left;                     /*для удобства обозначаем правый и левый элементы поддерева*/
        Node<T>* right = node->right;

        if (left == nullptr && right == nullptr)        /*если после удаленного элемента ничего нет*/
        {
            delete node;
            return nullptr;
        }
        else if (left == nullptr && right != nullptr)   /*если слева ничего нет, а слева есть*/
        {
            delete node;
            previous->left = right;                     /*то связываем предыдущий от удаляемого элемента элемент с тем, что находится после*/
            return balance(right);                      /*балансируем этот узел*/
        }
        else if (left != nullptr && right == nullptr)   /*если справа ничего нет, а слева есть*/
        {
            delete node;
            previous->right = left;                     /*то связываем предыдущий от удаляемого элемента элемент с тем, что находится после*/
            return balance(left);                       /*балансируем узел*/
        }
        else                                            /*если и справа, и слева что-то есть*/
        {
            Node<T>* max = find_max(left);                      /*ищем максисальный элемент в левом поддереве*/
            Node<T>* prev_max= previous_max(left, max);         /*ищем значение, которое находится перед максимальным элементом*/
            if (prev_max->information == node->information)     /*если предыдущий элемент и есть максимальный элемент в левом поддереве*/
            {
                node->information = max->information;           /*заменяем удаляемый элемент на максимальный элемент*/
                node->left = max->left;                         /*связываем элемент с тем, что находится после максимума*/
                delete max;                                     /*удаляем максимальный элемент*/
                balance(prev_max);                              /*балансируем*/
            }
            else
            {
                node->information = max->information;           /*заменяем удаляемый элемент на максимальный*/
                if (max->right != nullptr)                      /*если справа от максимального элемента что-то есть*/
                { 
                    prev_max->right = max->right;               /*связываем элемент с тем, что находится справа от масимального элемента*/
                    prev_max->right->left = max->left;          /*следом идем то, что слева от макимального элемента*/
                    delete max;                                 /*удаляем максимальный элемент*/
                    balance(prev_max);                          /*балансируем узел*/
                }
                else                                            /*если мправа от максимального элемента ничего нет*/
                {
                    prev_max->right = max->left;                /*сразу связываем элемент с тем, что находится слева от максимума*/
                    delete max;                                 /*удаляем максимальный узел*/
                    balance(prev_max);                          /*балансируем узел*/
                }
            }
        }
    }
    return balance(node);                                       /*балансируем оставшиеся узлы*/
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

Node<int>* tree = new Node<int>;

int main()
{

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
            int element;
            std::cout << "The element you want to create on the AVL-tree: ";
            std::cin >> element;
            constructor(tree, element);
            std::cout << "Constructing was finished successfully \n";
        }
        else if (command == 2)
        {
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
            print(tree);
        else if (command == 5) {
            destructor(tree);
            std::cout << "AVL-tree is empty \n";
        }
        
    } while (command != 6);

    std::cout << "That's all, goodbye!";



    return 0;
}
