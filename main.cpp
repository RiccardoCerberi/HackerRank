#include <cmath>
#include<algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
//#include "raylib.h"

// complete the path to add headers and implementation files as well
//#include "header\t.h"

// using namespace std;

class BaseNumber {
public:
  BaseNumber();
  std::string ConvertToDecimal(int const base);
protected:
  std::string m_numberStringRepresentation;
  int base;
  int boxes;
  int const hexadecimal = 16, binary = 2, oct = 8;
private:
  void TakeBoxes();
  void TakeBase();
  bool IsAllowedBase(int const&);
};

inline bool BaseNumber::IsAllowedBase(int const& base) {
  return base == hexadecimal || base == binary || base == oct;
}

void BaseNumber::TakeBoxes() {
  std::cout << "Insert boxes: ";
  std::cin >> boxes;
  while (boxes <= 0) {
    std::cout << "Invalid: negative or null value inserted";
    std::cin >> boxes;
  }
}

void BaseNumber::TakeBase() {
  std::cout << "insert base: ";
  std::cin >> base;
  while (IsAllowedBase(base)) {
    std::cout << "Invalid base, try again: ";
    std::cin >> base;
  }
}

BaseNumber::BaseNumber() {
  TakeBoxes();
  TakeBase();
}

int CharacterToDecimalNum(char c) {
  return c - '0';
}

char DecimalNumtoCharacter(int val) {
  return val + '0';
}

int Conversion(int base, std::string const& number_representation) {
  int size{number_representation.length()};
  int result{0};

  for (int i = 0; i < size; ++i) {
    result += pow(base, i) * CharacterToDecimalNum(number_representation[i]);
  }
  return result;
}

std::string Conversion(int base, int num_decimalrepresentation) {
  int rest;
  std::string numberBaseConversion;

  while (num_decimalrepresentation > 0) {
    rest = num_decimalrepresentation % base;
    numberBaseConversion.push_back(DecimalNumtoCharacter(rest));
    num_decimalrepresentation = num_decimalrepresentation / base;
  }
  std::reverse(numberBaseConversion.begin(), numberBaseConversion.end());
  return numberBaseConversion;
}

int main() {
  std::string num{Conversion(16, 80)};
  std::for_each(num.begin(), num.end(), [](char c) { std::cout << c; });
  return 0;
}

