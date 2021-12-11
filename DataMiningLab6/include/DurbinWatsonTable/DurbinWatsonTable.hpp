#pragma once

#include <map>

class DurbinWatsonTable {
public:
	~DurbinWatsonTable() = default;

	DurbinWatsonTable(const DurbinWatsonTable&) = delete;

	DurbinWatsonTable(DurbinWatsonTable&&) noexcept = default;

	DurbinWatsonTable& operator=(const DurbinWatsonTable&) = default;

	DurbinWatsonTable& operator=(DurbinWatsonTable&&) noexcept = default;

	static DurbinWatsonTable& GetInstance() {
		static DurbinWatsonTable instance;
		return instance;
	}

	[[nodiscard]] std::pair<double, double> GetElement(int n, int k, double q = 0.05) {
		return this->elementsTable.at({ n, k, q });
	}

private:
	DurbinWatsonTable() = default;

	std::map<std::tuple<int, int, double>, std::pair<double, double>> elementsTable = {
		std::make_pair(std::make_tuple(15, 1, 0.05), std::make_pair(0.95, 1.23)),
		std::make_pair(std::make_tuple(16, 1, 0.05), std::make_pair(0.98, 1.24)),
		std::make_pair(std::make_tuple(17, 1, 0.05), std::make_pair(1.01, 1.25)),
		std::make_pair(std::make_tuple(18, 1, 0.05), std::make_pair(1.03, 1.26)),
		std::make_pair(std::make_tuple(19, 1, 0.05), std::make_pair(1.06, 1.28)),
		std::make_pair(std::make_tuple(20, 1, 0.05), std::make_pair(1.08, 1.28)),
		std::make_pair(std::make_tuple(21, 1, 0.05), std::make_pair(1.10, 1.30)),
		std::make_pair(std::make_tuple(22, 1, 0.05), std::make_pair(1.12, 1.31)),
		std::make_pair(std::make_tuple(23, 1, 0.05), std::make_pair(1.14, 1.32)),
		std::make_pair(std::make_tuple(24, 1, 0.05), std::make_pair(1.16, 1.33)),
		std::make_pair(std::make_tuple(25, 1, 0.05), std::make_pair(1.18, 1.34)),
		std::make_pair(std::make_tuple(26, 1, 0.05), std::make_pair(1.19, 1.35)),
		std::make_pair(std::make_tuple(27, 1, 0.05), std::make_pair(1.21, 1.36)),
		std::make_pair(std::make_tuple(28, 1, 0.05), std::make_pair(1.22, 1.37)),
		std::make_pair(std::make_tuple(29, 1, 0.05), std::make_pair(1.24, 1.38)),
		std::make_pair(std::make_tuple(30, 1, 0.05), std::make_pair(1.25, 1.38)),
		std::make_pair(std::make_tuple(31, 1, 0.05), std::make_pair(1.26, 1.39)),
		std::make_pair(std::make_tuple(32, 1, 0.05), std::make_pair(1.27, 1.40)),
		std::make_pair(std::make_tuple(33, 1, 0.05), std::make_pair(1.28, 1.41)),
		std::make_pair(std::make_tuple(34, 1, 0.05), std::make_pair(1.29, 1.41)),
		std::make_pair(std::make_tuple(35, 1, 0.05), std::make_pair(1.30, 1.42)),
		std::make_pair(std::make_tuple(36, 1, 0.05), std::make_pair(1.31, 1.43)),
		std::make_pair(std::make_tuple(37, 1, 0.05), std::make_pair(1.32, 1.43)),
		std::make_pair(std::make_tuple(38, 1, 0.05), std::make_pair(1.33, 1.44)),
		std::make_pair(std::make_tuple(39, 1, 0.05), std::make_pair(1.34, 1.44)),
		std::make_pair(std::make_tuple(40, 1, 0.05), std::make_pair(1.35, 1.45)),
	};
};