// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "SillyQLClass.h"
#include "TableClass.h"
#include "TableEntry.h"
#include "xcode_redirect.hpp"
using namespace std;

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv); 
    cin >> boolalpha;
    cout << boolalpha;

    SillyQLClass testQL;
    
    testQL.get_option(argc, argv);
    //testQL.printProgramArgument(); // TODO: delete

    //cout << '\n'; // TODO: delete
    testQL.readQueryFile();
    //cout << '\n'; // TODO: delete
    //testQL.printTableDatabase(); // TODO: delete
    return 0;
}