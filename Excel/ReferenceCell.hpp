#include <iostream>

class ReferenceCell : public Cell   
{  
private:
   std::string reference;  
   Table* table = nullptr;  
   mutable size_t refRow = SIZE_MAX;
   mutable size_t refCol = SIZE_MAX;

protected:
   mutable Cell* cell = nullptr;

public:   
    ReferenceCell(const std::string& ref, Table* t) : reference(ref), table(t) { this->type = CellType::REFERENCE; this->rowValue = ref; }

   std::string evaluate() const override   
   {  
       if (reference.empty()) 
           return "#VALUE!";  

       std::string refToUse = reference;
       size_t pos = reference.find('=');
       if (pos != std::string::npos)
       {
           refToUse = reference.substr(pos + 1);  
       }  

       if (!std::isalpha(refToUse[0])) 
           return "#VALUE!";

       size_t row = refToUse[0] - 'A';  
       size_t col = std::stoi(refToUse.substr(1)) - 1;

       this->refRow = row;
       this->refCol = col;

       if (!table) 
           return "#VALUE!";  

       cell = table->getCell(row, col);  
       if (!cell) 
           return "0";  

       return cell->evaluate();  
   }  

   std::string toString() const override   
   {  
       return cell ? cell->toString() : "";  
   }  

   size_t getRow() const { return refRow; }
   size_t getCol() const { return refCol; }

   Cell* clone() const override   
   {  
       return new ReferenceCell(*this);  
   }  
};
