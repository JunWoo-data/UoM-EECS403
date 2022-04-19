// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include "RowColumnClass.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: November 4, 2021
//Purpose: RowColumnClass will be used as location information of 2d array.

void RowColumnClass::printRowCol() const
{
  cout << "[" << getRow() << "," << getCol() << "]";
}
// This function prints the object's attribute's in the format 
// "[<row>,<col>]" where <row> and <col> are replaced with the value of the
// corresponding attribute values.