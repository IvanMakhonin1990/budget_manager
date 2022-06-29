#pragma once

class DayData {
public:
  double income = 0;
  double spend = 0;
};

DayData operator+(const DayData lhs, const DayData &rhs);

