#pragma once
#include<stack>

template <typename K, typename V>
class Node {
public:
    K key;
    V value;
    Node<K, V>* left;
    Node<K, V>* right;
    int height;
};

template <typename K, typename V>
class Dictionary {

    class Iterator
    {
    public:
        Iterator(Dictionary<K, V>& dictionary);
        const K& key() const;
        const V& get() const;
        void set(const V& value);
        void next();
        void prev();
        bool hasNext() const;
        bool hasPrev() const;

    private:
        void toLeft(Node<K, V>* node);
        std::stack<Node<K, V>*> stackNext_;
        std::stack<Node<K, V>*> stackPrevious_;
    };

protected:
    Node<K, V>* root;
    int size;
    int height(Node<K, V>* node);
public:
    Node<K, V>* newNode(const K& key, const V& value);
    Node<K, V>* rightRotate(Node<K, V>* node);
    Node<K, V>* leftRotate(Node<K, V>* node);
    int getBalance(Node<K, V>* node);
    Node<K, V>* insert(Node<K, V>* node, const K& key, const V& value);
    Node<K, V>* minValueNode(Node<K, V>* node);
    Node<K, V>* deleteNode(Node<K, V>* root, const K& key);
    void preOrder(Node<K, V>* root);
    V* get(const K& key);
    Dictionary();
    ~Dictionary();
    void put(const K& key, const V& value);
    void remove(const K& key);
    bool contains(const K& key);
    int getSize() const;
    Iterator iterator();
    const Iterator iterator() const;
    const V& operator [](const K& key) const;
    V& operator [](const K& key);
};

#include "search.hpp"