/*Keabetswe
Madumo
u20438614
Practical 4*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include "node.h"
#include "table.h"

using namespace std;
void cleanUp();
void progr(node* );
void procdefs(node* );
void proc(node* );
void digits(node* );
void d(node* );
void more(node* );
void algo(node* );
void seq(node* );
void instr(node* );
void call(node* );
void assign(node* );
void loop(node* );
void branch(node* );
void Else(node* );
void numvar(node* );
void boolvar(node* );
void stringv(node* );
void numexp(node* );
void decnum(node* );
void neg(node* );
void pos(node* );
void Int(node* );
void boolexpr(node* );
void logic(node* );
void cmpr(node* );
void stri(node* );
void c(node* );
void comment(node* );
void input(node* );
void output(node* );
void value(node* );
void text(node* );
void prune(node *);
void compressing();
void assignScopes(node* );
bool siblingNames(node* );
void nameErrors(node* );
bool childNames(node* );
bool nephewNames(node* );
void checkingCalls(node* );
void htable(node* root, table *Hash);
void OutPutTable(table* Hash);

string program;
int globeID = 100;
bool errorFound;
string error;
string warning = "";

int main() 
{
    node* root = new node("root", false, globeID); globeID += 15;
    cout<<"Good day! Please enter the text file to be processed. Include the .txt extension."<<endl;
    string filename;
    cin>>filename;

    ifstream inputFile;
    inputFile.open(filename);

    if(!inputFile)
    {
        cout<<"FIle does not exist"<<endl;
        return 0;
    }

    program = "";
    string line;
    while(getline(inputFile, line))
    {
        program += line;
    }
    program += "$";
    inputFile.close();

    cleanUp();
    error = "";
    errorFound=false;
    progr(root);
  
    ofstream outputFile;
    outputFile.open("parseTree.xml");
    table *Hash = new table(); 
    if(errorFound)
    {
        outputFile<<error;
        Hash->addError(error);
    }
    else
    {
        string outline = "";
        string tab = "";
        outline = root->parse(tab, outline);
        outputFile<<outline;
        //root->display();
       // cout<<"--------------------------------------------------------"<<endl;
        prune(root);
        assignScopes(root);
        
        nameErrors(root);
        htable(root, Hash);
        //Hash->display();
        root->fixID(root);
        //root->display();
        root->crawl(root, Hash);
       Hash->display();
      
    }
    OutPutTable(Hash);
    outputFile.close();
    delete root;
    delete Hash;
    return 0;
}

void progr(node* n)
{
    //cout<<"PROGR"<<endl;

    node* a = new node("PROGR", false, globeID);
    globeID += 15;
    n->addNode(a);

    cleanUp();

    algo(a);
    procdefs(a);
}

void procdefs(node* n)
{
    //cout<<"PROCDEFS"<<endl;

    node* a = new node("PROCDEFS", false, globeID);
    globeID += 15;
    n->addNode(a);

    if(program[0]==',')
    {
        program = program.substr(1);
        cleanUp();
    
        proc(a);
        procdefs(a);
    }
    else if(program[0]=='$' || program[0]=='}')
    {
        node* t = new node("ε", true, globeID); //ε
        globeID += 15;
        a->addNode(t);
    }
    else 
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. PROCDEF begins with a comma if not empty";
        }
    }
    
}

void proc(node* n)
{
    //cout<<"PROC"<<endl;

    node* a = new node("PROC", false, globeID);
    n->addNode(a);

    if(program[0]=='p')
    {
        node* t = new node("p", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1); //p
        cleanUp();
        digits(a);

        if(program[0]=='{')
        {
            node* v = new node("{", true, globeID); globeID += 15;
            a->addNode(v);
            program = program.substr(1); //{
            cleanUp();
            progr(a);
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. PROC missing open brace. {";
            }
        }
        
        if(program[0]=='}')
        {
            node* m = new node("}", true, globeID); globeID += 15;
            a->addNode(m);
            program = program.substr(1); //}
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
            errorFound=true;
            error = "Error found. PROC missing closing brace. }";
            }
        }
    }
    else
    {
        if(!errorFound)
        {
        errorFound=true;
        error = "Error found. PROC starts with p after the comma in PROCDEF";
        }
    }
}

void digits(node* n)
{
    //cout<<"DIGITS"<<endl;
    
    node* a = new node("DIGITS", false, globeID); globeID += 15;
    n->addNode(a);

    d(a);
    more(a); 
}

void d(node* n)
{
    //cout<<"D"<<endl;
    node* a = new node("D", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='0' || program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        string place ="";
        place += program[0];
        node* t = new node(place, true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. DIGITS are made up of integers 0-9";
        }
    }    
}

void more(node* n)
{  
    //cout<<"MORE"<<endl;

    node* a = new node("MORE", false, globeID); globeID += 15;
    n->addNode(a);


    if(program[0]=='0' || program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        digits(a);
    }
    else if( program[0]=='.'|| program[0]=='{'|| program[0]==')'|| program[0]=='*'|| program[0] == ':' || program[0]==','|| program[0]=='$'|| program[0]=='}' || program[0]==';' || program[0]=='=')
    {
        node* t = new node("ε", true, globeID); globeID += 15;
        a->addNode(t);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. MORE. digits are made up of integers 0-1. If you stuck to that, maybe check if you're missing a semi-colon or comma or opening/closing brackets just after your digits.";
        }
    }
}

void algo(node* n)
{
    //cout<<"ALGO"<<endl;

    node* a = new node("ALGO", false, globeID); globeID += 15;
    n->addNode(a);

    instr(a);
    comment(a);
    seq(a);
}

void seq(node* n)
{
    //cout<<"SEQ"<<endl;

    node* a = new node("SEQ", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]==';')
    {
        node* t = new node(";", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1); //;
        cleanUp();
        algo(a);
    }
    else if(program[0]=='}' || program[0]==','|| program[0]=='$')
    {
        node* t = new node("ε", true, globeID); globeID += 15;
        a->addNode(t);
    }
    else if(program[0]=='p')
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. PROCDEF begins with a comma";
        }
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. SEQ begins with a semi-colon";
        }
    }        
}

void instr(node* n)
{
    //cout<<"INSTR"<<endl;

    node* a = new node("INSTR", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='h')
    {
        node* t = new node("h", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1); //h
        cleanUp();
    }
    else if(program[0]=='g')
    {
        input(a);
    }
    else if(program[0]=='c')
    {
        call(a);
    }
    else if(program[0]=='o' || program[0]=='r')
    {
        output(a);
    }
    else if(program[0]=='i')
    {
        branch(a);
    }
    else if(program[0]=='w')
    {
        loop(a);
    }
    else if(program[0]=='n'|| program[0]=='b' || program[0]=='s')
    {
        assign(a);
    }
    else
    {
         if(!errorFound)
        {
            errorFound=true;
            error = "Error found. not a valid or incomplete INSTR";
        }
    }
}

void call(node* n)
{
    //cout<<"CALL"<<endl;

    node* a = new node("CALL", false, globeID); globeID += 15;
    n->addNode(a);

    node* t = new node("c", true, globeID); globeID += 15;
    a->addNode(t);
    program = program.substr(1);//c 
    cleanUp();

    if(program[0]=='p')
    {
        node* p = new node("p", true, globeID); globeID += 15;
        a->addNode(p);
        program = program.substr(1);//p
        cleanUp();
        digits(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. Invalid CALL instruction. Correct form : c pDIGITS";
        }
    }
    
}

void assign(node* n)
{
    //cout<<"ASSIGN"<<endl;

    node* a = new node("ASSIGN", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='n')
    {
        numvar(a);
        if(program[0] == ':')
        {
            node* t = new node(":", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);
            cleanUp();
            if(program[0]=='=')
            {
                t= new node("=", true, globeID); globeID += 15;
                a->addNode(t);
                program = program.substr(1);// :=
                cleanUp();
            }
            else
            {
                if(!errorFound)
                {
                    errorFound=true;
                    error = "Error found. INVALID ASSIGN. Use ':='";
                }
            }
            
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID ASSIGN. Use ':='";
            }
        }
        
        numexp(a);

    }
    else if(program[0]=='b')
    {
        boolvar(a);
        
        if(program[0] == ':')
        {
            node* t = new node(":", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);
            cleanUp();
            if(program[0]=='=')
            {
                t= new node("=", true, globeID); globeID += 15;
                a->addNode(t);
                program = program.substr(1);// :=
                cleanUp();
            }
            else
            {
                if(!errorFound)
                {
                    errorFound=true;
                    error = "Error found. INVALID ASSIGN. Use ':='";
                }
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID ASSIGN. Use ':='";
            }
        }
        
        boolexpr(a);
    }
    else if(program[0]=='s')
    {
        stringv(a);
        if(program[0] == ':')
        {
            node* t = new node(":", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);
            cleanUp();
            if(program[0]=='=')
            {
                t= new node("=", true, globeID); globeID += 15;
                a->addNode(t);
                program = program.substr(1);// :=
                cleanUp();
            }
            else
            {
                if(!errorFound)
                {
                    errorFound=true;
                    error = "Error found. INVALID ASSIGN. Use ':='";
                }
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID ASSIGN. Use ':='";
            }
        }
        
        stri(a);
    }
}

void loop(node* n)
{
    //cout<<"LOOP"<<endl;

    node* a = new node("LOOP", false, globeID); globeID += 15;
    n->addNode(a);

    node* t = new node("w", true, globeID); globeID += 15;
    a->addNode(t);
    program = program.substr(1);//w
    cleanUp();

    if(program[0]=='(')
    {
        t = new node("(", true, globeID); globeID += 15;
        a->addNode(t);
            
        program = program.substr(1);//(
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID LOOP. Open bracket missing. '('.";
        }
    }
    
    
    boolexpr(a);

    if(program[0]== ')')
    {
        node* p = new node(")", true, globeID); globeID += 15;
        a->addNode(p);
        program = program.substr(1);//)
        cleanUp();

        if(program[0]=='{')
        { 
            p = new node("{", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//{
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOOP. Open brace missing. '{'";
            }
        }
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID LOOP. Use close bracket missing. ')'";
        }
    }
    algo(a);
    if(program[0]== '}')
    {
        node* v = new node("}", true, globeID); globeID += 15;
        a->addNode(v);
        program = program.substr(1);//}
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID LOOP. Close brace missing. '}'";
        }
    }
}

void branch(node* n)
{
    //cout<<"BRANCH"<<endl;

    node* a = new node("BRANCH", false, globeID); globeID += 15;
    n->addNode(a);



    node* t = new node("i", true, globeID); globeID += 15;
    a->addNode(t);
    program = program.substr(1);//i
    cleanUp();

    if(program[0]=='(')
    {
        t = new node("(", true, globeID); globeID += 15;
        a->addNode(t);
        
        program = program.substr(1);//(
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID BRANCH. Open bracket missing. '('";
        }
    }
    
 
    boolexpr(a);
    if(program[0]== ')')
    {
        node* p = new node(")", true, globeID); globeID += 15;
        a->addNode(p);
        program = program.substr(1);//)
        cleanUp();

        if(program[0]=='t')
        {
            p = new node("t", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//t
            cleanUp();

            if(program[0]=='{')
            {
                p = new node("{", true, globeID); globeID += 15;
                a->addNode(p);
                program = program.substr(1);//{
                cleanUp();
            }
            else
            {
                if(!errorFound)
                {
                    errorFound=true;
                    error = "Error found. INVALID BRANCH. Open brace missing. '{'";
                }
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID BRANCH. t missing or is uppercase. ";
            }
        }
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID BRANCH. Close bracket missing. ')'";
        }
    }
    algo(a);
    if(program[0]== '}')
    {
        node* v = new node("}", true, globeID); globeID += 15;
        a->addNode(v);
        program = program.substr(1);//}
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID BRANCH. Close brace missing. '}'";
        }
    }
    Else(a);
}

void Else(node* n)
{
    //cout<<"ELSE"<<endl;

    node* a = new node("ELSE", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='e')
    {
        node* t = new node("e", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//e
        cleanUp();
        if(program[0]=='{')
        {
            node* p = new node("{", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//{
            cleanUp();
        }
        else
        {
            //error
        }
        
        algo(a);
        if(program[0]=='}')
        {
            node* r = new node("}", true, globeID); globeID += 15;
            a->addNode(r);
            program = program.substr(1);//}
            cleanUp();
        }
        else
        {
            //error
        }
    }
    else if(program[0]=='*' || program[0]==';' || program[0]=='}' || program[0]==','  || program[0]=='$')
    {
        //cout<<"ε"<<endl;
        node* r = new node("ε", true, globeID); globeID += 15;
        a->addNode(r);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID ELSE. ELSE starts with e or is ε. You could be missing ; or ,";
        }
    }
}

void numvar(node* n)
{
    //cout<<"NUMVAR"<<endl;

    node* a = new node("NUMVAR", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0] == 'n')
    {
        node* t = new node("n", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//n
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID NUMVAR. Starts with n and followed by DIGITS";
        }
    }
    
    digits(a);
}

void boolvar(node* n)
{
    //cout<<"BOOLVAR"<<endl;

    node* a = new node("BOOLVAR", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0] == 'b')
    {
        node* t = new node("b", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//b
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID BOOLVAR. Starts with b and followed by DIGITS";
        }
    }
    digits(a);
}

void stringv(node* n)
{
    //cout<<"STRINGV"<<endl;

    node* a = new node("STRINGV", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0] == 's')
    {
        node* t = new node("s", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//s
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID STRINGV. Starts with s and followed by DIGITS";
        }
    }
    digits(a);
}

void numexp(node* n)
{
    //cout<<"NUMEXPR"<<endl;

    node* a = new node("NUMEXPR", false, globeID); globeID += 15;
    n->addNode(a);

    bool init = false;
    if(program[0]=='0' || program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
       
        init = true;
    }

    if(program[0]=='a')
    {
         
        node* t = new node("a", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//a
        cleanUp();

        if(program[0]=='(')
        {
            t = new node("(", true, globeID); globeID += 15;
            a->addNode(t);
            
            program = program.substr(1);//(
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing opening bracket. (";
            }
        }
            
        numexp(a);

        if(program[0]== ',')
        {
            node* p = new node(",", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//,
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing comma.";
            }
        }
        numexp(a);
        if(program[0]== ')')
        {
            node* v = new node(")", true, globeID); globeID += 15;
            a->addNode(v);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing closing bracket. )";
            }
        }
    }
    else if(program[0]=='m')
    {
       
        node* t = new node("m", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//m
        cleanUp();

        if(program[0]=='(')
        {
            t = new node("(", true, globeID); globeID += 15;
            a->addNode(t);
        
            program = program.substr(1);//(
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing opening bracket. (";
            }
        }
           
        numexp(a);

        if(program[0]== ',')
        {
            node* p = new node(",", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//,
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing comma";
            }
        }
        numexp(a);
        if(program[0]== ')')
        {
            node* v = new node(")", true, globeID); globeID += 15;
            a->addNode(v);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing closing bracket. )";
            }
        }
    }
    else if(program[0]=='d')
    {
        
        node* t = new node("d", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//d
        cleanUp();

        if(program[0]=='(')
        {
            t = new node("(", true, globeID); globeID += 15;
            a->addNode(t);
            
            program = program.substr(1);//(
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing opening bracket. (";
            }
        }
       
        numexp(a);

        if(program[0]== ',')
        {
            node* p = new node(",", true, globeID); globeID += 15;
            a->addNode(p);
            program = program.substr(1);//,
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing comma between expressions.";
            }
        }
        numexp(a);
        if(program[0]== ')')
        {
            node* v = new node(")", true, globeID); globeID += 15;
            a->addNode(v);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID NUMEXPR. Missing closing bracket. )";
            }
        }
    }
    else if(program[0]=='n')
    {
        numvar(a);
    }
    else if(program[0]=='-' || init)
    {
        decnum(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID NUMEXPR. Follow language";
        }
    }
}

void decnum(node* n)
{
    //cout<<"DECNUM"<<endl;

    node* a = new node("DECNUM", false, globeID); globeID += 15;
    n->addNode(a);

    bool init = false;

    if(program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        init = true;
    }
    if(program[0]=='0')
    {
        //add errors
        node* t = new node("0", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//0
        cleanUp();

        if(program[0]=='.')
        {
            t = new node(".", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//.
            cleanUp();

            if(program[0]=='0')
            {
                t = new node("0", true, globeID); globeID += 15;
                a->addNode(t); 
                program = program.substr(1);//0
                cleanUp();

                if(program[0]=='0')
                {
                    t = new node("0", true, globeID); globeID += 15;
                    a->addNode(t); 
                    program = program.substr(1);//0
                    cleanUp();
                }
                else
                {
                    if(!errorFound)
                    {
                        errorFound=true;
                        error = "Error found. INVALID DECBUM. Follow language. 0.00";
                    }
                }
            }
            else
            {
                if(!errorFound)
                {
                    errorFound=true;
                    error = "Error found. INVALID DECBUM. Follow language. 0.00";
                }
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID DECBUM. Follow language. 0.00";
            }
        }

    }
    else if(program[0]=='-')
    {
        neg(a);
    }
    else if(init)
    {
        pos(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID DECNUM. Not in language";
        }
    }
}

void neg(node* n)
{
    //cout<<"NEG"<<endl;

    node* a = new node("NEG", false, globeID); globeID += 15;
    n->addNode(a);

    node* t = new node("-", true, globeID); globeID += 15;
    a->addNode(t);
    program = program.substr(1);//-
    cleanUp();
   
    pos(a);
}

void pos(node* n)
{
    //cout<<"POS"<<endl;

    node* a = new node("POS", false, globeID); globeID += 15;
    n->addNode(a);

    Int(a);
    if(program[0]=='.')
    {
        node* t = new node(".", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//.
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID POS. example format: 11.00.";
        }
    }
    
    if(program[0]=='0' || program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        d(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID POS. Follow language syntax. eg 11.22 not 11.";
        }
    }

    if(program[0]=='0' || program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        d(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID POS. Follow language syntax. eg 11.22 not 11.2";
        }
    }
}

void Int(node* n)
{
    //cout<<"INT"<<endl;

    node* a = new node("INT", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='1' || program[0]=='2' || program[0]=='3' || program[0]=='4' || program[0]=='5' || program[0]=='6' || program[0]=='7' || program[0]=='8' || program[0]=='9')
    {
        string place ="";
        place += program[0];
        node* t = new node(place, true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//int
        cleanUp();
        more(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID INT. Does not begin with integers 1-9.";
        }
    }
}

void boolexpr(node* n)
{
    //cout<<"BOOLEXPR"<<endl;

    node* a = new node("BOOLEXPR", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='T'|| program[0]=='F'|| program[0]=='^' || program[0]=='v'|| program[0]=='!'|| program[0]=='b')
    {
        logic(a);
    }
    else if(program[0]=='E'|| program[0]=='<'|| program[0]=='>')
    {
        cmpr(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID BOOLEXPR.";
        }
    }
}

void logic(node* n)
{
    //cout<<"LOGIC"<<endl;

    node* a = new node("LOGIC", false, globeID); globeID += 15;
    n->addNode(a);

   if( program[0]=='T')
   {
        node* t = new node("T", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//T
        cleanUp();
   }
   else if(program[0]=='F')
   {
        node* t = new node("F", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//F
        cleanUp();
   }
   else if(program[0]=='^')
   {
        node* t = new node("^", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//^
        cleanUp();

        if(program[0]== '(')
        {
            t= new node("(", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//(
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing opening bracket. '('";
            }
        }

        boolexpr(a);
        
        if(program[0]==',')
        {
            t = new node(",", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//,
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing comma. ','";
            }
        }

        boolexpr(a);

        if(program[0]==')')
        {
            t = new node(")", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing closing bracket. ')'";
            }
        }
   } 
   else if(program[0]=='v')
   {
        node* t = new node("v", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//v
        cleanUp();

        if(program[0]=='(')
        {
            t= new node("(", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//(
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing opening bracket. '('";
            }
        }

        boolexpr(a);
        if(program[0]==',')
        {
            t = new node(",", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//,
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing comma.";
            }
        }

        boolexpr(a);
        
        if(program[0]==')')
        {
            t = new node(")", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing closing bracket. ')'";
            }
        }
   }
   else if(program[0]=='!')
   {
        node* t = new node("!", true, globeID); globeID += 15;
        a->addNode(t);
        
        program = program.substr(1);//!
        cleanUp();
        if(program[0]=='(')
        {
            t= new node("(", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//(
            cleanUp();
            boolexpr(a);
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing opening bracket. '('";
            }
        }
        
        if(program[0]==')')
        {
            t = new node(")", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//)
            cleanUp();
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID LOGIC. Missing closing bracket. ')'";
            }
        }
   }
   else if(program[0]=='b')
   {
        boolvar(a);
   }
   
}

void cmpr(node* n)
{
    //cout<<"CMPR"<<endl;

    node* a = new node("CMPR", false, globeID); globeID += 15;
    n->addNode(a);

    string place ="";
    place += program[0];
    node* t = new node(place, true, globeID); globeID += 15;
    a->addNode(t);
    program = program.substr(1);//E, <, >
    cleanUp();

    if(program[0]=='(')
    {
        t = new node("(", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//(
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID CMPR. Missing opening bracket. '('";
        }
    }
    
    numexp(a);

    if(program[0]==',')
    {
        t = new node(",", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//,
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID CMPR. Missing comma.";
        }
    }

    numexp(a);
    
    if(program[0]==')')
    {
        t = new node(")", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//)
        cleanUp();
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID CMPR. Missing closing bracket. ')'";
        }
    }
  
}

void stri(node* n)
{
    // cout<<"STRI"<<endl;

    node* a = new node("STRI", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]== '"')
    {
        node* t = new node("\"", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//"
        
        for(int x=0; x< 15; x++)
        {
            if(program[0]=='"')
            {
                errorFound=true;
                error = "Error found. \" is apparently a deliminator now. Won't even check if it's length 15. Don't use it in stri";
            }
            else
            {
                c(a);
            }
        }
        
        if(program[0]=='"')
        {
            t = new node("\"", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//"
            cleanUp();
        }
        else if(program[0]=='*' || program[0]==';' || program[0]=='}' || program[0] == '$' || program[0] == ',')
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID STRI. Missing closing quotes.";
            }
        }
        else if(program[0]!='*' || program[0]!=';' || program[0]!='}' || program[0] != '$' || program[0] != ',')
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID STRI. Incorrect length. Must be 15.";
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID STRI. Incorrect length. Must be 15.";
            }
        }
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID STRI. Missing opening quotes.";
        }
    }
}

void c(node* n)
{
    node* a = new node("C", false, globeID); globeID += 15;
    n->addNode(a);

    if (isascii(program[0]))
    {
        string place ="";
        place += program[0];
        node* t = new node(place, true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//_ (ASCII)
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID C. Not a ACSII character.";
        }
    }
}

void comment(node* n)
{
    //cout<<"COMMENT"<<endl;

    node* a = new node("COMMENT", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='*')
    {
        node* t = new node("*", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//*

        for(int x=0; x< 15; x++)
        {
            if(program[0]=='*')
            {
                errorFound=true;
                error = "Error found. * is apparently a deliminator now. Won't even check if it's length 15. Don't use it in comments";
            }
            else
            {
                c(a);
            }
        }
      
        if(program[0]=='*')
        {
            t = new node("*", true, globeID); globeID += 15;
            a->addNode(t);
            program = program.substr(1);//*
            cleanUp();
        }
        else if(program[0]==';' || program[0]==','|| program[0]=='}' || program[0]=='$')
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID COMMENT. Missing closing * or .";
            }
        }
        else if(program[0]!=';' && program[0]!=','&& program[0]!='}' && program[0]!='$')
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID COMMENT. Incorrect length. Must be 15";
            }
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID COMMENT. Incorrect length. Must be 15";
            }
        }
    }
    else if(program[0]==';' || program[0]==','|| program[0]=='}' || program[0]=='$')
    {
        node* t = new node("ε", true, globeID); globeID += 15;
        a->addNode(t);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID COMMENT. Missing opening * or ; to start SEQ.";
        }
    }
}

void input(node* n)
{
    //cout<<"INPUT"<<endl;

    node* a = new node("INPUT", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='g')
    {
        node* t = new node("g", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//g
        cleanUp();
        if(program[0]== 'n')
        {
            numvar(a);
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID INPUT. INPUT starts with g followed by NUMVAR.";
            }
        }
        
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID INPUT. INPUT starts with g.";
        }
    }
    
}

void output(node* n)
{
    //cout<<"OUTPUT"<<endl;

    node* a = new node("OUTPUT", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='o')
    {
        value(a);
    }
    else if(program[0]=='r')
    {
        text(a);
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID OUTPUT. OUTPUT uses VALUE or TEXT (o NUMVAR or r STRINGV ).";
        }
    }
}

void value(node* n)
{
    //cout<<"VALUE"<<endl;

    node* a = new node("VALUE", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='o')
    {
        node* t = new node("o", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//o
        cleanUp();

        if(program[0]=='n')
        {
            numvar(a);
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID VALUE. VALUE starts with o followed by NUMVAR.";
            }
        }
        
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID VALUE. VALUE starts with o.";
        }
    }
}

void text(node* n)
{
    //cout<<"Text"<<endl;
    node* a = new node("TEXT", false, globeID); globeID += 15;
    n->addNode(a);

    if(program[0]=='r')
    {
        node* t = new node("r", true, globeID); globeID += 15;
        a->addNode(t);
        program = program.substr(1);//r
        cleanUp();

        if(program[0]=='s')
        {
            stringv(a);
        }
        else
        {
            if(!errorFound)
            {
                errorFound=true;
                error = "Error found. INVALID TEXT. TEXT starts with r followed by STRINGV.";
            }
        }
    }
    else
    {
        if(!errorFound)
        {
            errorFound=true;
            error = "Error found. INVALID TEXT. TEXT starts with r.";
        }
    }
}

void cleanUp()
{
    while(program[0]==' '|| program[0]=='\n' || program[0]=='\t' || program[0]=='\r')
    {
        program= program.substr(1);
    } 
}

void prune(node* n)
{
    n->epsilonRemove();
    n->noKids();
    n->compressing();
}

void assignScopes(node* n)
{
    n->scoping();
}

void nameErrors(node* root)
{
    if(siblingNames(root))
    {
        //cout<<"sibling do not share"<<endl;
    }
    else
    {
        //cout<<"sibling share"<<endl;
    }

    if(childNames(root))
    {
     //   cout<<"child does not have same name as parent"<<endl;
    }
    else
    {
      //  cout<<"child has same name as parent"<<endl;
    }

    if(nephewNames(root))
    {
        //cout<<"nephew does not have same name as aunt"<<endl;
    }
    else
    {
        //cout<<"nephew has same name as aunt"<<endl;
    }

    checkingCalls(root);
    warning = root->deadCode(warning);
}

void htable(node* root, table *Hash)
{
    error = root->returnErrors();
    if(error.size()>5)
    {
        Hash->addError(error);
    }
    else
    {
        root->tableO(Hash);
        Hash->addError(warning);
    }
}

bool siblingNames(node* root)
{
    return root->siblingNames();
}

bool childNames(node* root)
{
   return root->childNames();
}

bool nephewNames(node* root)
{
    return root->nephewNames();
}

void checkingCalls(node* root)
{
    root->checkingCalls(root);
}

void OutPutTable(table* Hash)
{
    string ans = "";

    ans ="<!DOCTYPE html>\n<html>\n<style>\ntable, th, td{border: 1px solid black;}\n</style>\n<body>\n<h2>Symbol Table</h2>\n<table style=\"width:100%\">\n<tr>\n<th>NodeID</th>\n<th>NodeName</th>\n<th>ScopeID</th>\n<th>ScopeName</th>\n<th>Has Value</th>\n</tr>\n";
    
    vector<string*> tab = Hash->getTable();

    for(int x=0; x< tab.size(); x++)
    {
        ans += "<tr>\n<td>";
        ans += tab[x][0];
        ans+= "</td>\n<td>";
        ans += tab[x][1];
        ans += "</td>\n<td>";
        ans += tab[x][2];
        ans += "</td>\n<td>";
        ans+= tab[x][3];
        ans += "</td>\n<td>";
        ans+= tab[x][4];
        ans+= "</td>\n</tr>\n";
    }
     
    ans += "</table>\n";
    vector<string> errorList = Hash->getErrors();
    for(int x=0; x< errorList.size(); x++)
    {
        ans+= "<h3>";
        ans+= errorList[x];
        ans+="</h3>\n";
    }
    ans+="</body>\n</html>";

    ofstream outputFile;
    outputFile.open("table.html");
    outputFile<<ans;
    outputFile.close();
}