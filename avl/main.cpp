//
//  main.cpp
//  avl
//
//  Created by Егор on 07.05.2024.
//

#include "Header.h"

#include <iostream>
#include <cassert>
#include <utility>
#include <cstdint>
#include <string>
#include <stack>
#include <exception>
#include <stdexcept>
#include <iterator>
// балансировка в левую сторону
void BinarySearchTree::Node::small_left_rotation() {
    std::pair<Key, Value> copy = right->keyValuePair;
    right->keyValuePair = keyValuePair;
    keyValuePair = copy;
    Node *copy_right = right;
    right = right->right;
    if (right != nullptr) {
        right->parent = this;
    }
    copy_right->right = copy_right->left;
    copy_right->left = left;
    if (left != nullptr) {
        left->parent = copy_right;
    }
    left = copy_right;
    left->parent = this;

    update_height();
}

void BinarySearchTree::Node::small_right_rotation() {
    std::pair<Key, Value> copy = left->keyValuePair;
    left->keyValuePair = keyValuePair;
    keyValuePair = copy;
    Node *copy_left = left;
    left = left->left;
    if (left != nullptr) {
        left->parent = this;
    }
    copy_left->left = copy_left->right;
    copy_left->right = right;
    if (right != nullptr) {
        right->parent = copy_left;
    }
    right = copy_left;
    right->parent = this;

    update_height();

}

void BinarySearchTree::Node::big_left_rotation() {
    right->small_right_rotation();
    small_left_rotation();
}

void BinarySearchTree::Node::big_right_rotation() {
    left->small_left_rotation();
    small_right_rotation();
}

void BinarySearchTree::Node::update_height() {
    m_height = 1 + std::max(
            left == nullptr ? -1 : left->m_height,
            right == nullptr ? -1 : right->m_height);
    if (left != nullptr) {
        left->m_height = 1 + std::max(
                left->left == nullptr ? 0 : left->left->m_height,
                left->right == nullptr ? 0 : left->right->m_height
        );
    }
    if (right != nullptr) {
        right->m_height = 1 + std::max(
                right->left == nullptr ? 0 : right->left->m_height,
                right->right == nullptr ? 0 : right->right->m_height
        );
    }
}


void BinarySearchTree::Node::balance() {
    update_height();

    int left_height = left == nullptr ? 0 : left->m_height;
    int right_height = right == nullptr ? 0 : right->m_height;
    if (left_height - right_height > 1) {
        left->update_height();
        int left_left_height = left->left == nullptr ? 0 : left->left->m_height;
        int left_right_height = left->right == nullptr ? 0 : left->right->m_height;
        if (left_left_height - left_right_height > 0) {
            small_right_rotation();
        } else {
            big_right_rotation();
        }
    } else if (right_height - left_height > 1) {
        right->update_height();
        int right_left_height = right->left == nullptr ? 0 : right->left->m_height;
        int right_right_height = right->right == nullptr ? 0 : right->right->m_height;
        if (right_right_height - right_left_height > 0) {
            small_left_rotation();
        } else {
            big_left_rotation();
        }
    }
}


//Конструктор копирования
BinarySearchTree::BinarySearchTree(const BinarySearchTree& other){
  _size = other._size;
  if (other._root != nullptr){
    _root = new Node(*other._root);
  }
  else{
    _root = nullptr;
  }
}

//Оператор присваивания копированием
//Оператор присваивания копированием
BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        _size = other._size; //todo по-другому
        if (other._root != nullptr) { //Разменование опасно
            _root = new Node(*other._root);
        } else {
            _root = nullptr;
        }
    }
    return *this;
}

//конструктор перемещения
BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept{
  _root = other._root;
  _size = other._size;
  other._root = nullptr;
  other._size = 0;
}

//оператор присваивания перемещением
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this != &other) {
        _size = other._size;
        _root = other._root;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

//Деструктор
BinarySearchTree::~BinarySearchTree(){
  delete this->_root;
}

//! Найти первый элемент в дереве, равный ключу key
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *current = _root; //указатель на текущий узел

    while (current != nullptr) { //пока не дойдем до конца дерева

        if (key > current->keyValuePair.first && !current->right->flag) {
            //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (key < current->keyValuePair.first) {
            //если key меньше, чем у текущего узла, перейдем влево
            current = current->left;
        } else if (key == current->keyValuePair.first) { //если ключи равны
            return ConstIterator(current);
        } else {
            return cend();
        } //если ключа не найдено, вернуть cend()
    }
    return ConstIterator(nullptr);
}

//! Найти первый элемент в дереве, равный ключу key
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *current = _root; //указатель на текущий узел

    while (current != nullptr) { //пока не дойдем до конца дерева

        if (current->right && key > current->keyValuePair.first && !current->right->flag) {
            //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (current->left && key < current->keyValuePair.first) {
            //если key меньше, чем у
            //текущего узла, перейдем влево
            current = current->left;
        } else if (key == current->keyValuePair.first) { //если ключи равны
            return Iterator(current);
        } else
            return end(); //если ключа не найдено, вернуть end()
    }
    return end();
}

 //! Получить размер дерева
size_t BinarySearchTree::size() const{
  return _size;
}
    
//! Получить итератор на элемент с наименьшим ключем в дереве
BinarySearchTree::ConstIterator BinarySearchTree::min() const{
  Node *node = _root;
  while (node->left != nullptr){
    node = node->left;
  }
  return ConstIterator(node);
}

//! Получить итератор на элемент с наибольшим ключем в дереве
BinarySearchTree::ConstIterator BinarySearchTree::max() const {
  Node *node = _root;
  while (node && node->right && !node->right->flag) {
    node = node->right;
  }
  return ConstIterator(node);
}

//! Получить итератор на элемент с ключем key с наименьшим значением
/*BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    ConstIterator it = cbegin();
    ConstIterator minIt = cend();

    while (it != cend()) {
        if (it->first == key) {
            if (minIt == cend() || it->second < minIt->second) {
                minIt = it;
            }
        } else if (it->first > key) {
            break;
        }
        ++it;
    }

    return minIt;
}
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
Node* node = _root;
Node* result = nullptr;

while (node != nullptr) {
  if (node->keyValuePair.first <= key) {
    result = node;
    node = node->right;
  } else {
    node = node->left;
  }
}
  return ConstIterator(result);
}*/

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
  Node *current = _root; //указатель на текущий узел
  Node *res = nullptr;
  Value min;
  int f = 1; //флаг для первичной инициализации min

  while (current != nullptr) //пока не дойдем до конца дерева
  {
    if (key > current->keyValuePair.first &&
        !current->right
             ->flag) //если key больше, чем у текущего узла, перейдем вправо
    {
      current = current->right;
    } else if (key < current->keyValuePair.first) //если key меньше, чем у текущего
    {
      current = current->left;
    } else //если ключи равны
    {
      if (current->keyValuePair.second < min || f) //если значение текущего меньше min или min не проиницилизировано
      {
        min = current->keyValuePair.second; //обновляем min
        res = current; //обновляем возвращаемый узел
        f = 0;         //обнуляем флаг
      }
      current = current->right; //переходим в правую ветку, т.к. узел с
                                //одинаковым ключом уходит вправо
    }
  }
  return ConstIterator(res);
}

//! Получить итератор на элемент с ключем key с наибольшим значением
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
  
 Node *current = _root; //указатель на текущий узел
  Node *res = nullptr;
  Value max;
  int f = 1; //флаг для первичной инициализации max

  while (current != nullptr) //пока не дойдем до конца дерева
  {
    if (key > current->keyValuePair.first &&
        !current->right
             ->flag) //если key больше, чем у текущего узла, перейдем вправо
    {
      current = current->right;
    } else if (key <
               current->keyValuePair.first) //если key меньше, чем у текущего
                                            //узла, перейдем влево
    {
      current = current->left;
    } else //если ключи равны
    {
      if (current->keyValuePair.second > max ||f) //если значение текущего меньше max или max не проиницилизировано
      {
        max = current->keyValuePair.second; //обновляем max
        res = current; //обновляем возвращаемый узел
        f = 0;         //обнуляем флаг
      }
      current = current->right; //переходим в правую ветку, т.к. узел с
                                //одинаковым ключом уходит вправо
    }
  }
  return ConstIterator(res);
}

 //! Получить итератор на первый элемент дерева (элемент с наименьшим key)
BinarySearchTree::Iterator BinarySearchTree::begin(){
  Node* node = _root;
  if (node == nullptr){
    return Iterator(nullptr);
  }
  while (node->left != nullptr){
    node = node->left;
  }
  return Iterator(node);
}

//! Получить итератор на элемент, следующий за последним элементом дерева
BinarySearchTree::Iterator BinarySearchTree::end(){
  Node* node = _root;
  if (node == nullptr){
    return Iterator(nullptr);
  }
  while (node->right != nullptr){
    node = node->right;
  }
  return Iterator(node);
}

//! Получить константный итератор на начало
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const{
  Node* node = _root;
  if (node == nullptr){
    return ConstIterator(nullptr);
  }
  while (node->left != nullptr){
    node = node->left;
  }
  return ConstIterator(node);
}

 //! Получить константный итератор на конец
BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node *current = _root;
    while (current && current->right) {
        current = current->right;
    }
    return ConstIterator(current);
}

//----- Node -----

//! Конструктор с параметрами
BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right, bool flag)
        : keyValuePair(std::make_pair(key, value)), parent(parent), left(left), right(right), flag(flag) {}

//! Конструктор копирования
BinarySearchTree::Node::Node(const Node& other){
  keyValuePair = other.keyValuePair;
  parent = other.parent;
  left = other.left;
  right = other.right;
}

//! Оператор сравнения ==
bool BinarySearchTree::Node::operator==(const Node& other) const{
  if (keyValuePair == other.keyValuePair && parent == other.parent && left == other.left && right == other.right){
    return true;
  }
  return false;
}

    //! \brief Итератор бинарного дерева поиска

BinarySearchTree::Iterator::Iterator(Node *node){
  _node = node;
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*(){
  return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const{
  return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->(){
  return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const{
  return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(){
  if (_node == nullptr){
    return Iterator(nullptr);
  }
  if (_node->right != nullptr){
    _node = _node->right;
    while(_node->left != nullptr){
      _node = _node->left;
    }
  }
  else{
    while(_node->parent != nullptr && _node == _node->parent->right){
      _node = _node->parent;
    }
    _node = _node->parent;
  }
  return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int){
  BinarySearchTree::Iterator *tmp = this;
  ++(*this);
  return *tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(){
  if (_node == nullptr){
    return Iterator(nullptr);
  }
  if (_node->left != nullptr){
    _node = _node->left;
    while(_node->right != nullptr){
      _node = _node->right;
    }
  }
  else{
    while(_node->parent != nullptr && _node == _node->parent->left){
      _node = _node->parent;
    }
    _node = _node->parent;
  }
  return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int){
  BinarySearchTree::Iterator *tmp = this;
  --(*this);
  return *tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

    //! Константный итератор бинарного дерева поиска
BinarySearchTree::ConstIterator::ConstIterator(const Node* node) {
    _node = node;
}
const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    return (*_node).keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }
    else {
        while (_node->parent != nullptr && _node == _node->parent->right) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    BinarySearchTree::ConstIterator t(*this);
    ++(*this);
    return t;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    }
    else {
        while (_node->parent != nullptr && _node == _node->parent->left) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    BinarySearchTree::ConstIterator p = *this;
    --(*this);
    return p;
}
bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
    return _node == other._node;
}
bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
    return !(*this == other);
}

      //equalRange
std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
   Iterator it_start = find(key);
    if (it_start == end()) {
        return std::make_pair(it_start, it_start);
    }
    Iterator it_tmp = it_start;
    Iterator it_end = it_start;
    it_tmp--;
    Iterator begin_it = begin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it_start--;
        it_tmp--;
    }
    it_tmp = it_end;
    it_tmp++;
    Iterator end_it = end();
    while (it_tmp->first == key && it_tmp != end_it) {
        it_end++;
        it_tmp++;
    }
    it_end++;
    return std::make_pair(it_start, it_end);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator begin = find(key);
    ConstIterator end = begin;
    if (begin != cend()) {
        while (end != cend() && end->first == key) {
            ++end;
        }
    }
    return std::make_pair(begin, end);
}

//-----------------INSERT-----------------

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
  if (key < keyValuePair.first) //если в левый узел
  {
    if (left) //если уже есть левый, то вызвать от него рекурсивно вставку
    {
      left->insert(key, value);
    } else //если нету, создать новый узел и указать, что он левый для текущего
    {
      Node *node = new Node(key, value, this, nullptr, nullptr);
      this->left = node;
    }
  }
  if (key >= keyValuePair.first) //если в правый узел
  {
    if (right) //если уже есть правый, то вызвать от него рекурсивно вставку
               //(при этом, чтобы правый был не nil)
    {
      right->insert(key, value);
    } else //если нету, создать новый узел и указать, что он правый для текущего
    {
      Node *node = new Node(key, value, this, nullptr, nullptr);
      this->right = node;
    }
  }
  balance();
}
void BinarySearchTree::insert(const Key &key, const Value &value) {
  if (_root) {
    delete_nill();
    _root->insert(key, value);
  } else {
    _root = new Node(key, value);
  }
  insert_nill();
  _size++;
}

//-----------------ERASE-----------------

  //если у ноды один ребенок или она чайлдфри
 //! Удалить все элементы с ключем key

BinarySearchTree::Node *BinarySearchTree::Node::erase_node(Node *node,
                                                           const Key &key) {
  if (node == nullptr)
    return node;

  if (key < node->keyValuePair.first) //если ключ меньше
  {
    node->left = erase_node(node->left, key); //рекурсивно идем влево
    node->balance(); // балансируем после удаления
    return node;
  } else if (key > node->keyValuePair.first) //если ключ больше
  {
    node->right = erase_node(node->right, key); //рекурсивно идем вправо
    node->balance(); // балансируем после удаления
    return node;
  }

  //если у ноды один ребенок или она чайлдфри
  if (node->left == nullptr) {
    Node *temp = node->right;
    delete node;
    return temp;
  } else if (node->right == nullptr) {
    Node *temp = node->left;
    delete node;
    return temp;
  }

  //если два ребенка. Ищем наименьший элемент в правом поддереве (то есть самый
  //левый)
  Node *temp = node->right; //правый элемент удаляемого
  Node *parent = node;      //родитель этого правого

  while (temp->left != nullptr) {
    parent = temp;
    temp = temp->left; //идем в самый левый узел
  }

  node->keyValuePair = temp->keyValuePair;

  if (parent->left == temp)
    parent->left = temp->right;
  else
    parent->right = temp->right;

  delete temp;
  node->balance(); // балансируем после удаления
  return node;
}

void BinarySearchTree::erase(const Key &key) {
  if (_root) {
    delete_nill();
    _root = _root->erase_node(_root, key);
    _size--;
    insert_nill();
  }
  // Пока в дереве есть ключи key, удаляем их
  Iterator f = find(key);
  if (f != end()) {
    erase(key);
  }
}
void BinarySearchTree::delete_nill() {
  Node *current = _root;

  if (!current)
    return;

  while (current->right) {
    current = current->right;
  }

  if (current != nullptr && current->flag) {
    current->parent->right = nullptr;
    delete current;
  }
}

void BinarySearchTree::insert_nill() {
  if (_root) {
    Node *nil = new Node(0, 0);
    nil->flag = true;

    Node *current = _root;

    while (current->right) {
      current = current->right;
    }

    if (!current->flag) {
      current->right = nil;
      nil->parent = current;
    }
  }
}

void BinarySearchTree::Node::output_node_tree(const std::string &prefix, const BinarySearchTree::Node *node, bool isLeft) const {
    if (node != nullptr && !node->flag) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");

        std::cout << node->keyValuePair.first << std::endl;

        output_node_tree(prefix + (isLeft ? "│   " : "    "), node->left, node->left != nullptr);
        output_node_tree(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}


void BinarySearchTree::output_tree() {
    if (_root != nullptr) {
        _root->output_node_tree("", _root, false);
    } else {
        std::cout << "Tree is empty." << std::endl;
    }
}


int main() {
    BinarySearchTree tree;

    while (true) {
        std::cout << "1. Insert" << std::endl;
        std::cout << "2. Erase" << std::endl;
        std::cout << "3. Find" << std::endl;
        std::cout << "4. Equal Range" << std::endl;
        std::cout << "5. Min" << std::endl;
        std::cout << "6. Max" << std::endl;
        std::cout << "7. Print Tree" << std::endl;
        std::cout << "8. Exit" << std::endl;

        int choice;
        std::cout << "Choose an operation: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int key;
                int value;
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter value: ";
                std::cin >> value;
                tree.insert(key, value);
                break;
            }
            case 2: {
                int key;
                std::cout << "Enter key to erase: ";
                std::cin >> key;
                tree.erase(key);
                break;
            }
            case 3: {
                int key;
                std::cout << "Enter key to find: ";
                std::cin >> key;
                auto it = tree.find(key);
                if (it == tree.end()) {
                    std::cout << "Key not found" << std::endl;
                } else {
                    std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
                }
                break;
            }
            case 4: {
                int key;
                std::cout << "Enter key to find equal range: ";
                std::cin >> key;
                auto range = tree.equalRange(key);
                std::cout << "Equal range:" << std::endl;
                for (auto it = range.first; it != range.second; ++it) {
                    std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
                }
                break;
            }
            case 5: {
                int key;
                std::cout << "Enter key to find min: ";
                std::cin >> key;
                auto it = tree.min(key);
                if (it == tree.cend()) {
                    std::cout << "Key not found" << std::endl;
                } else {
                    std::cout << "Min key: " << it->first << ", Value: " << it->second << std::endl;
                }
                break;
            }
            case 6: {
                int key;
                std::cout << "Enter key to find max: ";
                std::cin >> key;
                auto it = tree.max(key);
                if (it == tree.cend()) {
                    std::cout << "Key not found" << std::endl;
                } else {
                    std::cout << "Max key: " << it->first << ", Value: " << it->second << std::endl;
                }
                break;
            }
            case 7: {
                std::cout << "AVL Tree :" << std::endl;
                tree.output_tree();
                break;
            }
            case 8: {
                return 0;
            }
            default: {
                std::cout << "Invalid choice" << std::endl;
                break;
            }
        }
    }
}
