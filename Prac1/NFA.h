/* Keabetswe Madumo u20438614*/
#ifndef NFA_H
#define NFA_H
#include<string>
#include<iostream>
#include "node.h"
#include "transition.h"
#include <vector>


using namespace std;


class NFA
{
   
    private:
        string s;
        node *start;
        int Name = 1;
        vector<string> input;
        vector<node*> nodes;
    public:
        NFA(string s);
        void makeNFA();
        int route(string str);
        node* addNewNode(string name);
        node* func(node *str, string word);
        string nodeName();
        void makeClosures();
        void display();
        string reformatReg(string str);
        vector<string> returnInput();
        vector<node*> getNodes();
        void addInput(string in);
        node* getStart();
    
};


#endif