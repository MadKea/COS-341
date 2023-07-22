#include "table.h"
#include <iostream>

table::table()
{
    halt = false;

}

table::~table()
{

}

void table::addRow(string name, string id, string val, string scope, string scopeName)
{
    bool ans = searchTable(val);
    if(!ans)
    {
        string *row = new string[5];
        row[0]= id;
        row[1]= val;
        row[2] = scope;
        row[3]= scopeName;
        if(name == "PROC" || val == "main")
        {
            row[4]= "null";
        }
        else
        {
            row[4]= "false";
        }
        
        tableEnviro.push_back(row);
    }   
}

void table::display()
{
    for(int x=0; x< tableEnviro.size(); x++)
    {
        cout<<tableEnviro[x][0]<<" | "<<tableEnviro[x][1]<<" | "<<tableEnviro[x][2]<<" | "<<tableEnviro[x][3]<<" | "<<tableEnviro[x][4]<<endl;
    }

    for(int x=0; x< errors.size(); x++)
    {
        cout<<errors[x]<<endl;
    }
}

bool table::searchTable(string name)
{
    for(int x=0; x< tableEnviro.size(); x++)
    {
        if(tableEnviro[x][1]==name)
        {
            return true;
        }
    }

    return false;
}

void table::setValue(string val)
{
    for(int x=0; x< tableEnviro.size(); x++)
    {
        if(tableEnviro[x][1]==val)
        {
            tableEnviro[x][4]="true";
        }
    }
}

string table::getValue(string val)
{
    for(int x=0; x< tableEnviro.size(); x++)
    {
        if(tableEnviro[x][1]==val)
        {
            return tableEnviro[x][4];
        }
    }

    return "";
}

void table::addError(string error)
{
    errors.push_back(error);
}

bool table::getHalt()
{
    return halt;
}

void table::setHalt()
{
    halt = true;
}

table* table::copy()
{
    table* cat = new table();
    for(int x=0; x< tableEnviro.size(); x++)
    {
        string*line = new string[5]; 
        line[0]=tableEnviro[x][0];
        line[1]= tableEnviro[x][1];
        line[2]= tableEnviro[x][2];
        line[3]= tableEnviro[x][3];
        line[4]= tableEnviro[x][4];

        cat->tableEnviro.push_back(line);
    }
    return cat;
}

void table::transferErrors(table* sec)
{
    for(int x=0; x< sec->errors.size(); x++)
    {
        addError(sec->errors[x]);
    }
}

void table::compare(table* a, table* b)
{
    if(a->halt == true && b->halt == true)
    {
        halt = true;
    }
    for(int x=0; x< tableEnviro.size(); x++)
    {
        if(tableEnviro[x][4]=="false")
        {
            if(a->tableEnviro[x][4]=="true" && b->tableEnviro[x][4]=="true")
            {
                tableEnviro[x][4]="true";
            }
        }
    }
}

vector<string*> table::getTable()
{
    return tableEnviro;
}

vector<string> table::getErrors()
{
    return errors;
}