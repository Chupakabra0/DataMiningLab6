#pragma once

#include "DataGetter/IDataGetter.hpp"
#include <numeric>

class TestDataGetter : public IDataGetter {
public:
    TestDataGetter() = default;

    TestDataGetter(const TestDataGetter&) = default;

    TestDataGetter(TestDataGetter&&) noexcept = default;

    ~TestDataGetter() override = default;

    [[nodiscard]] std::vector<double> GetDataX() const override {
        std::vector<double> x(24);
        std::iota(x.begin(), x.end(), 1.0);
        return x;
    }

    [[nodiscard]] virtual std::vector<double> GetDataY() const override {
        return { 40.69, 68.76, 91.76,
                 45.03, 67.69, 115.90,
                 152.70, 73.84, 110.4,
                 184.20, 240.10, 114.20,
                 168.80, 279.60, 359.60,
                 171.00, 248.70, 405.30,
                 514.90, 240.60, 347.90,
                 562.00, 708.20, 327.30
        };
    }

    [[nodiscard]] virtual double GetConfidenceProbability() const {
        return 0.95;
    }

private:

};