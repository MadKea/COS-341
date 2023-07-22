/* Keabetswe Madumo u20438614*/

#ifndef NODE_H
#define NODE_H
#include<string>
#include<iostream>
#include<vector>
#include<fstream>

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
            bool called;
            string scopeName;
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
        string table(string hml);
        string returnErrors();

};

#endif