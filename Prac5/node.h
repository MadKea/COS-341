/* Keabetswe Madumo u20438614*/

#ifndef NODE_H
#define NODE_H
#include<string>
#include<iostream>
#include<vector>
#include<fstream>

#include "table.h"
using namespace std;


class node
{
    private:
            string name;
            bool terminal;
            int id;
            vector<node*> nodes;
            string value;
            int scope;
            static int scopeIndex;
            static string Errors;
            string ErrorVar;
            bool called;
            int parentScope;
            string scopeName;
            bool checkedCrawl;
            bool checkedTrans;
            string lineNumber;
    public:
        node(string name, bool terminal, int id);
        ~node();
        void addNode(node* n);
        void display();
        string parse(string tab, string input);
        bool epsilonRemove();
        void noKids();
        string compress(string s);
        void compressing();
        void scoping();
        bool siblingNames();
        string getProcName();
        bool checkNames(string name);
        bool childNames();
        bool nephewNames();
        bool gatherNephews(node* main);
        void checkingCalls(node* root);
        vector<string> getCalls();
        void scopeI(int scopes);
        string getSisters(int i, string a);
        void checkSister(int scp, string name);
        string deadCode(string w);
        void tableO(table* Hash);
        string returnErrors();
        void fixID(node* root);
        void alter(string value, int id);
        //Prac4 Rules
        void crawl(node* root, table* Hash);
        void AssignRule(table* Hash);
        string numExprRule(table* Hash);
        string boolExprRule(table* Hash);
        void OutputRule(table* Hash);
        void InputRule(table* Hash);
        void LoopBranchRule(table* Hash);
        void Calling(node* root, table* Hash, string find, int Kscope, int Sscope);
        void LoopErrors(node* root, table* copy, table* orig);
        void BranchErrors(node* root, table* copy, table* orig);
        void BranchIE(node* root, table* Hash, table* copyt, table* copye);
        string getName();
        string getValue();
        vector<node*> getNodes();
        bool getTransCheck();
        void setTransCheck();
        string getScopeName();
        int getParentScope();
        string getLineNumber();
        void setLineNumber(string line);

};

#endif