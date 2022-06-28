#include "budget_manager.h"

#include <iostream>
#include <string_view>
#include <cassert>
#include <sstream>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
  std::stringstream ss;
  ss << line;
  string req_name;
  ss >> req_name;
  string date;
  ss >> date;
  Date s = Date::FromString(date);
  ss >> date;
  Date e = Date::FromString(date);
  if ("ComputeIncome" == req_name) {
    ComputeIncome ci(manager, s, e);
    ci.Execute();
  }
  else if ("PayTax" == req_name) {
    double tax_rate;
    ss >> tax_rate;
    PayTax pt(manager, s, e, tax_rate);
    pt.Execute();
  }
  else if ("Earn"sv == req_name) {
    double income;
    ss >> income;
    Earn er(manager, s, e, income);
    er.Execute();
  } else if ("Spend"sv == req_name) {
    double income;
    ss >> income;
    Spend sp(manager, s, e, income);
    sp.Execute();
  }
}

int ReadNumberOnLine(istream& input) {
  std::string line;
  std::getline(input, line);
  return std::stoi(line);
}

int main() {
  BudgetManager manager;

  const int query_count = ReadNumberOnLine(cin);

  for (int i = 0; i < query_count; ++i) {
    std::string line;
    std::getline(cin, line);
    ParseAndProcessQuery(manager, line);
  }
}
