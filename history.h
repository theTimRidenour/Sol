#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>

class History {
    public:
        int cardIndex;
        int currentStack;
        int previousStack;
        bool prevFaceUp;

        History();

        History(int cardIndex, int currentStack, int previousStack, bool prevFaceUp) {
            this->cardIndex = cardIndex;
            this->currentStack = currentStack;
            this->previousStack = previousStack;
            this->prevFaceUp = prevFaceUp;
            this->next = NULL;
        }
    History *next;
};

void push(struct History** headRef, int cardIndex, int currentStack, int previousStack, bool prevFaceUp) {
    struct History *newNode = (struct History*) malloc(sizeof(struct History));
    newNode->cardIndex = cardIndex;
    newNode->currentStack = currentStack;
    newNode->previousStack = previousStack;
    newNode->prevFaceUp = prevFaceUp;
    newNode->next = ( *headRef );
    ( *headRef ) = newNode;
};

void pop(struct History **headRef) {
    ( *headRef ) = ( *headRef )->next;
};

#endif