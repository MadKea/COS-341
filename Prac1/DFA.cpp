/* Keabetswe Madumo u20438614*/

#include "DFA.h"
#include<string>
#include<vector>
#include<sstream>
#include<iostream>

using namespace std;

DFA::DFA(NFA* automata)
{
    this->nonAuto = automata;
    automata->makeClosures();
    vector<node*> starting = automata->getStart()->getClosure();
    string n1 = makeName(starting);
    start = addNewNode(nodeName(), n1, starting);
    start->setStart();
    vector<node*> t;
    t.clear();
    trap = addNewNode("D", "D", t);
    trap->setDead();
    input = automata->returnInput();
}

vector<string> DFA::returnInput()
{
    return this->input;
}

void DFA::convert()
{
   
  bool done = false;
  bool in = false;
  while(!done)
  {
    for(int x=0; x< nodes.size(); x++)
    {
        in = false;
        if(nodes[x]->getaddedToDFA()==false)
        {
            table();
            setDone();
            in = true;
        }
    }

    if(!in)
    {
        done = true;
    }
  }

    for(int x=0; x< nodes.size(); x++)
    {
       while(checkDuplicate(nodes[x]->getTran()) == true)
       { 
            nodes[x]->setTrans(removeSingleDuplicate(nodes[x]->getTran()));
       }
    }

    removeDead();
    acceptStates();
}

void DFA::acceptStates()
{
    for(int x=0; x<nodes.size(); x++)
    {
        for(int y=0; y< nodes[x]->getSet().size(); y++)
        {
            if(nodes[x]->getSet()[y]->getAccept()==true)
            {
                nodes[x]->setAccept();
            }
        }
    }
}

vector<transition*> DFA::removeSingleDuplicate(vector<transition*> list)
{
   for(int x=0; x< list.size(); x++)
    {
        for(int y=0; y<list.size(); y++)
        { 
            if(x != y && list[x]->getTo() == list[y]->getTo() && list[x]->getTransition() == list[y]->getTransition())
            {
                list.erase(list.begin()+y);
                break;
            }
            
        }
    }

    return list;
}

bool DFA::checkDuplicate(vector<transition*> list)
{
    for(int x=0; x< list.size(); x++)
    {
        for(int y=0; y< list.size(); y++)
        {   
            if(x != y && list[x]->getTo() == list[y]->getTo() && list[x]->getTransition() == list[y]->getTransition())
            {
                return true;
            }
        }
    }

    return false;
}

void DFA:: table()
{
    for(int x=0; x< nonAuto->returnInput().size(); x++)
    {
        for(int y =0; y< nodes.size(); y++)
        {
            if(nodes[y]->getaddedToDFA()!=true)
            {
                vector<node*> set = nodes[y]->getSet();
                vector<node*> trans;
                for(int v =0; v< set.size(); v++)
                {
                    for(int z =0; z< set[v]->getTran().size(); z++)
                    {
                        if(set[v]->getTran()[z]->getTransition() == nonAuto->returnInput()[x])
                        {
                            trans = addvec(trans, set[v]->getTran()[z]->getTo()->getClosure() );

                        }
                    }

                }

                node* comp;
                for(int k=0; k< nodes.size(); k++)
                {
                    comp = compare(trans, nodes[k]);
                    if(comp != nullptr)
                    {
                        break;
                    }
                }

                transition* in;

                if(trans.size()==0)
                {
                    new transition(nodes[y], trap, nonAuto->returnInput()[x]);
                    if(nodes[y]->getName()!="D")
                    {
                        deadUsed=true;
                    }
                }
                else if(comp != nullptr)
                {
                    in = new transition(nodes[y], comp, nonAuto->returnInput()[x]);
                }
                else
                {
                    node* re = addNewNode(nodeName(), makeName(trans), trans);
                    in = new transition(nodes[y], re, nonAuto->returnInput()[x]);
                }
                
            }
        }
    }
}

void DFA::removeDead()
{
    if(deadUsed==false)
    {
        for(int x=0; x< nodes.size(); x++)
        {
            if(nodes[x]->getName()=="D")
            {
                nodes.erase(nodes.begin()+x);
                break;
            }
        }
    }
}

bool DFA::setDone()
{
    bool found;
    int total;
    for(int x=0; x< nodes.size(); x++)
    {
        total = 0;
        for(int y=0; y< nonAuto->returnInput().size(); y++)
        {
            found = false;
            for(int z=0; z< nodes[x]->getTran().size(); z++)
            {
                if(nonAuto->returnInput()[y]== nodes[x]->getTran()[z]->getTransition())
                {
                    total++;
                    found = true;
                    break;
                }
            }

            if(found == false)
            {
                return false;
            }
        }

        if(total == nonAuto->returnInput().size())
        {
            nodes[x]->setAddedToDFA(true);
        }
    }

    return true;
}

vector<node*> DFA:: addvec(vector<node*> into, vector<node*> adding)
{
    bool found;
    for(int x=0; x< adding.size(); x++)
    {
        found = false;
        for(int y=0; y<into.size(); y++)
        {
            if(adding[x]->getName() == into[y]->getName())
            {
                found = true;
            }
        }

        if(!found)
        {
            into.push_back(adding[x]);
        }
    }

    return into;
}
node* DFA::compare(vector<node*> f, node* lnode)
{
    vector<node*> l = lnode->getSet();
    if(f.size() != l.size())
    {
        return nullptr;
    }

    bool found =false;
    int x=0;

    while(x< f.size())
    {
        for(int y=0; y< l.size(); y++)
        {
            if(f[x]->getName()==l[y]->getName())
            {
                found = true;
                break;
            }
        }
        if(found == false)
        {
            return nullptr;
        }
        found = false;
        x++;
    }

    return lnode;
}

void DFA::display()
{
    this->start->displayTrans();
}

void DFA::displayD()
{
    this->start->displayTransD();
}


node* DFA::addNewNode(string s1, string s2, vector<node*> set)
{
    node* p = new node(s1, s2, set);

    nodes.push_back(p);
    return p;
}
string DFA::makeName(vector<node*> vec)
{
    string namae = "";

    int k= vec.size();

    for(int x=0; x< (k-1); x++)
    {
        namae += vec[x]->getName();
        namae += ",";
    }

    if(k-1 >= 0)
    {
        namae += vec[vec.size()-1]->getName();
    }

    return namae;
}

string DFA::nodeName()
{
    stringstream n;
    n<<Name;
    this->Name = this->Name + 1;
    string Naam;
    n>>Naam;

    string nm= "";
    nm += "q";
    nm+= Naam;

    return nm;
}

vector<node*> DFA::getNodes()
{
    return nodes;
}