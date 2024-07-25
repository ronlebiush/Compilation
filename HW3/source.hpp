#pragma once

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stack>
#include <vector>
#include <memory>
#include <iostream>
#include "hw3_output.hpp"

using namespace std;

class Node
{
public:
    int lineno;
    string type = "";
    Node(int lineno) : lineno(lineno) {};
    virtual ~Node()= default;
    virtual string print_Node(){return type;};
	
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
            int offset;
            string name;
            string type;
            string funcArg;
            
            Entry(string name, string type, int offset, string funcArg) : name(name), type(type), offset(offset), funcArg(funcArg) {};
            void printEntry() const
            {
                if(this->funcArg != ""){
                    output::printID(name, offset, output::makeFunctionType(funcArg, type));
                }
                else output::printID(name, offset, type);
            };
        };

        shared_ptr<Table> parent;
        vector<Entry*> entries;
        bool isWhile = false;

        Table(shared_ptr<Table> parent) : parent(parent), entries() {};

        void addEntry(string name, string type, int offset, string funcArg="")
        {
            entries.push_back(new Entry(name, type, offset, funcArg));
        };
        void printTable(){
            for(auto i : entries){
                i->printEntry();
            }
        };

        Entry* findEntry(string name){
            for(auto i : entries){
                if(i->name == name)
                    return i;
            }
            return nullptr;
        }
        
    }; 

    stack<shared_ptr<Table>> tableStack;
    stack<int> offsetsStack;
    int curr_offset = 0;
    shared_ptr<Table> root = nullptr;
    

    Symtab(){
        offsetsStack.push(0);
        shared_ptr<Table> newTable = make_shared<Table>(root);
        tableStack.push(newTable);

        newTable->addEntry("print", "VOID", 0, "STRING");
        newTable->addEntry("printi", "VOID", 0, "INT");
        newTable->addEntry("readi", "INT", 0, "INT");
    }
    
    shared_ptr<Table> addTable(shared_ptr<Table>& parent)
    {
        shared_ptr<Table> newTable = make_shared<Table>(parent);
        //curr_offset++;
        offsetsStack.push(offsetsStack.top());
        tableStack.push(newTable);
        return newTable;
    };

    void printSymtab(shared_ptr<Table> table)
    {
        shared_ptr<Table> curr = table;
        while(curr != root) {
            curr->printTable();
            curr = curr->parent;
        }
    };

    Table::Entry* find(string name){
        shared_ptr<Table> curr = tableStack.top();
        while(curr != nullptr){
            Table::Entry* entry = curr->findEntry(name);
            if(entry != nullptr)
                return entry;
            curr = curr->parent;
        }
        return nullptr;
    }
    bool isInWhile(){
        shared_ptr<Table> curr = tableStack.top();
        while(curr != nullptr){
            if(curr->isWhile)
                return true;
            curr = curr->parent;
        }
        return false;
    }
    
    
    /*void iteratorPrint(){
        for(auto i:tableStack )
            (*i)->print();
    };*/

};

class TypeNode : public Node {
    public:
    string typeString;
    TypeNode(int lineno, string typeString) : Node(lineno),  typeString(typeString){};
    string print_Node() override{
        return typeString;
    };

};

class NumNode : public Node {
    public:
    int num;
    string type = "INT";
    NumNode(int lineno, int num) : Node(lineno), num(num) {};
    string print_Node() override{
        return type;
    };
};

class IdNode : public Node {
    public:
    string id;
    string type;
    IdNode(int lineno, string id) : Node(lineno),  id(id){};
    string print_Node() override{
        return type;
    };
};




#define YYSTYPE Node*	// Tell Bison to use STYPE as the stack type