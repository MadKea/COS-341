/* Keabetswe Madumo u20438614*/
#ifndef node_H
#define node_H
#include<string>
#include<iostream>
#include <vector>
#include "transition.h"

class transition;
using namespace std;
class node
{
    private:
        string name;
        string DFAname;
        int dfaSize;
        int team;
        vector<transition*> transitions;
        vector<node*> Eclosure;
        vector<node*> nfaSet;
        bool visited;
        bool loop = false;
        bool accept;
        bool dead;
        bool start;
        bool addedToDfa = false;
    public:
        
        node(string name);
        node(string s1, string s2, vector<node*> set);
        void addTrans(transition* a);
        vector<node*> getSet();
        vector<node*> getClosure();
        void displayTransD();
        void setTeam(int a);
        int getTeam();
        void displayTrans();
        void setAccept();
        bool getAccept();
        void setStart();
        bool getDead();
        void setDead();
        bool getStart();
        void setTrans(vector<transition*> list);
        void addClose(node* e);
        string display();
        string displayD();
        string getName();
        string getDFAname();
        void setClosure(node* e, int cheat);
        bool getaddedToDFA();
        void setAddedToDFA(bool nd);
        vector<transition*> getTran();
        ~node();

};


#endif