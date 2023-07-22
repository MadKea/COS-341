/* Keabetswe Madumo u20438614*/
#include "node.h"
#include <iostream>

node::node(string name)
{
    this->name = name;
    this->accept=false;
    this->start = false;
    this->dead = false;
}

node::node(string name, string name2, vector<node*> set)
{
    this->DFAname = name;
    this->name = name2;
    this->nfaSet = set;
    this->accept=false;
}

vector<transition*> node::getTran()
{
    return transitions;
}
void node:: setAddedToDFA(bool ad)
{
    this->addedToDfa = ad;
}
bool node::getaddedToDFA()
{
    return addedToDfa;
}
vector<node*> node:: getSet()
{
    return nfaSet;
}

bool node::getAccept()
{
    return accept;
}

void node::setDead()
{
    this->dead = true;
}

bool node::getDead()
{
    return dead;
}

void node::setStart()
{
    this->start = true;
}

bool node::getStart()
{
    return start;
}

void node::setTrans(vector<transition*> list)
{
    this->transitions = list;
}

void node::addTrans(transition* t)
{
    this->transitions.push_back(t);
}

void node::setAccept()
{
    this->accept=true;
}

string node::getDFAname()
{
    return DFAname;
}
string node:: getName()
{
    return name;
}

string node::display()
{
    if(this->accept == true)
    {
        string a ="";
        a +="|";
        a+= this->name;
        a+= "|";
        return a;
    }
    return name;
}

string node::displayD()
{
    
    if(this->accept == true)
    {
        string a ="";
        a +="|";
        a+= this->DFAname;
        a+= "|";
        return a;
    }
    return DFAname;
}

void node :: displayTransD()
{ 
    this->visited = true;

    for(int x =0; x< transitions.size(); x++)
    {
        cout<<transitions[x]->getTransitionOnD()<<endl;

        if(transitions[x]->getTo()->visited == false)
        {
            transitions[x]->getTo()->displayTransD();
        }
        
    }

    this->visited = false;
 
}

void node :: displayTrans()
{ 
    this->visited = true;

    for(int x =0; x< transitions.size(); x++)
    {
        cout<<transitions[x]->getTransitionOn()<<endl;

        if(transitions[x]->getTo()->visited == false)
        {
            transitions[x]->getTo()->displayTrans();
        }
        
    }

    this->visited = false;
 
}

int node::getTeam()
{
    return team;
}

void node::setTeam(int a)
{
    this->team = a;
}

vector<node*> node::getClosure()
{
    return Eclosure;
}



void node::setClosure(node * e, int cheat)
{
    if(cheat>0)
    {
        for(int x=0; x< transitions.size(); x++)
        {
            if(transitions[x]->getTransition()=="<")
            {
            
                transitions[x]->getTo()->setClosure(e, --cheat);   
                
                
            } 
        }
        e->addClose(this);
    }
    
    
    
}


void node::addClose(node* e)
{
    this->Eclosure.push_back(e);
}

