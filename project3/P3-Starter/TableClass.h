// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef _TABLECLASS_H_
#define _TABLECLASS_H_

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "TableEntry.h"
using namespace std;

class TableClass {
    private:
        unordered_map<TableEntry, vector<size_t>> hashIndex;
        map<TableEntry, vector<size_t>, less<TableEntry>> bstIndex;
        vector<vector<TableEntry>> dataMatrix; 
        vector<string> columnNames;
        vector<string> columnTypes;
        size_t numOfCols;
        string indexedColName = "None";
        char indexType;

    public:
        TableClass() {}
        void printTableMetadata() const;
        void setNumOfCols(size_t inNum) {
            numOfCols = inNum;
        }
        size_t getNumOfCols() const {
            return numOfCols;
        }
        void putColName(string inName) {
            columnNames.push_back(inName);
        }
        vector<string> getColNames() const {
            return columnNames;
        }
        void putColType(string inType) {
            columnTypes.push_back(inType);
        }
        vector<string> getColTypes() const {
            return columnTypes;
        }
        vector<vector<TableEntry>>& getTableData() {
            return dataMatrix;
        }
        unordered_map<TableEntry, vector<size_t>>& getHashIndex() {
            return hashIndex;
        }
        map<TableEntry, vector<size_t>, less<TableEntry>>& getBstIndex() {
            return bstIndex;
        }
        void setIndexedColName(string inColName) {
            indexedColName = inColName;
        }
        string getIndexedColName() const {
            return indexedColName;
        }
        void setIndexType(char inType) {
            indexType = inType;
        }
        char getIndexType() const {
            return indexType;
        }
        size_t getColIndex(string inName) {
            for (size_t i = 0; i < numOfCols; i++) {
                if (columnNames[i] == inName) return i;
            }

            return size_t(-1);
        }

};

#endif