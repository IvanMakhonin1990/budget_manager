#pragma once

#include "entities.h"
#include "summing_segment_tree.h"
#include "budget_manager.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    double delta = {};
};

struct BulkMoneySpender {
  double delta = {};
};

struct BulkTaxApplier {
    double ComputeFactor() const {
        static const double factor = 0.87;
        //return std::pow(m_tax_rate > 0.0 ? m_tax_rate : factor, count);
        return std::pow(m_tax_rate, count);
    }

    int count = 0;
    double m_tax_rate = 0.0;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    BulkLinearUpdater(const BulkMoneySpender &spend) : spend_(spend) {}

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.count *= other.tax_.count;
        add_.delta = add_.delta * other.tax_.ComputeFactor() + other.add_.delta;
        spend_.delta += other.spend_.delta;
    }
    DayData Collapse(DayData origin, IndexSegment segment) const {
      return {origin.income * tax_.ComputeFactor() + add_.delta *
                       static_cast<double>(segment.length()),
              spend_.delta * static_cast<double>(segment.length())};
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spend_;
};
