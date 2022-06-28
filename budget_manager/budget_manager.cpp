#include "budget_manager.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <execution>
// разработайте менеджер личного бюджета самостоятельно

Day::Day() :m_money_count(0), m_tax_count(0)
{
}

Day::Day(double money_count, double tax) :m_money_count(std::move(money_count)), m_tax_count(std::move(tax)) {}

double Day::GetMoneyCount() const { return m_money_count; }

double& Day::GetMoneyCount()
{
  return m_money_count;
}

void Day::AddEarn(double earn)
{
  m_money_count += earn;
}

void Day::AddTax(double tax)
{
  m_tax_count += tax;
}

double Day::GetTaxCount()
{
  return m_tax_count;
}

BudgetManager::BudgetManager() : m_days(Date::ComputeDistance(START_DATE, END_DATE) + 1) {
}

std::vector<Day>& BudgetManager::GetDays()
{
  return m_days;
}


Request::Request(BudgetManager& budget_manager, const Date& start, const Date& end)
  :m_budget_manager(budget_manager), m_start(start), m_end(end) {}

void Earn::Execute()
{
  auto s = Date::ComputeDistance(BudgetManager::START_DATE, m_start);
  auto e = Date::ComputeDistance(BudgetManager::START_DATE, m_end);
  double earn = m_total_earn / (static_cast<double>(e - s + 1));

  for (auto i = s; i <= e; ++i) {
    m_budget_manager.GetDays()[i].AddEarn(earn);
  }
}

Earn::Earn(BudgetManager& budget_manager, const Date& start, const Date& end, double total_earn)
  :Request(budget_manager, start, end), m_total_earn(total_earn)
{}

ComputeIncome::ComputeIncome(BudgetManager& budget_manager, const Date& start, const Date& end) :Request(budget_manager, start, end)
{
}

void ComputeIncome::Execute()
{
  auto s = m_budget_manager.GetDays().begin() + Date::ComputeDistance(BudgetManager::START_DATE, m_start);
  auto e = m_budget_manager.GetDays().begin() + Date::ComputeDistance(BudgetManager::START_DATE, m_end) + 1;

  // std::cout << std::accumulate(s, e, 0.0, [](double v, const Day& d) {
  //     return v + d.GetMoneyCount();
  //     }) << std::endl;
  auto result = std::accumulate(
      s, e, 0.0, [](double v, const Day &d) { return v + d.GetMoneyCount(); });
  std::cout << result << std::endl;
}

PayTax::PayTax(BudgetManager &budget_manager, const Date &start,
               const Date &end, double tax_rate)
    : Request(budget_manager, start, end), m_tax_rate(tax_rate) {}

void PayTax::Execute()
{
  auto e = Date::ComputeDistance(BudgetManager::START_DATE, m_end);
  auto s = Date::ComputeDistance(BudgetManager::START_DATE, m_start);
  for (auto i = s; i <= e; ++i) {
    m_budget_manager.GetDays()[i].GetMoneyCount() *= ((100.0-m_tax_rate)/100.0);
  }
}

void Spend::Execute() {
  auto s = Date::ComputeDistance(BudgetManager::START_DATE, m_start);
  auto e = Date::ComputeDistance(BudgetManager::START_DATE, m_end);
  double earn = m_total_earn / (static_cast<double>(e - s + 1));

  for (auto i = s; i <= e; ++i) {
    m_budget_manager.GetDays()[i].AddEarn(-earn);
  }
}

Spend::Spend(BudgetManager &budget_manager, const Date &start, const Date &end,
             double total_earn)
    : Request(budget_manager, start, end), m_total_earn(total_earn) {}
