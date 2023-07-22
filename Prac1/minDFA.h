/* Keabetswe Madumo u20438614*/
#ifndef MINDFA_H
#define MINDFA_H
#include<string>
#include<iostream>
#include "DFA.h"
#include "node.h"


using namespace std;


class minDFA
{
    private:
        DFA* dautomata;
        int teamCounter = 0;
        int namingNode =0;
        node* start;
        vector<node*> mini;
        vector<string> input;
        vector<transition*> finalTrans;


    public:
        minDFA(DFA* automata);
        void minimise();
        int joinTeam(); 
        string nameNode();
        void setStart();
        int countFinalTeam();
        void createFinalNodes();
        int transitionTeam(string input, int team);
        int getTeamCounter();
        bool teamCompare(node* a, node* b);
        void partition();
        void addingTrans();
        void initialisePartition();
        void display();
        vector<node*> getMini();
};

#endif