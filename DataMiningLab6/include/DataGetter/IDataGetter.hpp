#pragma once

#include <vector>

struct IDataGetter {
public:
    virtual ~IDataGetter() = default;

    [[nodiscard]] virtual std::vector<double> GetDataX() const = 0;
    [[nodiscard]] virtual std::vector<double> GetDataY() const = 0;

    [[nodiscard]] virtual double GetConfidenceProbability() const = 0;
};