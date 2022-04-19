// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include "TableClass.h"
using namespace std;

void TableClass::printTableMetadata() const {
    cout << "-- table metadata" << '\n';
    cout << "Num of Rows: " << dataMatrix.size() << '\n';
    cout << "Num of Columns: " << numOfCols << '\n';
    cout << "Column names: ";
    for (auto name : columnNames) {
        cout << name << " - ";       
    }
    cout << '\n';
    cout << "Column types: ";
    for (auto type : columnTypes) {
        cout << type << " - ";
    }
    cout << '\n';
}