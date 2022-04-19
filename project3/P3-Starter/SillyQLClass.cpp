// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include "SillyQLClass.h"
#include <getopt.h>
#include <string>
#include <algorithm> 
#include <unordered_map>
#include <map>
#include "TableClass.h"
//using namespace std;

class WhereCondition {
    private:
        size_t colIndex;
        char operation;
        TableEntry valueCompared;

    public:
        WhereCondition(size_t inIndex, char inOp, TableEntry inValue) :
        colIndex{inIndex}, operation{inOp}, valueCompared{inValue} {}

        bool operator() (vector<TableEntry> &currentRow) {
            if (operation == '=') {
                return currentRow[colIndex] == valueCompared;
            }

            else if (operation == '<') {
                return currentRow[colIndex] < valueCompared;
            }

            else {
                return currentRow[colIndex] > valueCompared;
            }
        }
};

void SillyQLClass::get_option(int argc, char** argv) {
    int option_index = 0, option = 0;  
    opterr = false;

    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "quiet", no_argument, nullptr, 'q' },
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hq", longOpts, 
            &option_index)) != -1) {
        switch (option) {
            case 'h': {
                std::cout << "Some help sentence" << '\n';
                exit(0);    
            } // case h    

            case 'q': {
                isQuietMode = true;
                break;
            } // case q

            default: {
                std::cerr << "ERROR: Invalid options." << '\n';  
                exit(0);  
            } // case default
        } // switch
    } // while
} // get_option()

void SillyQLClass::readQueryFile() {
    string cmd; 
    string junk;
    string forComment;

    do {     
        if (std::cin.fail()) {
            std::cerr << "ERROR: Reading from cin has failed" << '\n';
            exit(1);
        } // if std::cin fail

        std::cout << "% ";
        std::cin >> cmd;   

        forComment = cmd[0];   

        if (cmd == "QUIT") {
            std::cout << "Thanks for being silly!" << '\n';
        } // if cmd == QUIT

        else if (forComment.compare("#") == 0) {
            getline(std::cin, junk);
        } // else if #

        else if (cmd == "CREATE") {
            doCreateCmd(cmd);
        } // else if cmd == CREATE 

        else if (cmd == "INSERT") {
            doInsertCmd(cmd);
        } // else if cmd == INSERT

        else if (cmd == "PRINT") {
            doPrintCmd(cmd);
        }

        else if (cmd == "REMOVE") {
            doRemoveCmd(cmd);
        }

        else if (cmd == "DELETE") {
            doDeleteCmd(cmd);
        }

        else if (cmd == "GENERATE") {
            doGenerateCmd(cmd);
        }

        else if (cmd == "JOIN") {
            doJoinCmd(cmd);
        }

        else {
            std::cerr << "Error: Unknown command line option" << '\n';
            getline(std::cin, junk);
        }
    } // do
    while (cmd != "QUIT");
} // readQueryFile()

void SillyQLClass::doCreateCmd(string inCmd) { 
    string junk;
    string tableName;
    size_t numOfCols;
    string colName;
    string colType;
    TableClass newTable;
    
    std::cin >> tableName;
    auto currentTable = tableDatabase.find(tableName);

    if (checkTableExist(currentTable, tableName, inCmd, false)){
        std::cerr << "Error during CREATE: Cannot create already " 
             << "existing table " << tableName << '\n';
        getline(std::cin, junk);  
    } // if: table with same name exists in data bas

    else {
        std::cout << "New table " << tableName << " with column(s) ";
        
        std::cin >> numOfCols;
        newTable.setNumOfCols(numOfCols);
        
        for (size_t i = 0; i < numOfCols; i++) {
            std::cin >> colType;   
            newTable.putColType(colType);
        } // for: read all column typ
        for (size_t i = 0; i < numOfCols; i++) {
            std::cin >> colName;
            std::cout << colName << " ";
            newTable.putColName(colName);
        } // for: read all column name
        
        std::cout << "created" << '\n';
        tableDatabase.insert({tableName, newTable});
    } // else: table with same name does not exist in data base    
} // doCreateCmd()

void SillyQLClass::doInsertCmd(string inCmd) { 
    string junk;
    string tableName;
    size_t numOfRows;
    bool result;

    std::cin >> junk >> tableName >> numOfRows;

    auto currentTable = tableDatabase.find(tableName);
    
    result = checkTableExist(currentTable, tableName, inCmd, true);

    if (result) {
        cin >> junk;
        
        size_t currentNumOfRows = currentTable->second.getTableData().size();
        size_t currentNumOfCols = currentTable->second.getNumOfCols();
        size_t resultNumOfRows = currentNumOfRows + numOfRows;
        
        currentTable->second.getTableData().resize(resultNumOfRows);

        for (size_t i = currentNumOfRows; i < resultNumOfRows; i++) { 
            auto &newRow = currentTable->second.getTableData()[i];

            for (size_t j = 0; j < currentNumOfCols; j++) {
                string currentColType = currentTable->second.getColTypes()[j];
                //TableEntry newEntry = getValidEntry(currentColType);
                
                newRow.reserve(currentNumOfCols);
                newRow.emplace_back(getValidEntry(currentColType));           
            } // for j = col
        } // for i = row
        
     
        std::cout << "Added " << numOfRows << " rows to " << tableName 
             << " from position " << currentNumOfRows << " to " 
             << resultNumOfRows - 1 << '\n';

        // do regenerate if needed
        if (currentTable->second.getIndexedColName() != "None") {
            regenerateIndex(currentTable, currentTable->second.getIndexedColName());
        } 
    } // if: table with same name exists in database

    else getline(std::cin, junk);
} // doInsertCmd()

void SillyQLClass::doPrintCmd(string inCmd) { 
    string junk;
    string tableName;
    size_t numOfPrintCols;
    string colName;
    vector<size_t> colIndexes;
    bool result = true;
    size_t printedNumRows = 0;

    std::cin >> junk >> tableName >> numOfPrintCols;
    
    auto currentTable = tableDatabase.find(tableName);

    result = checkTableExist(currentTable, tableName, inCmd, true);
    
    if (result == true) {
        for (size_t i = 0; i < numOfPrintCols; i++) {
            std::cin >> colName;

            result = checkColumnExist(currentTable, colName, inCmd, true) ;

            if (result == false) {
                getline(std::cin, junk);
                break;
            }

            colIndexes.emplace_back(tableDatabase.find(tableName)->second.getColIndex(colName));
        } // for check column name  
    } // if table exist

    else getline(std::cin, junk);

    if (result == true) {
        
        size_t numOfRows = currentTable->second.getTableData().size();

        std::cin >> junk;

        if ((junk == "ALL")) {
            if(!isQuietMode) {
                for (auto i : colIndexes) {
                    std::cout << currentTable->second.getColNames()[i] << " ";
                } // for: print column names
                std::cout << '\n';

                for (size_t i = 0; i < numOfRows; i++) {
                    for (auto j : colIndexes) {
                        std::cout << currentTable->second.getTableData()[i][j] << " ";        
                    } // for j = col
                std::cout << '\n';
                } // for i = row
            } // if not quiet mode

            printedNumRows = numOfRows;

            std::cout << "Printed " << printedNumRows << " matching rows from " << tableName
                      << '\n';
        } // if print all

        else if ((junk == "WHERE")) {
            string whereColName;
            char operation;
            bool result;

            std::cin >> whereColName >> operation;

            result = checkColumnExist(currentTable, whereColName, inCmd, true);

            if (result) {
                size_t colIndex = currentTable->second.getColIndex(whereColName);
                string currentColType = currentTable->second.getColTypes()[colIndex];
                TableEntry comparedValue = getValidEntry(currentColType);
                WhereCondition condition(colIndex, operation, comparedValue);
                bool isCondition;

                if (!isQuietMode) {
                    for (auto i : colIndexes) {
                        std::cout << currentTable->second.getColNames()[i] << " ";
                    } // for: print column names
                    std::cout << '\n';    
                } // if not quiet mode
                
                if ((currentTable->second.getIndexedColName() == whereColName) &&
                    (currentTable->second.getIndexType() == 'b')){
                    if (operation == '=') {
                        auto condition = currentTable->second.getBstIndex().find(comparedValue);

                        if (condition != currentTable->second.getBstIndex().end()) {
                            for (size_t i = 0; i < condition->second.size(); i++) {
                                if (!isQuietMode) {
                                    for (auto j : colIndexes) {
                                        std::cout << currentTable->second.getTableData()[condition->second[i]][j] << " ";        
                                    } // for j = col
                                    std::cout << '\n';  
                                } // if not quiet mode   

                                printedNumRows += 1; 
                            } // for i: rows
                        } // if there are rows satisfing condition
                    } // if operation == '='

                    else if (operation == '>') {
                        auto conditionUpper = currentTable->second.getBstIndex().upper_bound(comparedValue);

                        for (auto i = conditionUpper; i != currentTable->second.getBstIndex().end(); i++) {
                            printedNumRows += i->second.size();
                            for (size_t j = 0; j < i->second.size(); j++) {
                                if (!isQuietMode) {
                                    for (auto k : colIndexes) {
                                        std::cout << currentTable->second.getTableData()[i->second[j]][k] << " ";        
                                    } // for k = col
                                    std::cout << '\n';  
                                } // if not quiet mode     
                            } // for j: rows
                        } // for condition
                    } // else if operation == '>'

                    else {
                        auto conditionLower = currentTable->second.getBstIndex().lower_bound(comparedValue);

                        for (auto i = currentTable->second.getBstIndex().begin(); i != conditionLower; i++) {
                            printedNumRows += i->second.size();
                            for (size_t j = 0; j < i->second.size(); j++) {
                                if (!isQuietMode) {
                                    for (auto k : colIndexes) {
                                        std::cout << currentTable->second.getTableData()[i->second[j]][k] << " ";        
                                    } // for k = col
                                    std::cout << '\n';  
                                } // if not quiet mode     
                            } // for j: rows
                        } // for condition
                    } // else operation == '<'
                } // if col for where has bst index

                else if ((currentTable->second.getIndexedColName() == whereColName) &&
                         (currentTable->second.getIndexType() == 'h') &&
                         (operation == '=')) {
                    auto condition = currentTable->second.getHashIndex().find(comparedValue);

                    if (condition != currentTable->second.getHashIndex().end()) {
                        for (size_t i = 0; i < condition->second.size(); i++) {
                            if (!isQuietMode) {
                                for (auto j : colIndexes) {
                                    std::cout << currentTable->second.getTableData()[condition->second[i]][j] << " ";        
                                } // for j = col
                                std::cout << '\n';  
                            } // if not quiet mode   

                            printedNumRows += 1; 
                        } // for i: rows
                    } // if there are rows satisfing condition
                } // else if col for where has hash index & operation == '='

                else {
                    for (size_t i = 0; i < numOfRows; i++) {
                        isCondition = condition(currentTable->second.getTableData()[i]);

                        if (isCondition) {
                            if (!isQuietMode) {
                                for (auto j : colIndexes) {
                                    std::cout << currentTable->second.getTableData()[i][j] << " ";        
                                } // for j = col
                                std::cout << '\n';  
                            }

                            printedNumRows ++;
                        } // if condition is true
                    } // for i: row
                } // all other else cases

                std::cout << "Printed " << printedNumRows << " matching rows from " << tableName
                      << '\n';
            } // if col for where exist

            else getline(cin, junk);
        } // else if print where
    } // if correct command line
} // doPrintCmd()

void SillyQLClass::doRemoveCmd(string inCmd) {
    string tableName;
    bool result;

    std::cin >> tableName;

    auto currentTable = tableDatabase.find(tableName);
    result = checkTableExist(currentTable, tableName, inCmd, true);

    if (result) {
        tableDatabase.erase(tableName);

        std::cout << "Table " << tableName << " deleted" << '\n';
    }
} // doRemoveCmd

void SillyQLClass::doDeleteCmd(string inCmd) {
    string junk;
    string tableName;
    string colName;
    char operation;
    bool result = false;

    std::cin >> junk >> tableName >> junk >> colName >> operation;
    
    auto currentTable = tableDatabase.find(tableName);

    result = checkTableExist(currentTable, tableName, inCmd, true);

    if (result) {
        result = checkColumnExist(currentTable, colName, inCmd, true);
    } // if table exist -> check column exist
    
    size_t originalNumRows = currentTable->second.getTableData().size();

    if (result) {
        size_t colIndex = currentTable->second.getColIndex(colName);
        string currentColType = currentTable->second.getColTypes()[colIndex];
        TableEntry newEntry = getValidEntry(currentColType);

        currentTable->second.getTableData().erase(std::remove_if(currentTable->second.getTableData().begin(),
                                                                 currentTable->second.getTableData().end(),
                                                                 WhereCondition(colIndex, operation, newEntry)),
                                                  currentTable->second.getTableData().end());

        std::cout << "Deleted " << originalNumRows 
                              - currentTable->second.getTableData().size()
             << " rows from " << tableName << '\n';

        // do regenerate if needed
        if (currentTable->second.getIndexedColName() != "None") {
            regenerateIndex(currentTable, currentTable->second.getIndexedColName());
        }  
    } // if talbe & column both exist

    else getline(std::cin, junk);
} // doDeleteCmd

void SillyQLClass::doGenerateCmd(string inCmd) {
    string junk;
    string tableName;
    string indexType;
    string colName;
    bool result;

    std::cin >> junk >> tableName >> indexType >> junk >> junk >> colName;

    auto currentTable = tableDatabase.find(tableName);

    result = checkTableExist(currentTable, tableName, inCmd, true);

    if (result) {
        result = checkColumnExist(currentTable, colName, inCmd, true);
    } // if table exist -> check col exist

    if (result) {
        currentTable->second.setIndexedColName(colName);
        if (indexType == "hash") currentTable->second.setIndexType('h');
        else currentTable->second.setIndexType('b');
        
        currentTable->second.getHashIndex().clear();
        currentTable->second.getBstIndex().clear();

        size_t numRows = currentTable->second.getTableData().size();
        size_t colIndex = currentTable->second.getColIndex(colName);
        
        if (indexType == "hash") {
           for (size_t i = 0; i < numRows; i++) {
               currentTable->second.getHashIndex()
                            [currentTable->second.getTableData()[i][colIndex]]
                            .emplace_back(i);     
           }
        } // if index type = hash

        else {
            for (size_t i = 0; i < numRows; i++) {
               currentTable->second.getBstIndex()
                            [currentTable->second.getTableData()[i][colIndex]]
                            .emplace_back(i);
           }
        } // if index type = bst

        std::cout << "Created " << indexType << " index for table " << tableName 
             << " on column " << colName << '\n';
    } // if table & column exist
}

void SillyQLClass::regenerateIndex(unordered_map<string, TableClass>::iterator &currentTable, 
                                   string colName) {
    size_t numRows = currentTable->second.getTableData().size();
    size_t colIndex = currentTable->second.getColIndex(colName);

    if (currentTable->second.getIndexType() == 'h') {
        for (size_t i = 0; i < numRows; i++) {
            currentTable->second.getHashIndex()
                         [currentTable->second.getTableData()[i][colIndex]]
                         .emplace_back(i);     
        } // for i: rows
    } // if col has hash index   

    else {
        for (size_t i = 0; i < numRows; i++) {
            currentTable->second.getBstIndex()
                         [currentTable->second.getTableData()[i][colIndex]]
                         .emplace_back(i);
        }
    } // else col has bst index
} // regenerateIndex

void SillyQLClass::doJoinCmd(string inCmd) {
    string junk;
    string tableName1;
    string tableName2;
    string colName1;
    string colName2;
    char operation;
    size_t numPrintCol;
    unordered_map<string, TableClass>::iterator currentTable;
    string colNamePrinted;
    vector<string> colNamePrintedVector;
    char tableSelection;
    vector<char> tableSelectionVector;
    bool result = false;
    map<TableEntry, vector<size_t>> table2Index;
    int numPrintedRows = 0;

    std::cin >> tableName1 >> junk >> tableName2 >> junk 
        >> colName1 >> operation >> colName2;
    
    auto table1 = tableDatabase.find(tableName1);
    auto table2 = tableDatabase.find(tableName2);
    
    // == Error check == //
    // -- check table1 and colname1 exist -- //
    result = checkTableExist(table1, tableName1, inCmd, true);

    if (result) {
        result = checkColumnExist(table1, colName1, inCmd, true);
    } // if table1 exist -> check col1 exist

    // -- check table2 and colname2 exist -- //
    if (result) {
        result = checkTableExist(table2, tableName2, inCmd, true);

        if (result) {
            result = checkColumnExist(table2, colName2, inCmd, true);
        } // if table2 exist -> check col2 exist
    } // if table1, col1 exist

    if (!result) getline(cin, junk);

    // -- check print column exist -- //
    if (result) {
        std::cin >> junk >> junk >> numPrintCol;

        for (size_t i = 0; i < numPrintCol; i++) {
            std::cin >> colNamePrinted >> tableSelection;
            
            if (tableSelection == '1') currentTable = table1;
            else currentTable = table2;

            result = checkColumnExist(currentTable, colNamePrinted, inCmd, true);

            if (!result) {
                getline(std::cin, junk);
                return;
            } // if printed column not in selected table
            
            else {
                colNamePrintedVector.emplace_back(colNamePrinted);    
                tableSelectionVector.emplace_back(tableSelection);
            }
        } // for number of printed cols
    } // if table1, col1 exist & table2, col2 exist

    if (result) {
        if (!isQuietMode) {
            for (size_t i = 0; i < numPrintCol; i++) {
                std::cout << colNamePrintedVector[i] << " ";
            } // for: print column name
            std::cout << '\n';
        } // if not quiet mode
        
        size_t col1Index = table1->second.getColIndex(colName1);
        size_t col2Index = table2->second.getColIndex(colName2);

        for (size_t i = 0; i < table2->second.getTableData().size(); i++) {
            table2Index[table2->second.getTableData()[i][col2Index]].emplace_back(i);
        } // for i: table2 row

        for (size_t i = 0; i < table1->second.getTableData().size(); i++) {
            auto condition = table2Index.find(table1->second.getTableData()[i][col1Index]);

            if (condition != table2Index.end()) {
                for (size_t j = 0; j < condition->second.size(); j++) {
                    if (!isQuietMode) {
                        for (size_t k = 0; k < colNamePrintedVector.size(); k++) {
                            if (tableSelectionVector[k] == '1') {
                                size_t colIndex = table1->second.getColIndex(colNamePrintedVector[k]);

                                std::cout << table1->second.getTableData()[i][colIndex] << " ";
                            } // if col is from table 1

                            else {
                                size_t colIndex = table2->second.getColIndex(colNamePrintedVector[k]);

                                std::cout << table2->second.getTableData()[condition->second[j]][colIndex] << " ";
                            } // else col is from table 2
                        } // for k: cols that have to be printied

                        std::cout << '\n';
                    } // if not quiet mode

                    numPrintedRows += 1;
                } // for: table2 row index for condition
            } // if condition exist
        } // for i: table1 row

        std::cout << "Printed " << numPrintedRows << " rows from joining " 
             << tableName1 << " to " << tableName2 << '\n';
    } // if all error checking return true



}

bool SillyQLClass::checkTableExist(unordered_map<string, TableClass>::iterator &currentTable, 
                                   string inTableName, string inCmd, bool isPrint) {
    if (currentTable == tableDatabase.end()) {
        if (isPrint) {
            std::cerr << "Error during " << changeToFullCmd(inCmd) << ": "
                 << inTableName << " does not name a table in the database" 
                 << '\n';
        } // if isPrint == True
        return false;
    } // if does not exist inTableName          

    return true;
}

bool SillyQLClass::checkColumnExist(unordered_map<string, TableClass>::iterator &currentTable, 
                                    string inColName, string inCmd, 
                                    bool isPrint) {
    size_t numOfCols = currentTable->second.getNumOfCols();
    bool result = false;

    for (size_t i = 0; i < numOfCols; i++) {
        if (currentTable->second.getColNames()[i] == inColName) result = true;
    }

    if ((result == false) && isPrint) {
        std::cerr << "Error during " << changeToFullCmd(inCmd) << ": "
             << inColName << " does not name a column in " 
             << currentTable->first << '\n'; 
    }
    
    return result;
}

string SillyQLClass::changeToFullCmd(string inCmd) {
    string result;
    if (inCmd == "INSERT") result = "INSERT INTO"; 
    else if (inCmd == "DELETE") result = "DELETE FROM";
    else result = inCmd;
    return result; 
}



void SillyQLClass::printTableDatabase() const {
    std::cout << "== Print table database information ==" << '\n';
    for (auto i : tableDatabase) {
        std::cout << "-- table name: " << i.first << '\n';
        i.second.printTableMetadata();
        std::cout << '\n';
    }  
    std::cout << '\n';  
} // printTableDatabase()

TableEntry SillyQLClass::getValidEntry(string inColType) {
    if (inColType == "double") {
        double inputValue;
        
        std::cin >> inputValue;
        return TableEntry(inputValue);
                       
    } // if col type == double
    else if (inColType == "int") {
        int inputValue;
        
        std::cin >> inputValue;
        return TableEntry(inputValue);
    } // else if col type == int
    else if (inColType == "bool") {
        bool inputValue;
        
        std::cin >> inputValue;
        return TableEntry(inputValue);
    } // else if col type == bool
    else {
        string inputValue;
        
        std::cin >> inputValue;
        return TableEntry(inputValue);
    } // else col type == string
}    




