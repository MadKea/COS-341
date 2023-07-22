/* Keabetswe Madumo u20438614*/

#ifndef TABLE_H
#define TABLE_H
#include<string>
#include<iostream>
#include<vector>

using namespace std;


class table
{
    private:
        vector<string*> tableEnviro;
        vector<string> errors;
        bool halt;
            
    public: 
        table();
        ~table();
        void addRow(string name, string id, string value, string scope, string scopeName);
        bool searchTable(string value);
        void display();
        void setValue(string val);
        string getValue(string val);
        void addError(string error);
        bool getHalt();
        void setHalt();
        table* copy();
        void transferErrors(table* sec);
        void compare(table* a, table* b);
        vector<string*> getTable();
        vector<string> getErrors();


};

#endif