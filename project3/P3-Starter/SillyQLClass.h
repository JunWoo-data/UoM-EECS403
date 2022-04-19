// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef _SILLYQL_H_
#define _SILLYQL_H_

#include <iostream>
#include <unordered_map>
#include "TableClass.h"
using namespace std;

class SillyQLClass {
    private:
        unordered_map<string, TableClass> tableDatabase;
        bool isQuietMode = false;
    public:
        void get_option(int argc, char** argv);
        void printProgramArgument() const {
            cout << "== Print Program Argument ==" << '\n';
            cout << "Is quiet mode: " << isQuietMode << '\n';
        }
        void readQueryFile();
        void doCreateCmd(string inCmd);
        void doInsertCmd(string inCmd);
        void doPrintCmd(string inCmd); 
        void doRemoveCmd(string inCmd);
        void doDeleteCmd(string inCmd);
        void doGenerateCmd(string inCmd);
        void doJoinCmd(string inCmd);
        bool checkTableExist(unordered_map<string, TableClass>::iterator &currentTable, 
                             string inTableName, string inCmd, bool isPrint);
        bool checkColumnExist(unordered_map<string, TableClass>::iterator &currentTable, 
                              string inColName, string inCmd, bool isPrint); 
        void printTableDatabase() const;
        string changeToFullCmd(string inCmd);
        TableEntry getValidEntry(string inColType);
        void regenerateIndex(unordered_map<string, TableClass>::iterator &currentTable, string colName);
};

#endif