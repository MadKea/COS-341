/*Keabetswe Madumo u20438614*/

#include "node.h"
#include <sstream>

node::node(string name, bool terminal, int id)
{
    this->name = name;
    this->terminal = terminal;
    if(!terminal)
    {
        this->id = id;
    }
}

void node:: addNode(node* n)
{
    this->nodes.push_back(n);
}

void node::display()
{
    if(this->terminal )
    {
        return;
    }
   
    if(this->name != "root")
    {
        cout<<this->name<<": ";
        for(int x=0; x< nodes.size(); x++)
        {
            cout<<nodes[x]->name<<" ";
        }
        cout<<endl;
    }
    
    for(int x=0; x< nodes.size(); x++)
    {
        nodes[x]->display();
    }
}

node::~node()
{
    for(int x=0; x< nodes.size(); x++)
    {
        delete nodes[x];
    }

    nodes.clear();
}

string node::parse(string tab, string input)
{
    input += tab;
    if(name != "root" &&  !terminal)
    {
        input += "<";
        input += name;
        input += " id='";

        string ids;
        stringstream si;
        si << id;
        si>>ids;

        input += ids;
        input += "'  children: ";

        for(int x=0; x< nodes.size(); x++)
        {
            if(!nodes[x]->terminal)
            {
                input += "'";
                string idv;
                stringstream vi;
                vi << nodes[x]->id;
                vi>>idv;
                input += idv;
                input += "' ";
            }
        }

        input += ">\n";
        string otab = tab;

        tab += "\t";
    
        for(int x=0; x< nodes.size(); x++)
        {
      
            input = nodes[x]->parse(tab, input);
           
        }

        input += otab;
        input += "</";
        input += name;
        input += ">\n";
    }
    else if(name != "root" && terminal)
    {
        input += "< terminal '";
        input += name;
        input += "'>\n";
    }

    if(name == "root")
    {
        input = nodes[0]->parse(tab, input);
    }

    return input;
}