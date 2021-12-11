#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "DataGetter/IDataGetter.hpp"

class JsonDataGetter : public IDataGetter {
private:
	static constexpr auto* DATA_X_STR = "dataX";
	static constexpr auto* DATA_Y_STR = "dataY";
	static constexpr auto* PROBABILITY_STR = "confidence_probability";

public:
	explicit JsonDataGetter(std::fstream file) : file_(file) {
		this->ReadFile();
	}
	;
	JsonDataGetter(const JsonDataGetter&) = delete;

	JsonDataGetter(JsonDataGetter&&) noexcept = default;

	~JsonDataGetter() override = default;

	JsonDataGetter& operator=(const JsonDataGetter&) = default;

	JsonDataGetter& operator=(JsonDataGetter&&) noexcept = default;

	[[nodiscard]] std::vector<double> GetDataX() const override {
		return this->json_[JsonDataGetter::DATA_X_STR].get<std::vector<double>>();
	}

	[[nodiscard]] std::vector<double> GetDataY() const override {
		return this->json_[JsonDataGetter::DATA_Y_STR].get<std::vector<double>>();
	}

	[[nodiscard]] double GetConfidenceProbability() const override {
		return this->json_[JsonDataGetter::PROBABILITY_STR].get<double>();
	}

private:
	std::reference_wrapper<std::fstream> file_;
	nlohmann::json json_{};

	void ReadFile() {
		if (this->file_.get().is_open()) {
			this->file_ >> this->json_;
		}
	}
};
