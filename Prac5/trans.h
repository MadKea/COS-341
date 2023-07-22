/* Keabetswe Madumo u20438614*/

#ifndef TRANS_H
#define TRANS_H
#include "table.h"
#include "node.h"

using namespace std;


class trans
{
    private:
        table* symbolT;
        node* AST;
        int lineNum = 10;
        string basic= "";
        int tempV=11;
        
    public:
        trans(table* symbolT, node* tree);
        ~trans();
        void TRANS(node* curr);
        void TRANSINSTR(node* curr);
        void TRANSASSIGN(node* curr);
        string TRANSNUMEXPR(node* curr);
        string TRANSBOOLEXPR(node* curr);
        string TRANSLOGIC(node* curr);
        string TRANSCMPR(node* curr);
        void TRANSINPUT(node* curr);
        void TRANSOUTPUT(node* curr);
        void TRANSLOOP(node* curr);
        void TRANSBRANCH(node* curr);
        void TRANSCALL(node* curr);
        string findAndSetLine(string name, int sameParent, string isChild, node* curr, string line);
        string lining();
        string giveTemP();
      
};

#endif