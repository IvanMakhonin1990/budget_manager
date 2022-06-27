#include "budget_manager.h"

#include <iostream>
#include <string_view>
#include <cassert>
#include <sstream>
#include <iomanip>

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
        PayTax pt(manager, s, e);
        pt.Execute();
    }
    else if ("Earn"sv == req_name) {
        double income;
        ss >> income;
        Earn er(manager, s, e, income);
        er.Execute();
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;
    auto tt = Date::ComputeDistance(BudgetManager::START_DATE, BudgetManager::END_DATE);
    Earn e(manager, BudgetManager::START_DATE, BudgetManager::END_DATE, Date::ComputeDistance(BudgetManager::START_DATE, BudgetManager::END_DATE) + 1);
    e.Execute();
    PayTax pt(manager, BudgetManager::START_DATE, BudgetManager::END_DATE);
    pt.Execute();
    ComputeIncome ci(manager, BudgetManager::START_DATE, BudgetManager::END_DATE);
    ci.Execute();

    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }
}
