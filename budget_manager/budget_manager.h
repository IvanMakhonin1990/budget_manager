#pragma once
#include <vector>

#include "date.h"

class BudgetManager;

class Request {
public:
  Request(BudgetManager& budget_manager, const Date& start, const Date& end);
  virtual void Execute() = 0;
protected:
  BudgetManager& m_budget_manager;
  Date m_start;
  Date m_end;
};


class Earn : public Request {
public:
  void Execute() override;
  Earn(BudgetManager& budget_manager, const Date& start, const Date& end, double total_earn);
private:
  double m_total_earn = 0;
};

class ComputeIncome : public Request {
public:
  ComputeIncome(BudgetManager& budget_manager, const Date& start, const Date& end);
  void Execute() override;
};

class PayTax : public Request {
public:
  PayTax(BudgetManager& budget_manager, const Date& start, const Date& end);
  void Execute() override;
};

class Day {
public:
  Day();
  explicit Day(double money_count, double tax);
  double GetMoneyCount() const;
  double& GetMoneyCount();
  void AddEarn(double earn);
  void AddTax(double tax);
  double GetTaxCount();

private:
  double m_money_count = 0;
  double m_tax_count = 0;
};

class BudgetManager {
public:
  inline static const Date START_DATE{ 2000, 1, 1 };
  inline static const Date END_DATE{ 2100, 1, 1 };

public:
  BudgetManager();
  std::vector<Day>& GetDays();

private:
  std::vector<Day> m_days;
  // разработайте класс BudgetManager
};
