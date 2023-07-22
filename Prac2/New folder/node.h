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
    public:
        node(string name, bool terminal, int id);
        ~node();
        void addNode(node* n);
        void display();
        string parse(string tab, string input);
};


#endif