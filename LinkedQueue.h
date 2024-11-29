#pragma once

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

                The Node: item of type T and a "next" pointer
                    -------------
                    | item| next | --->
                    -------------
General Queue case:

                 frontPtr                                     backPtr
                    \                                              /
                     \                                            /
                    -------------     -------------     -------------     -------------
                    | item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
                    -------------     -------------     -------------     -------------

Empty Case:

                 frontPtr     backPtr
                    \         /
                     \       /
                    ---- NULL ------

Single Node Case:
                 frontPtr     backPtr
                    \        /
                     \      /
                    -----------
                    |item| next| -->NULL
                    -----------

*/

#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include <iostream> // For std::cout

template <typename T>
class Node {
private:
    T item; // A data item
    Node<T>* next; // Pointer to next node

public:
    Node();
    Node(const T& r_Item);
    Node(const T& r_Item, Node<T>* nextNodePtr);
    void setItem(const T& r_Item);
    void setNext(Node<T>* nextNodePtr);
    T getItem() const;
    Node<T>* getNext() const;
}; // end Node

template <typename T>
Node<T>::Node() : next(nullptr) {}

template <typename T>
Node<T>::Node(const T& r_Item) : item(r_Item), next(nullptr) {}

template <typename T>
Node<T>::Node(const T& r_Item, Node<T>* nextNodePtr) : item(r_Item), next(nextNodePtr) {}

template <typename T>
void Node<T>::setItem(const T& r_Item) {
    item = r_Item;
}

template <typename T>
void Node<T>::setNext(Node<T>* nextNodePtr) {
    next = nextNodePtr;
}

template <typename T>
T Node<T>::getItem() const {
    return item;
}

template <typename T>
Node<T>* Node<T>::getNext() const {
    return next;
}

template <typename T>
class LinkedQueue {
private:
    Node<T>* backPtr;
    Node<T>* frontPtr;
    int count;
public:
    LinkedQueue();
    bool isEmpty() const;
    bool enqueue(const T& newEntry);
    bool dequeue(T& frntEntry);
    bool peek(T& frntEntry) const;
    int getcount() const; // Corrected
    Node<T>* getHead() const; // Corrected
    void printQueue() const; // New function to print the queue
    ~LinkedQueue();
    // Copy constructor
    LinkedQueue(const LinkedQueue<T>& LQ);
};

template <typename T>
LinkedQueue<T>::LinkedQueue() : backPtr(nullptr), frontPtr(nullptr), count(0) {}

template <typename T>
bool LinkedQueue<T>::isEmpty() const {
    return frontPtr == nullptr;
}

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry) {
    Node<T>* newNodePtr = new Node<T>(newEntry);
    if (isEmpty()) // Special case if this is the first node to insert
        frontPtr = newNodePtr; // The queue is empty
    else
        backPtr->setNext(newNodePtr); // The queue was not empty

    backPtr = newNodePtr; // New node is the last node now
    count++;
    return true;
}

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry) {
    if (isEmpty())
        return false;

    Node<T>* nodeToDeletePtr = frontPtr;
    frntEntry = frontPtr->getItem();
    frontPtr = frontPtr->getNext();
    // Queue is not empty; remove front
    if (nodeToDeletePtr == backPtr) // Special case: last node in the queue
        backPtr = nullptr;

    // Free memory reserved for the dequeued node
    delete nodeToDeletePtr;
    count--;
    return true;
}

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const {
    if (isEmpty())
        return false;

    frntEntry = frontPtr->getItem();
    return true;
}

template <typename T>
int LinkedQueue<T>::getcount() const {
    return count;
}

template <typename T>
Node<T>* LinkedQueue<T>::getHead() const {
    return frontPtr;
}

template <typename T>
void LinkedQueue<T>::printQueue() const {
    Node<T>* currentNode = frontPtr;
    while (currentNode != nullptr) {
        std::cout << currentNode->getItem() << " ";
        currentNode = currentNode->getNext();
    }
    std::cout << std::endl;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue() {
    T temp;
    while (dequeue(temp));
}

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& LQ) : frontPtr(nullptr), backPtr(nullptr), count(0) {
    Node<T>* NodePtr = LQ.frontPtr;
    while (NodePtr) {
        enqueue(NodePtr->getItem());
        NodePtr = NodePtr->getNext();
    }
}

#endif
