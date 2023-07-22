/* Keabetswe Madumo u20438614*/

#ifndef DFA_H
#define DFA_H
#include<string>
#include<iostream>
#include "NFA.h"
#include "node.h"

using namespace std;


class DFA
{
    private:
        NFA* nonAuto;
        node* start;
        node* trap;
        int Name=0;
        vector<node*> nodes;
        vector<string> input;
        bool deadUsed = false;
    public:
        DFA(NFA* automata);
        void convert();
        void table();
        void display();
        void displayD();
        vector<string> returnInput();
        bool setDone();
        vector<node*> getNodes();
        vector<node*> addvec(vector<node*> into, vector<node*> adding);
        string makeName(vector<node*> vec);
        bool checkDuplicate(vector<transition*> list);
        node* addNewNode(string name, string name2, vector<node*> set);
        string nodeName();
        node* compare(vector<node*> f, node* l);
        vector<transition*> removeSingleDuplicate(vector<transition*> list);
        void acceptStates();
        void removeDead();
};


#endif