#include <iostream>
#include "LinkedList.h"

Node::Node(char value, Node* next) :
    value(value),
    next(next) 
{}

LinkedList::LinkedList() {
    this->head = nullptr;
}
LinkedList::LinkedList(const LinkedList& other) {

}
LinkedList::~LinkedList() {
    delete this;
}

int LinkedList::size() {
    Node* current = this->head;
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
   if(&(this->head) == nullptr) {
       return error;
   }
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
    if(this->head != nullptr) {
        toAdd->next = this->head;  
    }
    this->head = toAdd;
}

void LinkedList::addBack(char value) {
   Node* toAdd = new Node(value, nullptr);
   if (this->head == nullptr) {
      this->head = toAdd;
   } else {
      Node* current = this->head;
      while(current->next != nullptr) {
         current = current->next;
      }
      current->next = toAdd;
   }
}

void LinkedList::removeBack() {
    if(this->head != nullptr) {
        if(this->head->next != nullptr) {
            Node* current = this->head;
            while(current->next->next != nullptr) {
                current = current->next;
            }
            current->next = nullptr;
        }
        else {
            this->head = nullptr;
        }
    }
}

void LinkedList::removeFront() {
    Node* current = this->head;
    this->head = nullptr;
    this->head = current->next;
}
void LinkedList::clear() {
    
    for(int i = 0;i<this->size();++i) {
        this->removeNodeAtIndex(i);
    }
}

void LinkedList::removeNodeAtIndex(int index) {
    Node* current = this->head;
    if(index > 0) {
        for(int i = 0;i < index - 1;++i) {
            current = current->next; 
        }
        Node* temp = current->next;
        current->next = temp->next;
        temp = nullptr;
    }
    else {
        this->removeFront();
    }
}

bool LinkedList::contains(char input){

    Node* current = this->head;
    bool found = false;
    while(!found && current != nullptr) {
        if(current->value == input){
            found = true;
        }
        current = current->next;
    }
    return found;
}

void LinkedList::print() {
    Node* current = this->head;
    while(current != nullptr) {
        std::cout << current->value << ",";
        current = current->next;
    }
}