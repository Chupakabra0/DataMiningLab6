#pragma once

#include <vector>
#include <functional>
#include <numeric>

class MathSolver {
public:
	explicit MathSolver(const std::vector<double>& x, const std::vector<double>& y)
		: x_(x), y_(y), prodXX_(x.size()), prodXY_(x.size()), n_(x.size()),
		  yCap_(x.size()), e_(x.size()), prodEE_(x.size()),
		  diffPrevEE_(x.size()), prodDiffPrevEE_(x.size()),
		  plusCount_(0), minusCount_(0), seriesCount_(0) {
		this->FillProdXX();
		this->FillProdXY();

		this->BuildModel();

		this->FillYCap();

		this->FillE();
		this->FillProdEE();
		this->FillDiffPrevEE();
		this->FillProdDiffPrevEE();

		this->CountSigns();
	}

	MathSolver(const MathSolver&) = default;

	MathSolver(MathSolver&&) noexcept = default;

	~MathSolver() = default;

	//---------------- GET DATA -------------------------

	[[nodiscard]] std::vector<double> GetDataX() const {
		return this->x_;
	}

	[[nodiscard]] std::vector<double> GetDataY() const {
		return this->y_;
	}

	[[nodiscard]] std::vector<double> GetDataYCap() const {
		return this->yCap_;
	}

	[[nodiscard]] std::vector<double> GetDataE() const {
		return this->e_;
	}

	[[nodiscard]] std::vector<double> GetDataProdEE() const {
		return this->prodEE_;
	}

	[[nodiscard]] std::vector<double> GetDataDiffPrevEE() const {
		return this->diffPrevEE_;
	}

	[[nodiscard]] std::vector<double> GetDataProdDiffPrevEE() const {
		return this->prodDiffPrevEE_;
	}

	[[nodiscard]] std::vector<double> GetDataProdXX() const {
		return this->prodXX_;
	}

	[[nodiscard]] std::vector<double> GetDataProdXY() const {
		return this->prodXY_;
	}

	[[nodiscard]] int GetN() const {
		return this->n_;
	}

	[[nodiscard]] std::pair<double, double> GetModelCoef() const {
		return this->a_;
	}

	[[nodiscard]] double GetSumX() const {
		return std::reduce(this->x_.begin(), this->x_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumY() const {
		return std::reduce(this->y_.begin(), this->y_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumYCap() const {
		return std::reduce(this->yCap_.begin(), this->yCap_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumXX() const {
		return std::reduce(this->prodXX_.begin(), this->prodXX_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumXY() const {
		return std::reduce(this->prodXY_.begin(), this->prodXY_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumE() const {
		return std::reduce(this->e_.begin(), this->e_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumProdEE() const {
		return std::reduce(this->prodEE_.begin(), this->prodEE_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumDiffPrevEE() const {
		return std::reduce(this->diffPrevEE_.begin(), this->diffPrevEE_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetSumProdDiffPrevEE() const {
		return std::reduce(this->prodDiffPrevEE_.begin(), this->prodDiffPrevEE_.end(), 0.0, std::plus());
	}

	[[nodiscard]] double GetDStatistics() const {
		return this->GetSumProdDiffPrevEE() / this->GetSumProdEE();
	}

	[[nodiscard]] int GetPlusCount() const {
		return this->plusCount_;
	}

	[[nodiscard]] int GetMinusCount() const {
		return this->minusCount_;
	}

	[[nodiscard]] int GetSeriesCount() const {
		return this->seriesCount_;
	}

private:
    std::vector<double> x_;
    std::vector<double> y_;

	std::vector<double> prodXX_;
	std::vector<double> prodXY_;

	std::vector<double> yCap_;
	std::vector<double> e_;

	std::vector<double> prodEE_;

	std::vector<double> diffPrevEE_;
	std::vector<double> prodDiffPrevEE_;

	std::function<double(double)> model_;

	std::pair<double, double> a_;

	int n_;

	int plusCount_;
	int minusCount_;
	int seriesCount_;

	void BuildModel() {
		auto a1 = this->n_ * this->GetSumXY() - this->GetSumX() * this->GetSumY();
		a1 /= this->n_ * this->GetSumXX() - pow(this->GetSumX(), 2.0);

		auto a0 = (this->GetSumY() - a1 * this->GetSumX()) / this->n_;

		this->a_ = std::make_pair(a0, a1);

		this->model_ = [this](auto x) {
			return this->a_.first + this->a_.second * x;
		};
	}

	void FillProdXX() {
		for (auto i = 0; i < this->prodXX_.size(); ++i) {
			this->prodXX_[i] = pow(this->x_[i], 2.0);
		}
	}

	void FillProdXY() {
		for (auto i = 0; i < this->prodXX_.size(); ++i) {
			this->prodXY_[i] = this->x_[i] * this->y_[i];
		}
	}

	void FillYCap() {
		for (auto i = 0; i < this->yCap_.size(); ++i) {
			this->yCap_[i] = this->model_(this->x_[i]);
		}
	}

	void FillE() {
		for (auto i = 0; i < this->e_.size(); ++i) {
			this->e_[i] = this->yCap_[i] - this->y_[i];
		}
	}

	void FillProdEE() {
		for (auto i = 0; i < this->prodEE_.size(); ++i) {
			this->prodEE_[i] = std::pow(this->e_[i], 2.0);
		}
	}

	void FillDiffPrevEE() {
		for (auto i = 0; i < this->diffPrevEE_.size(); ++i) {
			this->diffPrevEE_[i] = (i == 0) ? 0.0 : this->e_[i - 1] - this->e_[i];
		}
	}

	void FillProdDiffPrevEE() {
		for (auto i = 0; i < this->prodDiffPrevEE_.size(); ++i) {
			this->prodDiffPrevEE_[i] = std::pow(this->diffPrevEE_[i], 2.0);
		}
	}

	void CountSigns() {
		for (auto i = 0; i < this->e_.size(); ++i) {
			const auto current  = this->e_[i];
			const auto previous = i == 0 ? this->e_[i] : this->e_[i - 1];

			if (current < 0.0) {
				++this->minusCount_;
			}
			else {
				++this->plusCount_;
			}

			if ((current < 0.0 && previous >= 0.0) || (previous < 0.0 && current >= 0.0)) {
				++this->seriesCount_;
			}
		}

		++this->seriesCount_;
	}
};
