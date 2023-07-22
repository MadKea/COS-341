#include <fstream>
#include <sstream>
#include "trans.h"

using namespace std;

trans::trans(table* Hash, node* AST)
{
    ofstream outputFile;
    outputFile.open("basic.txt");
    if(!outputFile)
    {
        cout<<"basic.txt does not exist"<<endl;
        return;
    }

    
    // vector<string> ers = Hash->getErrors();
    // if(ers.size()>1)
    // {
    //     string er = "05 PRINT \"";
    //     for(int x=0; x< ers.size(); x++)
    //     {
    //         er += ers[x];
    //     }
    //     er += "\"\n";
    //     basic += er;
    // }

    this->symbolT = Hash;
    this->AST = AST;
    TRANS(AST);
    outputFile<<basic;
    outputFile.close();
}

trans::~trans(){}

void trans::TRANS(node* current)
{
   // cout<<current->getName()<<endl;
    if(current->getName() == "INSTR")
    {
        if(current->getTransCheck() == false)
        {
            TRANSINSTR(current);
            current->setTransCheck();
        }
        
    }

    vector<node*> nodes = current->getNodes();

    if(current->getName()=="root")
    {
        vector<node*> nodes1 = nodes[0]->getNodes();
        TRANS(nodes1[0]);
        basic += lining();
        basic += " END\n";
        if(nodes1.size()> 1)
        {
            TRANS(nodes1[1]);
        }
    }
    else if(current->getName()=="PROC")
    {
        string ln =current->getLineNumber();
        vector<node*> nodes1= current->getNodes();
        vector<node*> nodes2= nodes1[2]->getNodes();
        if(ln=="-1")
        {
            ln = lining();
        }
        basic += ln;
        basic += " REM start of Proc ";
        basic += current->getValue();
        basic += "\n";
        TRANS(nodes2[0]);
        basic += lining();
        basic += " RETURN\n";

        if(nodes2.size()>1)
        {
            TRANS(nodes2[1]);
        }

    }
    else
    {
        for(int x=0; x< nodes.size(); x++)
        {
            TRANS(nodes[x]);
        }
    }
    
}

void trans::TRANSINSTR(node* current)
{
    vector<node*> nodes = current->getNodes();
    if(nodes[0]->getName() == "ASSIGN")
    {
        TRANSASSIGN(nodes[0]);
        return;
    }

    if(nodes[0]->getName() == "INPUT")
    {
        TRANSINPUT(nodes[0]);
        return;
    }

    if(nodes[0]->getName() == "OUTPUT")
    {
        TRANSOUTPUT(nodes[0]);
        return;
    }

    if(nodes[0]->getName()== "LOOP")
    {
        TRANSLOOP(nodes[0]);
        return;
    }

    if(nodes[0]->getName()=="BRANCH")
    {
        TRANSBRANCH(nodes[0]);
        return;
    }

    if(nodes[0]->getName()=="CALL")
    {
        TRANSCALL(nodes[0]);
        return;
    }

    //halt

    string an = "";
    an += lining();
    an += " END\n";
    basic += an;

}

void trans::TRANSCALL(node* current)
{
    vector<node*> nodes1 = current->getNodes();
    string lineOn;
    lineOn = findAndSetLine(nodes1[0]->getName(), nodes1[0]->getParentScope(), nodes1[0]->getScopeName(), AST, "");

    string an = "";
    an += lining();
    an += " GOSUB ";
    an += lineOn;
    an += "\n";
    basic += an;
}

string trans::findAndSetLine(string name, int sameParent, string ischild, node* curr, string line)
{
    string lineOn = line;
    if(curr->getName()=="PROC" && curr->getValue()==name && (curr->getParentScope()==sameParent ||curr->getScopeName() == ischild ))
    {
        if(curr->getLineNumber()=="-1")
        {
            curr->setLineNumber(lining());
        }
        lineOn = curr->getLineNumber();
    }

    vector<node*> nodes = curr->getNodes();
    for(int x=0; x< nodes.size(); x++)
    {
       lineOn = findAndSetLine(name, sameParent, ischild, nodes[x], lineOn);
    }

    return lineOn;
}

void trans::TRANSBRANCH(node* current)
{
    vector<node*> nodes = current->getNodes();

    string tempVariable = TRANSBOOLEXPR(nodes[2]);

    string an = "";
    an += lining();
    an += " IF ";
    an += tempVariable;
    an += "=1 THEN GOTO ";
    string lineA = lining();
    an += lineA;
    an += "\n";
    an += lining();
    an += " REM start of else if present\n";
    basic += an;

    if(nodes.size()==9)
    {
        vector<node*> nodes1 = nodes[8]->getNodes();
        TRANS(nodes1[2]);
    }
    an ="";
    an += lining();
    an += " GOTO ";
    string lineB = lining();
    an += lineB;
    an += "\n";
    an += lineA;
    an += " REM Start of then\n";
    basic += an;
    TRANS(nodes[6]);
    an = "";
    an += lineB;
    an += " REM End of branch\n";
    basic += an;

}

void trans::TRANSLOOP(node* current)
{
    vector<node*> nodes = current->getNodes();
    string lineA = "";
    stringstream v;
    v<<lineNum;
    v>>lineA;
    string neggy = TRANSBOOLEXPR(nodes[2]);
    string an = "";
    an += lining();
    string lines1 = lining();
    string lines2 = lining();
    string lines3 = lining();
    an += " IF ";
    an += neggy;
    an += " =1 THEN GOTO ";
    an += lines3;
    an += "\n";
    an += lines1;
    an += " LET ";
    an += neggy;
    an += "= 1\n";
    an += lines2;
    an+= " GOTO ";
    stringstream vv;
    vv<<lineNum;
    string atNow="";
    vv>>atNow;
    an += atNow;
    an += "\n";
    an += lines3;
    an += " LET ";
    an += neggy;
    an += " =0\n";

    an += lining();
    an += " IF ";
    an += neggy;
    an += "=1 THEN GOTO ";
    string lineB = lining();
    an += lineB;
    an += "\n";
    basic += an;

    TRANS(nodes[5]);

    string an2="";
    an2 += lining();
    an2 += " GOTO ";
    an2 += lineA;
    an2 += "\n";

    an2 += lineB;
    an2 += " REM Spacing for loop GOTO's";
    an2 += "\n";

    basic += an2;
}
void trans::TRANSOUTPUT(node* current)
{
    vector<node*> nodes = current->getNodes();
    vector<node*> nodes1 = nodes[0]->getNodes();

    string Line = "";
    Line += lining();
    Line += " PRINT ";

    if(nodes1[0]->getName()=="o")
    {
        Line += nodes1[1]->getValue();
    }
    else
    {
        Line += nodes1[1]->getValue();
        Line += "$";
    }
    
    Line += "\n";
    basic += Line;
}

void trans::TRANSINPUT(node* current)
{
    vector<node*> nodes = current->getNodes();

    string Line = "";
    Line += lining();
    Line += " INPUT \"Input into ";
    Line += nodes[1]->getValue();
    Line += ":\";";
    Line +=nodes[1]->getValue();
    Line += "\n";

    basic += Line;

}
void trans::TRANSASSIGN(node* current)
{
   // cout<<"ASSIGN TRANS"<<endl;

    vector<node*> nodes = current->getNodes();

    string ans = "";
    if(nodes[0]->getName()=="NUMVAR")
    {
        ans = TRANSNUMEXPR(nodes[3]);
        string Line = lining();
        Line += " LET ";
        Line += nodes[0]->getValue();
        Line += " = ";
        Line += ans;
        basic += Line;
    }
    else if(nodes[0]->getName()=="BOOLVAR")
    {
        ans = TRANSBOOLEXPR(nodes[3]);
        string Line = lining();
        Line += " LET ";
        Line += nodes[0]->getValue();
        Line += " = ";
        Line += ans;
        basic += Line;
    }
    else
    {
        //string
        string Line = lining();
        Line += " LET ";
        Line += nodes[0]->getValue();
        Line += "$ = ";
        Line += nodes[3]->getValue();
        basic += Line;
      
    }
    basic += "\n";

    // vector<node*> nodes = current->getNodes();
    // for(int x=0; x< nodes.size(); x++)
    // {
    //     TRANS(nodes[x]);
    // }
}

string trans::TRANSNUMEXPR(node* current)
{
    vector<node*> nodes = current->getNodes();

    if(nodes[0]->getName() == "NUMVAR")
    {
        return nodes[0]->getValue();
    }
    else if(nodes[0]->getName()== "DECNUM")
    {
        return nodes[0]->getValue();
    }
    else
    {
        string n = nodes[0]->getName();
        string ans="";
        if(n=="a")
        {
            string para1 = TRANSNUMEXPR(nodes[2]);
            string para2 = TRANSNUMEXPR(nodes[4]);
            ans += "(";
            ans += para1;
            ans += ")";
            ans += "+";
            ans += "(";
            ans += para2;
            ans += ")";

        }
        else if(n=="m")
        {
            string para1 = TRANSNUMEXPR(nodes[2]);
            string para2 = TRANSNUMEXPR(nodes[4]);
            ans += "(";
            ans += para1;
            ans += ")";
            ans += "*";
            ans += "(";
            ans += para2;
            ans += ")";

        }
        else
        {
            //d
            string para1 = TRANSNUMEXPR(nodes[2]);
            string para2 = TRANSNUMEXPR(nodes[4]);
            ans += "(";
            ans += para1;
            ans += ")";
            ans += "/";
            ans += "(";
            ans += para2;
            ans += ")";
        }

        return ans;
    }

}

string trans::lining()
{
    int l = lineNum;
    lineNum=lineNum+5;

    stringstream ll;
    ll<<l;
    string ls;
    ll>>ls;
    return ls;
}

string trans::TRANSBOOLEXPR(node* current)
{
    vector<node*> nodes = current->getNodes();

    if(nodes[0]->getName()=="LOGIC")
    {
        return TRANSLOGIC(nodes[0]);
    }
    else 
    {
        //CMPR
        return TRANSCMPR(nodes[0]);
    }

    return "";
}

string trans::TRANSCMPR(node* current)
{
    vector<node*> nodes = current->getNodes();

    string para1 = "";
    string para2 = "";
    para1 = TRANSNUMEXPR(nodes[2]);
    para2 = TRANSNUMEXPR(nodes[4]);
    string an="";
    an += lining();
    an += " LET ";
    string tempVariable = giveTemP();
    an += tempVariable;
    an += " = ";
    an += para1;
    
    if(nodes[0]->getName()=="E")
    {
        an += "=";
    }
    else if (nodes[0]->getName()=="<")
    {
        an += "<";
    }
    else
    {
        an += ">";
    }

    an += para2;
    an += "\n";
    basic += an;
    return tempVariable;
}

string trans::TRANSLOGIC(node* current)
{
    vector<node*> nodes = current->getNodes();

    if(nodes[0]->getName()=="T")
    {
        return "1";
    }
    else if(nodes[0]->getName()=="F")
    {
        return "0";
    }
    else if(nodes[0]->getName()=="BOOLVAR")
    {
        return nodes[0]->getValue();
    }
    else if(nodes[0]->getName()=="^" || nodes[0]->getName()=="v")
    {
        string para1 ="";
        string para2 = "";
        para1 = TRANSBOOLEXPR(nodes[2]);
        para2 = TRANSBOOLEXPR(nodes[4]);
        string tempVariable = giveTemP();
        string test = "";
        string assign = "";
        if(nodes[0]->getName()=="^")
        {
            test = "0";
            assign = "1";
        }
        else
        {
            test = "1";
            assign = "0";
        }

        string lineA = lining();
        string lineB = lining();

        string an = "";
        an += lining();
        an += " IF ";
        an += para1;
        an += " = ";
        an += test;
        an += " THEN GOTO ";
        an += lineA;
        an += "\n";
        an += lining();
        an += " IF ";
        an += para2;
        an += " = ";
        an += test;
        an += " THEN GOTO ";
        an += lineA;
        an += "\n";
        an += lining();
        an += " LET ";
        an += tempVariable;
        an += " = ";
        an += assign;
        an += "\n";
        an += lining();
        an += " GOTO ";
        an += lineB;
        an += "\n";
        an += lineA;
        an += " LET ";
        an += tempVariable;
        an += " = ";
        an += test;
        an += "\n";
        an += lineB;
        an += " REM end of ^/v\n"; 
        basic += an;
        return tempVariable;
    }
    else
    {
        //!
        string para1="";
        para1 = TRANSBOOLEXPR(nodes[2]);
        string an = "";
        an += lining();
        an += " LET ";
        string tempVariable = giveTemP();
        an += tempVariable;
        an += " =";
        an+= para1;
        an+="\n";
        an+= lining();
        an += " IF ";
        an += tempVariable;
        string lines1 = lining();
        string lines2 = lining();
        string lines3 = lining();
        an += "=1 THEN GOTO ";
        an += lines3;
        an += "\n";
        an += lines1;
        an += " Let ";
        an += tempVariable;
        an += " =1\n";
        an += lines2;
        stringstream vv;
        vv<<lineNum;
        string atNow="";
        vv>>atNow;
        an += " GOTO ";
        an += atNow;
        an += "\n";
        an += lines3;
        an += " LET ";
        an += tempVariable;
        an += "=0\n";
        basic += an;

        return tempVariable;
    }
}

string trans::giveTemP()
{
    int x = tempV;
    tempV += 11;

    stringstream xx;
    xx<<x;
    string ss;
    xx>>ss;
    string give = "t";
    give +=ss;
    return give; 
}