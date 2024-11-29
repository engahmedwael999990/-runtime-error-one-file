#pragma once
#include<iostream>
using namespace std;
#include"LinkedQueue.h"


template <typename T>
class UpdatedQueue : public LinkedQueue<T>
{
public:
    UpdatedQueue() {}

    bool cancel(const T& entry)
    {

        Node<T>*& frontPtr = LinkedQueue<T>::frontPtr;
        Node<T>*& backPtr = LinkedQueue<T>::backPtr;


        if (frontPtr == nullptr)
            return false;


        T frontEntry;
        if (LinkedQueue<T>::peek(frontEntry) && frontEntry == entry)
        {
            LinkedQueue<T>::dequeue(frontEntry);
            return true;
        }


        Node<T>* prevNode = frontPtr;
        Node<T>* currNode = frontPtr->getNext();

        while (currNode)
        {
            if (currNode->getItem() == entry)
            {

                prevNode->setNext(currNode->getNext());


                if (currNode == backPtr)
                    backPtr = prevNode;

                delete currNode;
                return true;
            }
            prevNode = currNode;
            currNode = currNode->getNext();
        }

        return false;
    }


    ~UpdatedQueue() {}

};
