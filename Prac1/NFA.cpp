/* Keabetswe Madumo u20438614*/
#include "NFA.h"
#include<sstream>
NFA::NFA(string s)
{
    this->s = s;
    this->start = addNewNode("start");
    start->setStart();
    this->makeNFA();
}

void NFA::display()
{
    this->start->displayTrans();
}
void NFA::makeNFA()
{
    node* end = func(start, this->s);
    end->setAccept();
}


node* NFA::func(node* str, string wo) 
{
   string word = this->reformatReg(wo);
    if(word == "")
    {
        return str;
    }
    int w = route(word);
    
    if(w == 1)
    {
        node* end = addNewNode(this->nodeName());

        while(word!="")
        {
            int start= 0;
            int endin = 0;
            int b=0;

            while(endin < word.size())
            {
                if(word[endin]=='(')
                {
                    b++;
                }
                if(word[endin]==')')
                {
                    b--;
                }

                if(b==0 && word[endin]=='|')
                {
                    break;
                }

                endin++;
            }
            
            string a = "";

            for(int x=0; x< endin; x++)
            {
                a += word[x];
            }

            if(word[endin]=='|')
            {
                word.erase(0,endin+1);
            }
            else
            {
                word.erase(0,endin);
            }

            node* begin = addNewNode(this->nodeName());
            transition* eps = new transition(str, begin, "<");
            transition* don = new transition(func(begin, a), end, "<");
        }

        return end;
    }
    else if(w ==2)
    {
        node* nodeA = addNewNode(this->nodeName());
        node* nodeB = addNewNode(this->nodeName());
        transition* a1 = new transition(str, nodeA, "<");
        transition* a2 = new transition(nodeA, nodeB, "<");
        word.pop_back();
        node* ret1 = func(nodeB, word);
        transition* a3 = new transition(ret1, nodeA, "<");
        return nodeA;

    }
    else if(word.size()== 1)
    {
        addInput(word);
        node *p = addNewNode(this->nodeName());
        transition* t = new transition(str, p, word);
        return p;
    }
    else
    {
        node *vp = str;
        int count=0;
        int b=0;

        for(int x=0; x< word.size(); x++)
        {
            if(word[x] == '(')
            {
                b++;
            }
            if(word[x] == ')')
            {
                b--;
            }

            if(word[x] != '*' && b==0)
            {
                
                count++;
            }
        }

        string temp ="";
        b=0;
        int start=0;
        int trv = 0;

        for(int x=0; x< count; x++)
        {
           temp = "";

            while(trv < word.size())
            {
                if(word[trv]=='(')
                {
                    b++;
                }
                if(word[trv]==')')
                {
                    b--;
                }

                if(b==0 && trv < word.size())
                {
                    for(int r = start; r< trv+1; r++)
                    {
                        
                        temp += word[r];
                    }
                    
                    if(trv+1 < word.size() && word[trv+1] == '*')
                    {
                        temp += "*";
                        trv++;
                    }
        
                }
              
                trv++;

                if(temp != "")
                {
                    break;
                }
            }
            
            start = trv;
            vp = func(vp, temp);
        }

        return vp;
    }
    return str;
}

string NFA::reformatReg(string reg)
{
    int x=0;
    int b=0;
    bool skip = false;
    while(x< reg.size())
    {
        if(reg[x]=='(')
        {
            b++;
        }
        if(reg[x]==')')
        {
            b--;
        }

        if(b==0 && x<reg.size()-1)
        {
            skip = true;
        }
        x++;
    }
    string m = reg;
    if(reg[0]=='(' && reg[reg.size()-1]==')' && skip==false)
    {
        
        m.erase(0,1);
        m.pop_back();
    }

    return m;
}

int NFA::route(string str)
{
    int b = 0;
    bool brack =false;
    int var =0;
    bool skip = false;

    if(str[str.size()-1]=='*')
    {
        while(var < str.size())
        {
            if(str[var]=='(')
            {
                b++;
            }
            if(str[var]==')')
            {
                b--;
            }

            if(b==0 && var<str.size()-2)
            {
                skip = true;
            }
            var++;
        }

        if(skip==false)
        {
            return 2;
        }
    }

    if(str.size()== 2)
    {
        if(str[1]=='*')
        {
            return 2;
        }
    }

    for(int x=0; x< str.size(); x++)
    {
        if(str[x]=='(')
        {
            brack = true;
            b++;
        }
        if(str[x]==')')
        {
            brack = true;
            b--;
        }

        if(b==0 && str[x]=='|')
        {
            return 1;
        }

        if(b==0 && x==str.size()-2 &&  str[str.size()-2]==')'&& str[str.size()-1]=='*' )
        {
            return 4;
        }
    }

    return 3;
}

string NFA::nodeName()
{
    stringstream n;
    n<<Name;
    this->Name = this->Name + 1;
    string Naam;
    n>>Naam;

    return Naam;
}

vector<string> NFA::returnInput()
{
    return input;
}

node* NFA:: getStart()
{
    return start;
}
void NFA::addInput(string in)
{
    int x=0;
    if(in == "<")
    {
        return;
    }
    if(input.size()== 0)
    {
        this->input.push_back(in);
        return;
    }
    while(in != input[x] && x< input.size())
    {
        x++;
    }

    if(x==input.size())
    {
        this->input.push_back(in);
    }
}

node* NFA::addNewNode(string name)
{
    node* a = new node(name);
    nodes.push_back(a);
    return a;
}

vector<node*> NFA::getNodes()
{
    return nodes;
}

void NFA::makeClosures()
{
    for(int x=0; x< nodes.size(); x++)
    {
        int cheat = nodes.size();
        nodes[x]->setClosure(nodes[x], cheat);
    }
}