// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef _ROWCOLUMNCLASS_H_
#define _ROWCOLUMNCLASS_H_

#include <iostream>
#include "constants.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: RowColumnClass will be used as location information of 2d vector.

class RowColumnClass
{
  public:
    RowColumnClass()
    {
      setRowCol(DEFAULT_ROW_COLUMN_VALUE, DEFAULT_ROW_COLUMN_VALUE);  
    }
    // This constructor simply sets both the row and column value of the newly
    // created RowColumnClass object to 0.

    RowColumnClass(uint32_t inRow, uint32_t inCol)
    {
      setRowCol(inRow, inCol);     
    }
    // This constructor simply set the row and column values to the 
    // corresponding values passed into the constructor.

    void setRowCol(uint32_t inRow, uint32_t inCol)
    {
      setRow(inRow);
      setCol(inCol);  
    }
    
    void setRow(uint32_t inRow)
    {
      rowIndex = inRow;  
    }

    void setCol(uint32_t inCol)
    {
      colIndex = inCol;  
    }
    // These functions simply set the appropriate attrubute(s) to the value(s)
    // provided.

    uint32_t getRow() const
    {
      return rowIndex;  
    }

    uint32_t getCol() const
    {
      return colIndex;  
    }
    // These functions just return the appropriate attribute value to the 
    // caller.

    void printRowCol() const;
    // This function prints the object's attribute's in the format 
    // "[<row>,<col>]" where <row> and <col> are replaced with the value of the
    // corresponding attribute values.

  private:
    uint32_t rowIndex;
    uint32_t colIndex;
};

#endif