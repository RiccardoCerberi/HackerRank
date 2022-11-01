#include"..\header\t.h"

int Test::GetVal() { return m_val; }

std::string Test::GetName() { return m_testName; }

std::ostream &PrintValue(std::ostream& out) { 
  return out << "The value is = " << value; // the value to be printed out depends on the order of the initialization
}