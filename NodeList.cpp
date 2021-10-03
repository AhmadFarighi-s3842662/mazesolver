#include "NodeList.h"
#include <iostream>

NodeList::NodeList(){
    this->length = 0;
}

NodeList::~NodeList(){
    for (int i = 0; i < length; ++i) {
        delete nodes[i];
    }
}

NodeList::NodeList(NodeList& other) :
    length(other.length)
{
    for (int i = 0; i < this->length; ++i) {
        nodes[i] = new Node(*other.nodes[i]);
    }
}

int NodeList::getLength(){
    return this->length;
}

void NodeList::addElement(Node* newPos){
    Node* newElement = new Node(*newPos);
    this->nodes[length] = newElement;
    this->length++;
}

Node* NodeList::getNode(int i){
    Node* nodePtr = nullptr;
    for (int index = 0; index < this->length; ++index) {
        if (i == index) {
            nodePtr = nodes[i];
        }
    }
    return nodePtr;
}
