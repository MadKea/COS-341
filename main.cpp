/* Keabetswe Madumo u20438614*/

#include "NFA.h"
#include "transition.h"
#include "node.h"
#include "DFA.h"
#include "minDFA.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std; 
string inputRegex();
string formatString(string);
string questionMark(string);
string brackets(string);
string plusStar(string);
void createXML(minDFA*);
bool stringValidationA(string);
void createErrorXMLA();
bool stringValidationB(string);
void createErrorXMLA2();



int main()
{


    string regex = inputRegex();
    if(stringValidationB(regex))
    {
        if(stringValidationA(regex))
        {
            regex = formatString(regex);
            NFA *automata = new NFA(regex);
            DFA *dautomata = new DFA(automata);
            dautomata->convert();
            minDFA* miniD = new minDFA(dautomata);
            createXML(miniD);
        }
        else
        {
            createErrorXMLA();
        }
    }
    else
    {
        createErrorXMLA2();
    }
    

    return 0;
}

void createErrorXMLA()
{
    string error = "<error> Invalid characters as input </error>";
    std::ofstream fileStored("minDFA.xml");
    fileStored << error;
    fileStored.close();

}

void createErrorXMLA2()
{
    string error = "<error> Uneven amount of brackets. Double check </error>";
    std::ofstream fileStored("minDFA.xml");
    fileStored << error;
    fileStored.close();

}

bool stringValidationA(string input)
{
    
    for(int x=0; x< input.length(); x++)
    {
        if(!(iswalnum(input[x])|| input[x]== '*'|| input[x]== '(' || input[x]== ')'|| input[x]== '+'|| input[x]== '?' || input[x]== '<'|| input[x]== '|'))
        {
            return false;
        }
    }

    return true;
}

bool stringValidationB(string input)
{
    int b=0;
    for(int x=0; x< input.length(); x++)
    {
        
        if(input[x]=='(')
        {
            b++;
        }
        if(input[x]==')')
        {
            b--;
        }
    }

    if(b==0)
    {
        return true;
    }

    return false;
}
string inputRegex()
{
    string regy;
    cin>>regy;
    return regy;
}
string formatString(string s)
{

    string t = plusStar(s); 
    string m = questionMark(t);
    string k = brackets(m);
    return k;
}

void createXML(minDFA* a)
{

    string xmlString="";
    xmlString += "<states>\n";

    vector<node*> list = a->getMini();
    for(int x=0; x< list.size(); x++)
    {
        if(list[x]->getDead()==false)
        {
            xmlString += "<";
            xmlString += list[x]->getName();
            xmlString += "/>\n"; 
        }
        
    }

    xmlString += "</states>\n<transitions>\n";

    for(int x=0; x< list.size(); x++)
    {
        if(list[x]->getDead()==false)
        {
            xmlString += "\t<";
            xmlString += list[x]->getName();
            xmlString += ">\n";

            vector<transition*> path = list[x]->getTran();
            for(int y=0; y< path.size(); y++)
            {
                if(path[y]->getTo()->getDead() == false)
                {
                    xmlString += "\t\t<";
                    string to = path[y]->getTo()->getName();
                    xmlString += to;
                    xmlString += ">";
                    xmlString += path[y]->getTransition();
                    xmlString += "<";
                    xmlString += to;
                    xmlString += "/>\n";
                }
            }

            xmlString += "\t</";
            xmlString += list[x]->getName();
            xmlString += ">\n";
        }
        
    }

    xmlString += "</transitions>\n";

    xmlString += "<start state>\n";

    for(int x=0; x<list.size(); x++)
    {
        if(list[x]->getStart() == true)
        {
            xmlString += "\t<";
            xmlString += list[x]->getName();
            xmlString += ">\n</start state>\n";
        }
    }

    xmlString += "<accept states>\n";

    for(int x=0; x<list.size(); x++)
    {
        if(list[x]->getAccept() == true)
        {
            xmlString += "\t<";
            xmlString += list[x]->getName();
            xmlString += ">\n";
        }
    }

    xmlString += "</accept state>\n";


    
    std::ofstream fileStored("minDFA.xml");
    fileStored << xmlString;
    fileStored.close();

}

string plusStar(string s)
{
    string m = s;
    string temp ="";
    bool end = true;
    while(end)
    {
        int pos =0;
        int var =0;
        bool brack = false;
        while(pos<m.size() && m[pos]!= '+')
        {
            pos++;
        }

        if(pos==m.size())
        {
            break;
        }
        var = pos-1;
        while(m[var] == '?' || m[var]== '*' || m[var]==')')
        {
            if(m[var]==')')
            {
                brack = true;
                break;
            }
            var--;
        }

    
        if(brack != true)
        {
            for(int x=0; x<var; x++)
            {
                temp=temp+m[x];
            }
            temp = temp + '(';
            for(int x=var; x<pos; x++)
            {
                temp=temp+m[x];
            }
            temp = temp+ ")(";
            for(int x=var; x<pos; x++)
            {
                temp=temp+m[x];
            }
            temp= temp+ ")*";
            for(int x=pos+1; x< m.size(); x++)
            {
                temp=temp+m[x];
            }

        }

        if(brack)
        {
            int b = 1;
            int fback = var;
            while(b != 0)
            {
                fback--;
                if(m[fback]==')')
                {
                    b++;
                }
                else if(m[fback]=='(')
                {
                    b--;
                }
                
            }
    
            for(int x=0; x<pos; x++)
            {
                temp=temp+m[x];
            }
            
            for(int x=fback; x<pos; x++)
            {
                temp=temp+m[x];
            }
            temp = temp+ "*";
            for(int x=pos+1; x<m.size(); x++)
            {
                temp=temp+m[x];
            }
            brack=false;

        }
        
        m = temp;

        if(pos==m.size())
        {
            end = true;
        }

    }

    return m;
}

string brackets(string s)
{
    
    int b = 0;
    bool in =false;

    for(int x=0; x< s.size(); x++)
    {
        if(s[x]=='(')
        {
            b++;
        }
        if(s[x]==')')
        {
            b--;
        }

        if(b==0 && s[x]=='|')
        {
            in = true;
        }
    }

    if(in ==false)
    {
        return s;
    }

   string temp = "";
   string m=s;
   int start =0;
   int end = 0;
   int trv =0;
   b=0;

   while(trv < m.size())
   {

        if(m[trv] =='(')
        {
            b++;
        }
        if (m[trv] == ')')
        {
            b--;
        }
        if(b==0 && m[trv] == '|')
        {
            temp += '(';
            for(int x=start; x< trv;x++ )
            {
                temp += m[x];
            }
            temp += ")|";

            start = trv +1;
        }

        trv ++;
   }

    temp += '(';
    for(int x=start; x< trv;x++ )
    {
        temp += m[x];
    }
    temp += ")";

   return temp;
}
string questionMark(string s)
{
    string m = s;
    string temp ="";
    bool end = true;
    while(end)
    {
        int pos =0;
        while(pos<m.size() && m[pos]!= '?')
        {
            pos++;
        }

        if(pos==m.size())
        {
            break;
        }

        int var = pos-1;
        bool brack=false;
        while(m[var]== '*' || m[var]==')')
        {
            if(m[var]==')')
            {
                brack = true;
                break;
            }
            var--;
        }
        
        if(m[pos-1] == '*')
        {
            if(brack != true)
            {
                for(int x=0; x< var; x++)
                {
                    temp += m[x];
                }
                temp += "(("; temp += m[var]; temp += "*)|<)";
                for(int x= pos+1; x< m.size(); x++)
                {
                    temp += m[x];
                }
            }
            else
            {
                int b = 1;
                int fback = var;
                while(b != 0)
                {
                    fback--;
                    if(m[fback]==')')
                    {
                        b++;
                    }
                    else if(m[fback]=='(')
                    {
                        b--;
                    }
                    
                }

                for(int x=0; x< fback; x++)
                {
                    temp += m[x];
                }
                temp += '(';
                for(int x=fback; x< var+1; x++)
                {
                    temp += m[x];
                }
                temp += "*|<)";
                for(int x= pos+1; x< m.size(); x++)
                {
                    temp += m[x];
                }


            }
        }
        else if(m[pos-1] == ')')
        {
            int b = 1;
            int fback = pos-1;
            while(b != 0)
            {
                fback--;
                if(m[fback]==')')
                {
                    b++;
                }
                else if(m[fback]=='(')
                {
                    b--;
                }
                
            }

            for(int x=0; x< fback; x++)
            {
                temp += m[x];
            }
            temp += '(';
            for(int x=fback ; x< pos; x++)
            {
                temp += m[x];
            }
            temp += "|<)";

            for(int x= pos+1; x< m.size(); x++)
            {
                temp += m[x];
            }

        }
        else
        {
            if(pos-2<0)
            {
                temp = temp + "(";
                temp = temp + m[pos-1];
                temp = temp +"|<)";
            }
            else
            {
                for(int x=0; x<pos-1; x++)
                {
                    temp = temp+m[x];
                }

                temp = temp + "(";
                temp = temp + m[pos-1];
                temp = temp+ "|<)";
            }
            
            for(int x=pos+1; x< m.size(); x++)
            {
                temp = temp + m[x];
            }
        }
        
        if(pos>=m.size())
        {
            end = false;
        }
        m= temp;
        temp="";
    }
    
    return m;
}