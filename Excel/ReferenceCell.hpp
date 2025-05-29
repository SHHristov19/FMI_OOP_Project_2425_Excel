#pragma once  
#include "Cell.h"  
#include "Table.h"

class ReferenceCell : public Cell   
{  
   std::string reference;  
   mutable Cell* cell = nullptr;  
   Table* table = nullptr;  
public:  
   ReferenceCell(const std::string& ref, Table* t) : reference(ref), table(t) {}  

   std::string evaluate() const override   
   {  
       if (reference.empty() || !isalpha(reference[0])) return "#VALUE!";  
       size_t row = reference[0] - 'A';  
       int col = std::stoi(reference.substr(1)) - 1;  

       if (!table) return "#VALUE!";  

       cell = table->getCell(row, col);  
       if (!cell) return "0";  
       return cell->evaluate();  
   }  

   std::string toString() const override   
   {  
       return cell ? cell->toString() : "";  
   }  

   Cell* clone() const override   
   {  
       return new ReferenceCell(*this);  
   }  
};
