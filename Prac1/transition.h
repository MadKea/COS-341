/* Keabetswe Madumo u20438614*/
#ifndef TRANSITION_H
#define TRANSITION_H
#include "node.h"
#include<string>
#include<iostream>

class node;
using namespace std;
class transition
{
    private:
        string transitionOn;
        node *from;
        node *to;
    public:
        transition(node* from, node* to, string transOn);
        node* getFrom();
        node* getTo();
        string getTransition();
        string getTransitionOn();
        string getTransitionOnD();
        ~transition();

};

#endif