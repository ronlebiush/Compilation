#pragma once

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stack>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Node
{
public:
    string typeString;
    Node():typeString(""){};
    Node(string type):typeString(type){};
    virtual ~Node()= default;
	
};

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
            void printEntry()
            {
                cout << "name: " << this->name << "type: " << this->type->typeString << "offset: " << this->offset << "\n";
            };
        };

        shared_ptr<Table> parent;
        vector<Entry> entries;

        Table(shared_ptr<Table> parent) : parent(parent), entries() {};

        void addEntry(string name ,const shared_ptr<Node>& Type,int offset)
        {
            entries.push_back(Entry(name, Type, offset));
        };
        void printTable(){
            for(auto i : entries){
                cout << "entry num: " << i.offset;
                i.printEntry();
            }
        };
        
    }; 
    stack<shared_ptr<Table>> tableStack;
    stack<int> offsetsStack;
    int curr_offset = 0;
    shared_ptr<Table> root = nullptr;
    shared_ptr<Table> addTable(shared_ptr<Table>& parent)
    {
        shared_ptr<Table> newTable = make_shared<Table>(parent);
        curr_offset++;
        tableStack.push(newTable);
        return newTable;
    };

    void printSymtab(shared_ptr<Table> table)
    {
        shared_ptr<Table> curr = table;
        while(curr != root) {
            cout << "found table\n";
            curr->printTable();
            curr = curr->parent;
        }
        cout << "reached root\n";
    };
    /*void iteratorPrint(){
        for(auto i:tableStack )
            (*i)->print();
    };*/

};

class Num : public Node {
    public:
    Num(string typeString) : Node(typeString) {};
    
};

class Bool : public Node {
    bool token;

};


#define YYSTYPE Node*	// Tell Bison to use STYPE as the stack type