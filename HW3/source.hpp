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
    Node(int lineno) : lineno(lineno) {};
    virtual ~Node()= default;
    virtual string print_Node(){return "";};
	
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
            
            Entry(string name, string type, int offset) : name(name), type(type), offset(offset) {};
            void printEntry() const
            {
                cout << "name: " << this->name << " type: " << this->type << " offset: " << this->offset << endl;
            };
        };

        shared_ptr<Table> parent;
        vector<Entry*> entries;

        Table(shared_ptr<Table> parent) : parent(parent), entries() {};

        void addEntry(string name, string type, int offset)
        {
            entries.push_back(new Entry(name, type, offset));
        };
        void printTable(){
            for(auto i : entries){
                cout << "entry num: " << i->offset << " ";
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

    Table::Entry* find(string name){
        shared_ptr<Table> curr = tableStack.top();
        while(curr != nullptr){
            Table::Entry* entry = curr->findEntry(name);
            if(entry != nullptr)
                return entry;
        }
        return nullptr;
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
    NumNode(int lineno, char* num) : Node(lineno), num(stoi(num)) {};
    string print_Node() override{
        return "INT";
    };
};

class IdNode : public Node {
    public:
    string id;
    IdNode(int lineno, string id) : Node(lineno),  id(id){};
    string print_Node() override{
        return id;
    };
};




#define YYSTYPE Node*	// Tell Bison to use STYPE as the stack type