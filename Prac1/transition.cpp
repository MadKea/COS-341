/* Keabetswe Madumo u20438614*/
#include "transition.h"
#include <iostream>

using namespace std;

transition :: transition(node* from, node* to, string transOn)
{
    this->from = from;
    this->to = to;
    this->transitionOn = transOn;
    from->addTrans(this);
}

node* transition::getTo()
{
    return to;
}

string transition::getTransitionOn()
{
    string a = "|";
    a+= from->display();
    a+= "|--";
    a+= transitionOn;
    a+= "-->|";
    a+= to->display();
    a+= "|";

    return a;
}

string transition::getTransitionOnD()
{
    string a = "|";
    a+= from->displayD();
    a+= "|--";
    a+= transitionOn;
    a+= "-->|";
    a+= to->displayD();
    a+= "|";

    return a;
}

string transition:: getTransition()
{
    return transitionOn;
    
}

node* transition::getFrom()
{
    return from;
}