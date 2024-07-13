#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stack>
#include <vector>
#include <memory>
using namespace std;

class Symtab 
{
    public:
    class Table
    {
        public:
        class Entry
        {
            public:
            string name;
            shared_ptr<Node> type;
            int offset;

            Entry(string name, shared_ptr<Node> type, int offest) : name(name), type(type), offset(offset) {};
        };

        shared_ptr<Table> parent;
        vector<Entry> entries;

        Table(shared_ptr<Table> parent) : parent(parent), entries() {};

        void addEntry(string name ,const shared_ptr<Node>& Type,int offset)
        {
            entries.push_back(Entry(name, Type, offset));
        };
        
    }; 
    stack<shared_ptr<Table>> tableStack;
    stack<int> offsetsStack;
    int curr_offset;
    void addTable(shared_ptr<Table>& parent)
    {
        tableStack.push(make_shared<Table>(parent));

    };

};

Symtab symtable;

class Node
{
	
};
class Num : public Node {
    
};

class Bool : public Node {

};

#define YYSTYPE Node*	// Tell Bison to use STYPE as the stack type