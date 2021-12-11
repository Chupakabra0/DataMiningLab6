#pragma once
#include "MainHeader.hpp"

class Main {
private:
	static const std::pair <const char*, const char*> FILES_ARG_STR;

public:
	Main() = delete;

	Main(const Main&) = delete;

	Main(Main&&) = default;

	explicit Main(int argc, char* argv[]) {
		this->InitArgv(argc, argv);
	}

	~Main() = default;

	int LaunchMain() {
		Main::SetLocale(1251u);

		try {
			this->ParseArgs();
		}
		catch (const std::exception&) {
			fmt::print(fmt::fg(fmt::color::orange),
				"ВНИМАНИЕ! Добавьте файлы с входными данными...\nПример: {} {} fileName.json\n",
				this->argv_.front(), Main::FILES_ARG_STR.second);

			return EXIT_FAILURE;
		}

		for (const auto& path : this->filePaths_) {
			try {
				fmt::print("Файл данных: ");
				fmt::print(fmt::fg(fmt::color::aqua), "{}\n", path);

				std::fstream file(path, std::fstream::in);

				if (!file.is_open()) {
					throw std::runtime_error("Wrong path or file doesn't exist!");
				}

				std::unique_ptr<IDataGetter> dataGetter =
					std::make_unique<JsonDataGetter>(std::move(file));

				const auto x = dataGetter->GetDataX();
				const auto y = dataGetter->GetDataY();
				const auto p = dataGetter->GetConfidenceProbability();

				const auto mathSolver = std::make_unique<MathSolver>(x, y);

				const auto signTable = this->GetSignTable(mathSolver.get());

				std::cout << signTable << std::endl;

				const auto [a0, a1] = mathSolver->GetModelCoef();

				const auto n1 = mathSolver->GetPlusCount();
				const auto n2 = mathSolver->GetMinusCount();
				const auto nu = mathSolver->GetSeriesCount();

				const auto [low, high] = SignTables::GetInstance().GetBounds(n1, n2);

				fmt::print("a_0 = {}, a_1 = {}\n", a0, a1);

				Main::PrintDelimiter(std::cout);
				fmt::print("n_1 = {}, n_2 = {}, n_u = {}\n", n1, n2, nu);
				Main::PrintDelimiter(std::cout);

				fmt::print("Интервал значений n: ({}, {})\n", low, high);
				Main::PrintDelimiter(std::cout);

				const auto dTable = this->GetDTable(mathSolver.get());

				std::cout << dTable << std::endl;

				const auto [di, du] = DurbinWatsonTable::GetInstance().GetElement(x.size(), 1);
				const auto dStat    = mathSolver->GetDStatistics();

				fmt::print("Статистика Дарбина-Уотсона: d_i = {}, d_u = {}\n", di, du);
				fmt::print("d-статистика: {:.4f}\n", dStat);
				fmt::print("Интервал d: ({}, {})\n", du, 4.0 - du);
				Main::PrintDelimiter(std::cout);
			}
			catch (const std::exception& exception) {
				fmt::print(fmt::fg(fmt::color::red), "ОШИБКА!!! {}\n", exception.what());
			}
		}

		return EXIT_SUCCESS;
	}

private:
	static void SetLocale(unsigned code) {
		#if defined(_WIN32)
			SetConsoleCP(code);
			SetConsoleOutputCP(code);
		#endif
	}

	static void PrintDelimiter(std::ostream& out, const char symbol = '-', const size_t count = 40u) {
		out << std::string(count, symbol) << '\n';
	}

	std::vector<std::string> argv_;
	std::vector<std::string> filePaths_;

	void InitArgv(int argc, char* argv[]) {
		std::copy_n(argv, argc, std::back_inserter(this->argv_));
	}

	void ParseArgs() {
		try {
			const auto argumentParser = std::make_unique<argparse::ArgumentParser>();
			argumentParser->add_argument(Main::FILES_ARG_STR.first, Main::FILES_ARG_STR.second)
				.required()
				.nargs(static_cast<int>(this->argv_.size()) - 2)
				.help("specify the input JSON-files.");

			argumentParser->parse_args(this->argv_);

			this->filePaths_ = argumentParser->get<std::vector<std::string>>(Main::FILES_ARG_STR.first);
		}
		catch (const std::exception&) {
			throw;
		}
	}

	tabulate::Table GetSignTable(MathSolver* mathSolver) {
		tabulate::Table table;

		table.add_row({ "N", "x", "y", "y^", "e_i", "sign" });

		const auto n = mathSolver->GetN();
		const auto x = mathSolver->GetDataX();
		const auto y = mathSolver->GetDataY();
		const auto e = mathSolver->GetDataE();

		const auto yCap = mathSolver->GetDataYCap();
		
		for (auto i = 0; i < n; ++i) {
			table.add_row({
				std::format("{}", i + 1),
				std::format("{:.4f}", x[i]),
				std::format("{:.4f}", y[i]),
				std::format("{:.4f}", yCap[i]),
				std::format("{:.4f}", e[i]),
				(e[i] < 0) ? "-" : "+"
			});
		}

		return table;
	}

	tabulate::Table GetDTable(MathSolver* mathSolver) {
		tabulate::Table table;

		table.add_row({
			"N", "x", "y", "y^", "e_i", "e_i^2", "e_{i - 1} - e_i", "(e_{i -1} - e_i)^2"}
		);

		const auto n = mathSolver->GetN();
		const auto x = mathSolver->GetDataX();
		const auto y = mathSolver->GetDataY();
		const auto e = mathSolver->GetDataE();

		const auto yCap     = mathSolver->GetDataYCap();
		const auto prodE    = mathSolver->GetDataProdEE();
		const auto diff     = mathSolver->GetDataDiffPrevEE();
		const auto prodDiff = mathSolver->GetDataProdDiffPrevEE();

		for (auto i = 0; i < n; ++i) {
			table.add_row({
				std::format("{}", i + 1),
				std::format("{:.4f}", x[i]),
				std::format("{:.4f}", y[i]),
				std::format("{:.4f}", yCap[i]),
				std::format("{:.4f}", e[i]),
				std::format("{:.4f}", prodE[i]),
				std::format("{:.4f}", diff[i]),
				std::format("{:.4f}", prodDiff[i]),
			});
		}

		table.add_row({
			std::format("sum:"),
			std::format("{:.4f}", mathSolver->GetSumX()),
			std::format("{:.4f}", mathSolver->GetSumY()),
			std::format("{:.4f}", mathSolver->GetSumYCap()),
			std::format("{:.4f}", mathSolver->GetSumE()),
			std::format("{:.4f}", mathSolver->GetSumProdEE()),
			std::format("{:.4f}", mathSolver->GetSumDiffPrevEE()),
			std::format("{:.4f}", mathSolver->GetSumProdDiffPrevEE())
		});

		return table;
	}
};

const std::pair <const char*, const char*> Main::FILES_ARG_STR = { "-f", "--files" };
