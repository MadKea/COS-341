/* Keabetswe Madumo u20438614*/
#include "minDFA.h"
#include<sstream>

using namespace std;

minDFA::minDFA(DFA* automata)
{
    this->dautomata = automata;
    this->input = this->dautomata->returnInput();
    minimise();
}
 

void minDFA::minimise()
{
    initialisePartition();
    partition();

    vector<node*> n = dautomata->getNodes();
    // for(int x=0; x< n.size(); x++ )
    // {
    //     cout<<n[x]->getName()<<" "<<n[x]->getTeam()<<endl;
    // }

    createFinalNodes();
    setStart();
    addingTrans();
}

int minDFA::joinTeam()
{
    this->teamCounter++;
    return teamCounter;
}

void minDFA::setStart()
{
    int team;
    for(int x=0; x< dautomata->getNodes().size(); x++)
    {
        if(dautomata->getNodes()[x]->getStart())
        {
            team = dautomata->getNodes()[x]->getTeam();
        }
    }

    for(int x=0; x< mini.size(); x++)
    {
        if(mini[x]->getTeam()==team)
        {
            mini[x]->setStart();
        }
    }


}

void minDFA::createFinalNodes()
{
    vector<int> covered;
    bool in;
   for(int x=0; x< dautomata->getNodes().size(); x++)
   {
        in = false;
        for(int y=0; y< covered.size(); y++)
        {
            if(dautomata->getNodes()[x]->getTeam()==covered[y])
            {
                in = true;
            }
        }

        if(!in)
        {
            node* nody = new node(nameNode());
            nody->setTeam(dautomata->getNodes()[x]->getTeam());
            if(dautomata->getNodes()[x]->getAccept())
            {
                nody->setAccept();
            }

            if(dautomata->getNodes()[x]->getDead())
            {
                nody->setDead();
            }
            mini.push_back(nody);

        }

        covered.push_back(dautomata->getNodes()[x]->getTeam());
   }
}

void minDFA::display()
{
    for(int x=0; x< mini.size(); x++)
    {
        cout<<"--------------"<<endl;
        mini[x]->displayTrans();
    }
}

vector<node*> minDFA:: getMini()
{
    return mini;
}
void minDFA::addingTrans()
{
    for(int x=0; x< mini.size(); x++)
    {
        for(int y=0; y< input.size(); y++)
        {
            int t = transitionTeam(input[y], mini[x]->getTeam());

            for(int k=0; k< mini.size(); k++)
            {
                if(mini[k]->getTeam()== t)
                {
                    transition* b = new transition(mini[x], mini[k], input[y]);
                    finalTrans.push_back(b);
                }
            }
        }
    }
}

int minDFA::transitionTeam(string input, int team)
{
    vector<node*> a = dautomata->getNodes();

    for(int x=0; x< a.size(); x++)
    {
        if(a[x]->getTeam()== team)
        {
            for(int y=0; y< a[x]->getTran().size(); y++)
            {
                if(a[x]->getTran()[y]->getTransition()==input)
                {
                    return a[x]->getTran()[y]->getTo()->getTeam();
                }
            }
        }
    }

    return -1;
}

string minDFA::nameNode()
{
    stringstream n;
    n << this->namingNode++;
    string namae = "q";
    string num;
    n>>num;
    namae += num;

    return namae;
}



void minDFA::initialisePartition()
{
    vector<node*> nodes = dautomata->getNodes();
    bool normal=false;

    for(int x=0; x< nodes.size(); x++)
    {

        if(nodes[x]->getAccept())
        {

            nodes[x]->setTeam(1);
        }
        else
        {
            nodes[x]->setTeam(2);
            normal = true;
        }
    }

    joinTeam();

    if(normal)
    {
        joinTeam();
    }
}

int minDFA::getTeamCounter()
{
    return teamCounter;
}


bool minDFA::teamCompare(node* a, node* b)
{
    vector<transition*> transa =a->getTran();
    vector<transition*> transb =b->getTran();

    bool same = true;

    for(int x=0; x< transa.size(); x++)
    {
        for(int y=0; y< transb.size(); y++)
        {
            //cout<<transa[x]->getFrom()->getName()<<" vs "<<transb[y]->getFrom()->getName()<<endl;
            if(transa[x]->getTransition() == transb[y]->getTransition())
            {
                //cout<<transa[x]->getTo()->getTeam()<<" "<<transb[y]->getTo()->getTeam()<<endl;

                if(transa[x]->getTo()->getTeam() != transb[y]->getTo()->getTeam())
                {
                    same = false;
                }
            }
        }
    }

    return same;
}
void minDFA::partition()
{
    vector<node*> nodes = dautomata->getNodes();
    int r = nodes.size();
    bool used = true;
    while(r>0)
    {
        int check = getTeamCounter();
        int next = getTeamCounter();

        for(int x=0; x< nodes.size(); x++)
        {
            int next = joinTeam(); 
            for(int y=0; y< nodes.size(); y++)
            {
                if(y != x && nodes[x]->getTeam() == nodes[y]->getTeam())
                {
                   if(!(teamCompare(nodes[x], nodes[y])))
                   {
                        nodes[y]->setTeam(next);
                   }
                }
            }
        }
        r--;
    }
}