#include <iostream>
#include "LinkedList.h"

Node::Node(char value, Node* next) :
    value(value),
    next(next) 
{}

LinkedList::LinkedList() {
    head = nullptr;
}
LinkedList::LinkedList(const LinkedList& other) {

}
LinkedList::~LinkedList() {

}

int LinkedList::size() {
    Node* current = head;
    int count = 0;
    while(current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

char LinkedList::get(int index) {

    int outputValue = 0;
    int count = 0;
    Node* current = head;
    
    if(index < size()) {
        while(count < index) {
            ++count;
            current = current->next;
        }
        outputValue = current->value;
    }
    else {
        // change to a throw statement
        std::cout << "out of range" << std::endl;
    }
    return outputValue;

}

bool LinkedList::get(int index, char& returnValue)  {
   bool error = true;
   int count = 0;
   Node* current = head;
   if (index >= 0 && index < size()) {
      while(count < index) {
         ++count;
         current = current->next;
      }
      returnValue = current->value;
      error = false;
   }
   
   return error;
}

void LinkedList::addFront(char value) {
    Node* toAdd = new Node(value,nullptr);
    if(head != nullptr) {
        toAdd->next = head;
    }
    head = toAdd;   
}

void LinkedList::addBack(char value) {
   Node* toAdd = new Node(value, nullptr);
   if (head == nullptr) {
      head = toAdd;
   } else {
      Node* current = head;
      while(current->next != nullptr) {
         current = current->next;
      }
      current->next = toAdd;
   }
}

void LinkedList::removeBack() {
    if(head != nullptr) {
        if(head->next != nullptr) {
            Node* current = head;
            while(current->next->next != nullptr) {
                current = current->next;
            }
            current->next = nullptr;
        }
        else {
            head = nullptr;
        }
    }
}

void LinkedList::removeFront() {
    Node* current = head;
    head = nullptr;
    head = current->next;
}
void LinkedList::clear() {
    Node* current = head;
    while(current->next != nullptr) {
        current = nullptr;
    }
}


