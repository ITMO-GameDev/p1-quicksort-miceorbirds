#pragma once

#include <algorithm>

template<typename K, typename V>
Dictionary<K, V>::Dictionary() {
    root = nullptr;
    size = 0;
}

template <typename K, typename V>
int Dictionary<K, V>::height(Node<K, V>* node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::newNode(const K& key, const V& value)
{
    Node<K, V>* node = new Node<K, V>();
    node->key = key;
    node->value = value;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;

    return node;
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::rightRotate(Node<K, V>* node)
{
    Node<K, V>* initLeft = node->left;
    Node<K, V>* leftsRight = initLeft->right;

    initLeft->right = node;
    node->left = leftsRight;

    node->height = std::max(height(node->left), height(node->right)) + 1;
    initLeft->height = std::max(height(initLeft->left), height(initLeft->right)) + 1;

    return initLeft;
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::leftRotate(Node<K, V>* node)
{
    Node<K, V>* initRight = node->right;
    Node<K, V>* rightsLeft = initRight->left;

    initRight->left = node;
    node->right = rightsLeft;

    node->height = std::max(height(node->left), height(node->right)) + 1;
    initRight->height = std::max(height(initRight->left), height(initRight->right)) + 1;

    return initRight;
}

template <typename K, typename V>
int Dictionary<K, V>::getBalance(Node<K, V>* N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::insert(Node<K, V>* node, const K& key, const V& value)
{
    if (node == nullptr) {
        size++;
        return newNode(key, value);
    }

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else {
        node->value = value;
        return node;
    }


    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::minValueNode(Node<K, V>* node)
{
    Node<K, V>* current = node;

    while (current->left != nullptr)
        current = current->left;

    return current;
}

template <typename K, typename V>
Node<K, V>* Dictionary<K, V>::deleteNode(Node<K, V>* root, const K& key)
{
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node<K, V>* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else {
                *root = *temp;
            }

            free(temp);
        }
        else {
            Node<K, V>* temp = minValueNode(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right, temp->key);
        }

        size--;
    }

    if (root == nullptr)
        return root;

    root->height = 1 + std::max(height(root->left), height(root->right));

    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template <typename K, typename V>
void Dictionary<K, V>::preOrder(Node<K, V>* root)
{
    if (root != nullptr) {
        preOrder(root->left);
        preOrder(root->right);
    }
}



template<class K, class V>
void Dictionary<K, V>::put(const K& key, const V& value) {
    root = insert(root, key, value);
}

template<typename K, typename V>
void Dictionary<K, V>::remove(const K& key) {
    root = deleteNode(root, key);
}

template<typename K, typename V>
int Dictionary<K, V>::getSize() const {
    return size;
}

template<typename K, typename V>
V* Dictionary<K, V>::get(const K& key) {
    Node<K, V>* node = root;
    while (node != NULL) {
        if (node->key == key) {
            return &node->value;
        }

        if (key < node->key) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }

    return nullptr;
}

template<typename K, typename V>
const V& Dictionary<K, V>::operator[](const K& key) const {
    V* value = get(key);
    if (value == nullptr) {
        if (std::is_class<V>::value) {
            put(key, V());
        }
        else {
            put(key, NULL);
        }

        return *get(key);
    }
    else {
        return *value;
    }
}

template<typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key) {
    V* value = get(key);
    if (value == nullptr) {
        if (std::is_class<V>::value) {
            put(key, V());
        }
        else {
            put(key, NULL);
        }

        return *get(key);
    }
    else {
        return *value;
    }
}

template<typename K, typename V>
bool Dictionary<K, V>::contains(const K& key) {
    return get(key) != nullptr;
}

template<typename K, typename V>
const typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() const {
    return Dictionary::Iterator(*this);
}

template<typename K, typename V>
typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() {
    return Dictionary::Iterator(*this);
}

template<typename K, typename V>
Dictionary<K, V>::~Dictionary() {
}

template<typename K, typename V>
Dictionary<K, V>::Iterator::Iterator(Dictionary<K, V>& dictionary) {
    stackNext_ = std::stack<Node<K, V>*>();
    stackPrevious_ = std::stack<Node<K, V>*>();

    toLeft(dictionary.root);
}

template<typename K, typename V>
const V& Dictionary<K, V>::Iterator::get() const {
    Node<K, V>* node = stackNext_.top();

    return node->value;
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::set(const V& value) {
    Node<K, V>* node = stackNext_.top();
    node->value = value;
}

template<typename K, typename V>
const K& Dictionary<K, V>::Iterator::key() const {
    Node<K, V>* node = stackNext_.top();

    return node->key;
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::next() {
    Node<K, V>* node = stackNext_.top();
    stackNext_.pop();
    stackPrevious_.push(node);
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::prev() {
    Node<K, V>* node = stackPrevious_.top();
    stackPrevious_.pop();
    stackNext_.push(node);
}

template<typename K, typename V>
bool Dictionary<K, V>::Iterator::hasNext() const {
    return stackNext_.size() > 1;
}

template<typename K, typename V>
bool Dictionary<K, V>::Iterator::hasPrev() const {
    return !stackPrevious_.empty();
}

template<typename K, typename V>
void Dictionary<K, V>::Iterator::toLeft(Node<K, V>* node) {
    if (node != nullptr) {
        toLeft(node->right);
        toLeft(node->left);

        stackNext_.push(node);
    }
}