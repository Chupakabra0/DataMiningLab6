#pragma once

#include <iostream>
#include <vector>

#include "argparse/argparse.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include "nlohmann/json.hpp"
#include "tabulate/table.hpp"

#if defined(_WIN32)
    #include <Windows.h>
#endif

#include "DataGetter/TestDataGetter.hpp"
#include "DataGetter/JsonDataGetter.hpp"
#include "MathSolver/MathSolver.hpp"
#include "SignTables/SignTables.hpp"
#include "DurbinWatsonTable/DurbinWatsonTable.hpp"
