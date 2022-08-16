#include<iostream>
#include<string>

class Test {
  Test(std::string const& testName) : m_testName(testName) {}
  std::string m_testName;
};

int main() {
  std::cout << "Hello World" << std::endl;
  return 0;
}