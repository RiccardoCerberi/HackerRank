#pragma once
#include <iostream>
#include <string>

class Test {
public:
  Test(int const&, std::string const&);
private:
  int m_val;
  std::string m_testName;
public:
  int GetVal();
  std::string GetName();
};

std::ostream &PrintValue(std::ostream& out); 

extern int value;