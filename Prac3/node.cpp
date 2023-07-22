/*Keabetswe Madumo u20438614*/

#include "node.h"
#include <sstream>
string toString(int );

int node::scopeIndex= 0;
string node::Errors = "";

string node::returnErrors()
{
    return Errors;
}

node::node(string name, bool terminal, int id)
{
    this->name = name;
    this->terminal = terminal;
    this->value = "";
    this->called = false;
    if(!terminal)
    {
        this->id = id;
    }

    if(name =="NUMVAR" || name == "BOOLVAR" || name == "STRINGV")
    {
        scope = 0;
        scopeName = "global";
    }
    else
    {
        scope = -1;
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
        cout<<this->name<<"("<<scope<<")"<<": ";
        for(int x=0; x< nodes.size(); x++)
        {
            cout<<nodes[x]->name<<" ";
        }

        if(value != "")
        {
            cout<<value<<" ";
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

bool node::epsilonRemove()
{

    if(terminal && this->name =="ε")
    {
        return true;
    }
    else if(terminal)
    {
        return false;
    }


    if(nodes.size()==0)
    {
        return true;
    }

    bool change = true;
    while(change)
    {
        change = false;
        for(int x=0; x< nodes.size(); x++)
        {
            if(nodes[x]->epsilonRemove())
            {
                nodes.erase(nodes.begin()+x);
                change =true;
                break;
            }
        }
    }

    return false;
}

void node::noKids()
{
    bool changed = true;

    while(changed)
    {
        changed = false;
        for(int x=0; x< nodes.size(); x++)
        {
            if(nodes[x]->terminal != true && nodes[x]->nodes.size()==0)
            {
                nodes.erase(nodes.begin()+x);
                changed = true;
                break;
            }
        }
    }

    for(int x=0; x< nodes.size(); x++)
    {
        nodes[x]->noKids();
    }
    
}

void node:: compressing()
{
    if(name == "PROC")
    {
        string v = "p";
        v += nodes[1]->compress("");
        value = v;
        nodes.erase(nodes.begin() + 1);
    }
    if(name == "CALL")
    {
        string c = "p";
        c+= nodes[2]->compress("");
        nodes.erase(nodes.begin()+1);
        nodes.erase(nodes.begin()+1);
        nodes[0]->name = c;
    }
    if(name== "NUMVAR" || name== "BOOLVAR" || name== "STRINGV" || name== "DECNUM" || name == "STRI" || name == "COMMENT")
    {
        value = this->compress("");
        while(nodes.size()>0)
        {
            nodes.erase(nodes.begin());
        }
    }
    for(int x=0; x< nodes.size(); x++)
    {
       nodes[x]->compressing();
    }
}

string node::compress(string s)
{
    for(int x=0; x< nodes.size(); x++)
    {
        s = nodes[x]->compress(s);
    }

    if(terminal)
    {
        s+= name;
    }
    
    return s;
}


void node::scoping()
{
    scopeIndex++;

    node* curr;
    if(name == "root")
    {
        curr = nodes[0];
        curr->scope = 0;
        curr->scopeName = "global";
        curr->value = "main";
    }
    else
    {
        curr = nodes[0]->nodes[2];
    }

    curr->nodes[0]->scopeI(scopeIndex);
    node* next = curr;

    while(next->nodes.size()==2)
    {
        next= next->nodes[1];
        next->scope = scopeIndex;
        next->nodes[0]->scope = scopeIndex;
        next->nodes[0]->nodes[2]->scope= scopeIndex;
        if(scopeIndex==1)
        {
            next->nodes[0]->scopeName = "main";
            next->nodes[0]->nodes[2]->scopeName= "main";
        }
        else
        {
            next->nodes[0]->scopeName = nodes[0]->value;
           //cout<<next->nodes[0]->nodes[2]->name<<" ************************** "<<nodes[0]->name<<endl;
            next->nodes[0]->nodes[2]->scopeName= nodes[0]->value;
        }
        //cout<<"!!!!!!!!!!!!!!!!!! "<<next->name<<endl;
    }

    next = curr;
    while(next->nodes.size()==2)
    {
        next = next->nodes[1];
        //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<next->name<<endl;
        next->scoping();

    }
}

void node::scopeI(int scopes)
{
    if(scope != 0)
    {
        this->scope = scopes;
        //cout<<name<<" ####################### "<<scopes<<endl;
    }

    for(int x=0; x< nodes.size(); x++)
    {
        nodes[x]->scopeI(scopes);
    }
}
bool node:: siblingNames()
{
    node* curr;
    node* p;
    if(this->name == "root")
    {
        curr = nodes[0];
        p=curr;
        //cout<<curr->name<<endl;

    }
    else
    {
        curr = nodes[0]->nodes[2];
        p = nodes[0];
    }

    string names ="";
    while(curr->nodes.size()==2)
    {
        names += curr->nodes[1]->getProcName();
        names += ",";
        curr= curr->nodes[1];
    }
    if(checkNames(names))
    {
        Errors += "Naming Convention Error: Siblings within the same scope (";
        Errors += p->value;
        Errors += ") have identical names.\n";
        return false;
    }

    curr = nodes[0];
    while(curr->nodes.size()==2)
    {
        if(!(curr->nodes[1]->siblingNames()))
        {
            return false;
        }
        curr = curr->nodes[1];
    }
    
    return true;
}

string node::getProcName()
{
    node* curr = nodes[0];
   // cout<<curr->value<<endl;
    return curr->value;
}

bool node::checkNames(string names)
{
    string n;
    string given =names;
  
    while(given != "")
    {
        int i=0;
        while(given[i]!=',')
        {
            i++;
        }

        n = given.substr(0,i+1);
        given.erase(given.begin()+0, given.begin()+i+1);
        size_t duplicate= given.find(n);
        //cout<<given<<" ****** "<<n<<endl;
        if(duplicate != string::npos)
        {
            return true;
        }
    }
    
    return false;
}

bool node::childNames()
{
    if(name == "root")
    {
        node* curr = nodes[0];
        while(curr->nodes.size()==2)
        {
            curr= curr->nodes[1];
            if(!(curr->childNames()))
            {
                return false;
            }
            
        }
    }
    else
    {
        node* curr = nodes[0];
        string parent = curr->value;
        string kids ="";
        curr = curr->nodes[2];
        while(curr->nodes.size()==2)
        {
            kids += curr->nodes[1]->getProcName();
            kids += ",";
            curr= curr->nodes[1];
        }
        size_t duplicate= kids.find(parent);
       // cout<<kids<<" ****** "<<parent<<endl;
        if(duplicate != string::npos)
        {
            Errors += "Naming Convention Error: Child has same name as parent. Scope (";
            Errors += parent;
            Errors += ").\n";
            return false;
        }

    }

    return true;
}

bool node::nephewNames()
{
    node* curr;
    if(name=="root")
    {
        curr = nodes[0];

    }
    else
    {
        curr = nodes[0]->nodes[2];
    }

    node* parent = curr;
    while(curr->nodes.size()==2)
    {
        if(!(parent->gatherNephews(curr->nodes[1])))
        {
            Errors += "Naming Convention Error: Node has same name as nephew.(";
            Errors += curr->nodes[1]->getProcName();
            Errors += " Scope: ";
            Errors += toString(curr->nodes[1]->scope);
            Errors += ").\n";
            return false;
        }
        curr= curr->nodes[1];
        if(!(curr->nephewNames()))
        {
            return false;
        }
    }
    
    return true;
}

bool node:: gatherNephews(node* main)
{
    node* curr = nodes[1];
    string nephews = "";
    bool cont = true;
    while(cont)
    {
        if(curr!=main)
        {
            node* niece = curr->nodes[0]->nodes[2];

            while(niece->nodes.size()==2)
            {
                nephews += niece->nodes[1]->getProcName();
                nephews += ",";
                niece= niece->nodes[1];
            }
        }

        
        if(curr->nodes.size()==2)
        {
            curr= curr->nodes[1];
        }
        else{
            cont = false;
        }
    }

    string aunt = main->getProcName();

    size_t duplicate= nephews.find(aunt);
    //cout<<nephews<<" ****** "<<aunt<<endl;
    if(duplicate != string::npos)
    {
        return false;
    }


    return true;
}

void node::checkingCalls(node* root)
{
    node* curr;
    if(name =="root")
    {
        curr = nodes[0];
    }
    else
    {
        curr = this;
    }

    vector<string> calls = curr->getCalls();

    if(name != "root")
    {
        string tab = "";
        string sisters = root->getSisters(curr->scope, tab);

        bool noChange = true;
        int j=0;
        while(noChange && j< calls.size())
        {
            size_t found= sisters.find(calls[j]);

            if(found == string::npos)
            {
                if(j == calls.size())
                {
                    noChange=false;
                }
                j++;
            }
            else
            {
                root->checkSister(curr->scope, calls[j]);
                calls.erase(calls.begin()+j);
                j=0;

            }
        }
   
        noChange = true;
        while(noChange)
        {
            int found = -1;
            for(int x=0; x< calls.size(); x++)
            {
                if(calls[x]==curr->value)
                {
                    found = x;
                }
            }

            if(found != -1)
            {
                curr->called = true;
                calls.erase(calls.begin()+found); 
            }
            else
            {
                noChange = false;
            }
        }
    }
    string kids="";
    node* kid = curr;

    while(kid->nodes.size()==2)
    {
        kid = kid->nodes[1];
        kids = kid->getProcName();
        bool noChange = true;
        while(noChange)
        {
            int found = -1;
            for(int x=0; x< calls.size(); x++)
            {
                if(calls[x]==kids)
                {
                    found = x;
                }
            }

            if(found != -1)
            {
                kid->nodes[0]->called = true;
                calls.erase(calls.begin()+found); 
            }
            else
            {
                noChange = false;
            }
        }
    }

    for(int x=0; x< calls.size(); x++)
    {
        Errors += "Error: The procedure [";
        Errors += calls[x];
        Errors += "] called here has no corresponding declaration in this scope!\n";
    }

    node* next = curr;

    while(next->nodes.size()==2)
    {
        next = next->nodes[1];
        next->nodes[0]->nodes[2]->checkingCalls(root);

    }

}

vector<string> node::getCalls()
{
    vector<string> calls;

    node* curr = nodes[0];
    node* instr;
    bool cont =true;

    while(cont)
    { 
        instr = curr->nodes[0]->nodes[0];
        if(instr->name=="CALL")
        {
            calls.push_back(instr->nodes[0]->name);

        } 

        if(curr->nodes.size()==2 && curr->nodes[1]->name == "SEQ")
        {
            curr = curr->nodes[1]->nodes[1];
        }
        else if(curr->nodes.size()==3 && curr->nodes[2]->name == "SEQ")
        {
            curr = curr->nodes[2]->nodes[1];
        }
        else
        {
            cont = false;
        }
    }
   
    return calls;

}

string node::getSisters(int i, string list)
{
    if(name=="PROC" && scope==i)
    {
        list += value;
    }

    for(int x=0; x< nodes.size(); x++)
    {
        list = nodes[x]->getSisters(i, list);
    }

    return list;
}

void node::checkSister(int scp, string nme)
{
    if(name=="PROC" && scope==scp && value == nme)
    {
        called = true;
    }

    for(int x=0; x< nodes.size(); x++)
    {
        nodes[x]->checkSister(scp, nme);
    }
}

string node::deadCode(string w)
{
    bool entered = false;
    string warning = "";
    warning += "<p>";
    if(name=="PROC" && !called)
    { 
        entered = true;
        warning += "Dead Code Warnging- The procedure(";
        warning += value;
        warning+=") declared here(";
        warning += scopeName;
        warning+=") is not called from anywhere within the scope.";
    }

    if(entered)
    {
        warning += "</p>\n";
        w+= warning;
    }

    for(int x=0; x< nodes.size(); x++)
    {
        w=nodes[x]->deadCode(w);
    }

    return w;
    
}

string node::table(string ans)
{
    if(name=="PROC" || name=="NUMVAR"|| name=="BOOLVAR" || name == "STRINGV")
    {
        ans += "<tr>\n<td>";
        ans += toString(id);
        ans+= "</td>\n<td>";
        ans+= name;
        ans+= "--";
        ans+= value;
        ans += "</td>\n<td>";
        ans += toString(scope);
        ans += "</td>\n<td>";
        ans+= scopeName;
        ans+= "</td>\n</tr>\n";
    }
    else if(name =="PROGR" && value=="main")
    {
        ans += "<tr>\n<td>";
        ans += toString(id);
        ans+= "</td>\n<td>";
        ans+= name;
        ans+= "--";
        ans+= value;
        ans += "</td>\n<td>";
        ans += toString(scope);
        ans += "</td>\n<td>";
        ans+= scopeName;
        ans+= "</td>\n</tr>\n";
    }

    for(int x=0; x< nodes.size(); x++)
    {
        ans = nodes[x]->table(ans);
    }
    
    return ans;
}

string toString(int i)
{
    stringstream ss;
    ss<<i;
    string s;
    ss>>s;
    return s;
}