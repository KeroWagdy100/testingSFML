#pragma once
#include <stdexcept>
#include <iostream>

template <class T>
class Node
{
public:
    T val;
    Node<T> *next = nullptr;
    Node<T> *prev = nullptr;
    Node(T item): 
        val(item){}
private:
};

template <class T>
class LinkedList
{
public:
    LinkedList();
    void append(T item);
    void prepend(T item);
    void insertAt(T item, int index);
    T remove(T item);
    T removeAt(int index);
    void clear();
    T get(int index) const;
    T& operator[](int index);
    int find(T item);
    int findAny(const T* const item, const int SIZE);
    int length() const;
    void printAll() const;           // for debugging reasons
    void printAll_inversely() const; // for debugging reasons
    ~LinkedList();
    
    template <class I>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<I>& ll);

private:
    Node<T>* Iget(int index) const;
    Node<T> *head = nullptr;
    Node<T> *tail = nullptr;
    size_t m_length = 0;
};

template <class T>
LinkedList<T>::LinkedList()
{}


template <class T>
void LinkedList<T>::append(T item)
{
    Node<T> *new_node = new Node<T>(item);
    m_length++;
    // in case of empty list
    if (head == nullptr)
    {
        head = tail = new_node;
        return;
    }
    tail->next = new_node;
    new_node->prev = tail;
    tail = new_node;
}

template <class T>
void LinkedList<T>::prepend(T item)
{
    Node<T> *new_node = new Node<T>(item);
    m_length++;
    if (head == nullptr)
    {
        head = tail = new_node;
        return;
    }
    head->prev = new_node;
    new_node->next = head;
    head = new_node;
}

template <class T>
void LinkedList<T>::insertAt(T item, int index)
{

    if (index >= m_length)
        throw std::invalid_argument("Invalid index given in insertAt() func.");
    Node<T> *curr_node = head;
    for (int i = 0; i < index && curr_node != nullptr; i++)
        curr_node = curr_node->next;

    Node<T> *new_node = new Node<T>(item);
    m_length++;
    new_node->prev = curr_node->prev;
    new_node->prev->next = new_node;
    new_node->next = curr_node;
    curr_node->prev = new_node;
}

template <class T>
T LinkedList<T>::remove(T item)
{
    Node<T> *curr_node = head;
    while (curr_node != nullptr)
    {
        if (curr_node->val == item)
            break;
        curr_node = curr_node->next;
    }
    if (curr_node == nullptr)
        throw std::invalid_argument("Invalid item given. Not found in the list");
    // if removing the head
    if (curr_node->prev == nullptr)
        head = curr_node->next;
    else
        curr_node->prev->next = curr_node->next;

    // if removing the tail
    if (curr_node->next == nullptr)
        tail = curr_node->prev;
    else
        curr_node->next->prev = curr_node->prev;
    curr_node->prev = curr_node->next = nullptr;

    T temp = curr_node->val;
    delete curr_node;
    m_length--;
    return temp;
}

template <class T>
T LinkedList<T>::removeAt(int index)
{
    if (index >= m_length)
        throw std::invalid_argument("Invalid index given in removeAt() func.");
    Node<T> *curr_node = head;
    for (int i = 0; i < index && curr_node != nullptr; i++)
        curr_node = curr_node->next;
    // if removing the head
    if (curr_node->prev == nullptr)
        head = curr_node->next;
    else
        curr_node->prev->next = curr_node->next;

    // if removing the tail
    if (curr_node->next == nullptr)
        tail = curr_node->prev;
    else
        curr_node->next->prev = curr_node->prev;

    curr_node->prev = curr_node->next = nullptr;

    T temp = curr_node->val;
    delete curr_node;
    m_length--;
    return temp;
}


template <class T>
void LinkedList<T>::clear() {
    Node<T> *currNode = head;
    while (currNode != nullptr)
    {
        Node<T> *next = currNode->next;
        currNode->next = nullptr;
        delete currNode;
        currNode = next;
    }
    head = nullptr;
    tail = nullptr;
    m_length = 0;
}

template <class T>
Node<T>* LinkedList<T>::Iget(int index) const {
    if (index >= m_length)
    {
        throw std::invalid_argument("Invalid index given as a parameter\n");
        return nullptr;
    }
    Node<T> *curr_node = head;
    for (int i = 0; i < index && curr_node != nullptr; i++)
        curr_node = curr_node->next;
    return curr_node;
}

template <class T>
T LinkedList<T>::get(int index) const
{
    Node<T>* ptrToNode = Iget(index);
    return (ptrToNode == nullptr ? T() : ptrToNode->val);
}

template <class T>
T& LinkedList<T>::operator[](int index) {
    Node<T>* ptrToNode = Iget(index);
    if (ptrToNode == nullptr)
        throw std::invalid_argument("Error at operator[], index might be out of range.\n");
    return ptrToNode->val;
}

// returns index of first found item
template <class T>
int LinkedList<T>::find(T item) {
    Node<T>* curr_node = head;
    int index = 0;
    while (curr_node != nullptr) {
        if (curr_node->val == item)
            return index;
        curr_node = curr_node->next;
        index++;
    }
    return -1;
}


template <class T>
int LinkedList<T>::findAny(const T* const item, const int SIZE) {
    Node<T>* curr_node = head;
    int index = 0;
    while (curr_node != nullptr) {
        T curr_val = curr_node->val;
        for (int i = 0; i < SIZE; i++)
            if (curr_val == item[i])
                return index;
        curr_node = curr_node->next;
        index++;
    }
    return -1;
}

template <class T> // for debugging reasons
void LinkedList<T>::printAll() const
{
    // Node<T> *curr_node = head;
    // std::cout << "[";
    // while (curr_node != nullptr)
    // {
    //     std::cout << " -> " << curr_node->val;
    //     curr_node = curr_node->next;
    // }
    // std::cout << "]";
    // // std::cout << "\n";
    std::cout << *this;
}

template <class I>
std::ostream& operator<<(std::ostream& out, const LinkedList<I>& ll) {
    Node<I> *curr_node = ll.head;
    out << "[";
    while (curr_node != nullptr)
    {
        std::cout << curr_node->val << (curr_node->next != nullptr ? ", " : "");
        curr_node = curr_node->next;
    }
    out << "]";
    return out;
}

template <class T> // for debugging reasons
void LinkedList<T>::printAll_inversely() const
{
    Node<T> *curr_node = tail;
    while (curr_node != nullptr)
    {
        std::cout << " <- " << curr_node->val;
        curr_node = curr_node->prev;
    }
    std::cout << "\n";
}

template <class T>
int LinkedList<T>::length() const
{
    // int len{0};
    // Node<T> *curr_node = head;
    // while (curr_node != nullptr)
    // {
    //     len++;
    //     curr_node = curr_node->next;
    // }
    // return len;
    return m_length;
}

template <class T>
LinkedList<T>::~LinkedList()
{
    clear();
}