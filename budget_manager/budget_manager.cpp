#include "budget_manager.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <execution>
// разработайте менеджер личного бюджета самостоятельно

Day::Day() :m_money_count(0)
{
}

Day::Day(double money_count) :m_money_count(std::move(money_count)) {}

double Day::GetMoneyCount() const { return m_money_count; }

double& Day::GetMoneyCount()
{
  return m_money_count;
}

void Day::AddEarn(double earn)
{
  m_money_count += earn;
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

  //std::cout << std::accumulate(s, e, 0.0, [](double v, const Day& d) {
  //    return v + d.GetMoneyCount(); 
  //    }) << std::endl;
  auto result = std::accumulate(s, e, 0.0, [](double v, const Day& d) {
    return v + d.GetMoneyCount();
    });
  std::cout << result << std::endl;
  /*std::cout << std::reduce(s, e, 0.0, [](double v, const Day& d) {
      return v + d.GetMoneyCount();
      }) << std::endl;

  {
      auto t1 = std::chrono::high_resolution_clock::now();
      auto result = std::accumulate(s, e, 0.0, [](double v, const Day& d) {
      return v + d.GetMoneyCount();
      });
      auto t2 = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> ms = t2 - t1;
      std::cout << std::fixed << "std::accumulate result " << result
          << " took " << ms.count() << " ms\n";
  }

  {
      auto t1 = std::chrono::high_resolution_clock::now();
      std::vector<double> v1(std::distance(s, e));
      std::transform(std::execution::par, s, e, v1.begin(), [](const Day& d) {return d.GetMoneyCount(); });

      double result = std::reduce(std::execution::par, v1.begin(), v1.end());
      auto t2 = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> ms = t2 - t1;
      std::cout << "std::reduce result "
          << result << " took " << ms.count() << " ms\n";
  }*/
}

PayTax::PayTax(BudgetManager& budget_manager, const Date& start, const Date& end) : Request(budget_manager, start, end)
{
}

void PayTax::Execute()
{
  for (auto i = Date::ComputeDistance(BudgetManager::START_DATE, m_start); i <= Date::ComputeDistance(BudgetManager::START_DATE, m_end); ++i) {
    m_budget_manager.GetDays()[i].GetMoneyCount() *= 0.87;
  }
}
