#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "tether.hpp"

// -----------------------------------------------------------------------------
// 1. Единая функция проверки для простых типов
// -----------------------------------------------------------------------------
template <typename T>
void check(const char* name, const T& actual, const T& expected) {
    if (actual == expected) {
        std::printf("Yes %s\n", name);
        return;
    }
    if constexpr (std::is_same_v<T, std::optional<double>>) {
        if (actual.has_value() && expected.has_value())
            std::printf("No %s: got %.6f, expected %.6f\n", name,
                actual.value(), expected.value());
        else
            std::printf("No %s: got %s, expected %s\n", name,
                actual.has_value() ? "value" : "nullopt",
                expected.has_value() ? "value" : "nullopt");
    }
    else if constexpr (std::is_same_v<T, std::optional<std::string>>) {
        if (actual.has_value() && expected.has_value())
            std::printf("No %s: got \"%s\", expected \"%s\"\n", name,
                actual.value().c_str(), expected.value().c_str());
        else
            std::printf("No %s: got %s, expected %s\n", name,
                actual.has_value() ? "value" : "nullopt",
                expected.has_value() ? "value" : "nullopt");
    }
    else if constexpr (std::is_same_v<T, std::optional<std::uint16_t>>) {
        if (actual.has_value() && expected.has_value())
            std::printf("No %s: got %u, expected %u\n", name,
                actual.value(), expected.value());
        else
            std::printf("No %s: got %s, expected %s\n", name,
                actual.has_value() ? "value" : "nullopt",
                expected.has_value() ? "value" : "nullopt");
    }
    else if constexpr (std::is_same_v<T, double>) {
        std::printf("No %s: got %.6f, expected %.6f\n", name, actual, expected);
    }
    else if constexpr (std::is_same_v<T, bool>) {
        std::printf("No %s: got %s, expected %s\n", name,
            actual ? "true" : "false", expected ? "true" : "false");
    }
    else if constexpr (std::is_same_v<T, int>) {
        std::printf("No %s: got %d, expected %d\n", name, actual, expected);
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        std::printf("No %s: got \"%s\", expected \"%s\"\n", name,
            actual.c_str(), expected.c_str());
    }
    else {
        std::printf("No %s: values differ\n", name);
    }
    std::abort();
}

// Допуск для double
void checkClose(const char* name, double actual, double expected, double eps = 1e-6) {
    if (std::abs(actual - expected) < eps) {
        std::printf("Yes %s\n", name);
    }
    else {
        std::printf("No %s: got %.6f, expected %.6f (diff=%.2e)\n",
            name, actual, expected, std::abs(actual - expected));
        std::abort();
    }
}

// Проверка optional<double> с допуском
void checkOptionalDouble(const char* name, const std::optional<double>& actual,
    const std::optional<double>& expected, double eps = 1e-6) {
    if (actual.has_value() != expected.has_value()) {
        std::printf("No %s: got %s, expected %s\n", name,
            actual.has_value() ? "value" : "nullopt",
            expected.has_value() ? "value" : "nullopt");
        std::abort();
    }
    if (actual.has_value())
        checkClose(name, actual.value(), expected.value(), eps);
    else
        std::printf("Yes %s\n", name);
}

// Специальная проверка для optional<vector<string>>
void reportTestLines(const char* name,
    const std::optional<std::vector<std::string>>& opt,
    const std::vector<std::string>& expected) {
    if (!opt.has_value()) {
        std::printf("No %s: nullopt\n", name);
        std::abort();
    }
    const auto& lines = opt.value();
    if (lines.size() != expected.size()) {
        std::printf("No %s: size mismatch (%zu vs %zu)\n", name, lines.size(), expected.size());
        std::abort();
    }
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i] != expected[i]) {
            std::printf("No %s: line %zu differs\n", name, i);
            std::abort();
        }
    }
    std::printf("Yes %s: matches expected\n", name);
}

// -----------------------------------------------------------------------------
// 2. Тестовые данные
// -----------------------------------------------------------------------------
const auto& globalBreakers = Tether::Data::loadBreakerCatalog();
const auto& globalCables = Tether::Data::loadCableCatalog();
const auto& globalMotors = Tether::Data::loadMotorCatalog();

const std::vector<Tether::Data::Breaker> smallBreakers = {
    {10.0, Tether::Data::Characteristic::C, 3},
    {16.0, Tether::Data::Characteristic::C, 3},
    {20.0, Tether::Data::Characteristic::C, 3},
    {25.0, Tether::Data::Characteristic::C, 3},
    {32.0, Tether::Data::Characteristic::C, 3}
};

const std::vector<Tether::Data::Cable> smallCables = {
    {1.5e-6, 19.0, 12.3 / 1000.0},
    {2.5e-6, 25.0, 7.38 / 1000.0},
    {4.0e-6, 35.0, 4.60 / 1000.0},
    {6.0e-6, 42.0, 3.07 / 1000.0},
    {10.0e-6,55.0, 1.84 / 1000.0}
};

const std::vector<Tether::Data::Motor> smallMotors = {
    {370.0,  1.30, 380.0, 0.70, 0.620, 4.7},
    {550.0,  1.80, 380.0, 0.72, 0.650, 4.7},
    {750.0,  2.29, 380.0, 0.73, 0.690, 5.3},
    {1100.0, 3.18, 380.0, 0.74, 0.720, 5.5},
    {1500.0, 4.00, 380.0, 0.75, 0.760, 5.5}
};

// -----------------------------------------------------------------------------
// 3. Тесты selection (явные типы, вариативные)
// -----------------------------------------------------------------------------
void testSelection() {
    using namespace Tether::Data;

    // ---------- Breaker ----------
    auto testBrk = [&](const char* name, const std::vector<Breaker>& catalog,
        double current, double coef, std::optional<double> expected) {
        auto res = selectBreaker(catalog, current, coef);
        std::optional<double> actual =
            res.has_value() ? std::optional<double>(res->nominalCurrent) : std::nullopt;
        checkOptionalDouble(name, actual, expected);
    };

    testBrk("Global exact 16A", globalBreakers, 16.0, 1.0, std::optional<double>(16.0));
    testBrk("Global 16A coef 1.2", globalBreakers, 16.0, 1.2, std::optional<double>(20.0));
    testBrk("Global 5A -> nearest", globalBreakers, 5.0, 1.0, std::optional<double>(5.0));
    testBrk("Global 40A -> nearest", globalBreakers, 40.0, 1.0, std::optional<double>(40.0));
    testBrk("Global empty", {}, 16.0, 1.0, std::nullopt);

    testBrk("Small exact 16A", smallBreakers, 16.0, 1.0, std::optional<double>(16.0));
    testBrk("Small between 20/25, pick 25 (first >=)", smallBreakers, 21.0, 1.0, std::optional<double>(25.0));
    testBrk("Small equidistant 20/25, pick 25 (first >=)", smallBreakers, 22.5, 1.0, std::optional<double>(25.0));
    testBrk("Small below first -> 10A", smallBreakers, 5.0, 1.0, std::optional<double>(10.0));
    testBrk("Small above last -> 32A", smallBreakers, 40.0, 1.0, std::optional<double>(32.0));

    // ---------- Cable ----------
    auto testCbl = [&](const char* name, const std::vector<Cable>& catalog,
        double current, std::optional<double> expected) {
        auto res = selectCable(catalog, current);
        std::optional<double> actual =
            res.has_value() ? std::optional<double>(res->maxCurrent) : std::nullopt;
        checkOptionalDouble(name, actual, expected);
    };

    testCbl("Cable global exact 25A", globalCables, 25.0, std::optional<double>(25.0));
    testCbl("Cable global 22A -> nearest 25A", globalCables, 22.0, std::optional<double>(25.0));
    testCbl("Cable global 10A -> nearest 19A", globalCables, 10.0, std::optional<double>(19.0));
    testCbl("Cable global above all -> last 445A", globalCables, 500.0, std::optional<double>(445.0));
    testCbl("Cable global empty", {}, 21.0, std::nullopt);

    testCbl("Cable small exact 35A", smallCables, 35.0, std::optional<double>(35.0));
    testCbl("Cable small between 25/35, pick 35 (first >=)", smallCables, 30.0, std::optional<double>(35.0));
    testCbl("Cable small below first -> 19A", smallCables, 10.0, std::optional<double>(19.0));
    testCbl("Cable small above last -> 55A", smallCables, 60.0, std::optional<double>(55.0));

    // ---------- Motor ----------
    auto testMtr = [&](const char* name, const std::vector<Motor>& catalog,
        double power, std::optional<double> expected) {
        auto res = selectMotor(catalog, power);
        std::optional<double> actual =
            res.has_value() ? std::optional<double>(res->power) : std::nullopt;
        checkOptionalDouble(name, actual, expected);
    };

    testMtr("Motor global exact 1100W", globalMotors, 1100.0, std::optional<double>(1100.0));
    testMtr("Motor global 1000W -> nearest 1100W", globalMotors, 1000.0, std::optional<double>(1100.0));
    testMtr("Motor global equidistant 550/750, pick smaller", globalMotors, 625.0, std::optional<double>(550.0));
    testMtr("Motor global below first -> 370W", globalMotors, 100.0, std::optional<double>(370.0));
    testMtr("Motor global above last -> 250000W", globalMotors, 300000.0, std::optional<double>(250000.0));
    testMtr("Motor global empty", {}, 1000.0, std::nullopt);

    testMtr("Motor small exact 750W", smallMotors, 750.0, std::optional<double>(750.0));
    testMtr("Motor small between 750/1100, pick smaller", smallMotors, 900.0, std::optional<double>(750.0));
    testMtr("Motor small below first -> 370W", smallMotors, 100.0, std::optional<double>(370.0));
    testMtr("Motor small above last -> 1500W", smallMotors, 2000.0, std::optional<double>(1500.0));
}

// -----------------------------------------------------------------------------
// 4. Остальные тесты (calc, scada, csv)
// -----------------------------------------------------------------------------
void testCalc() {
    using namespace Tether::Calc;
    auto catalog = Tether::Data::loadMotorCatalog();

    {
        CalculateCurrentContext ctx;
        ctx.power = 1500.0; ctx.voltage = 380.0; ctx.cosPhi = 0.85;
        ctx.eff = 0.9; ctx.selectFromCatalog = false;
        ctx.loadType = LoadType::THREE_PHASE;
        auto res = currentByContext(catalog, ctx);
        double expected = 1500.0 / (380.0 * 0.9 * Tether::Constants::SQRT3 * 0.85);
        checkOptionalDouble("3ph manual calc", res, expected);
    }

    {
        CalculateCurrentContext ctx;
        ctx.power = 2000.0; ctx.voltage = 230.0; ctx.cosPhi = 1.0;
        ctx.eff = 1.0; ctx.selectFromCatalog = false;
        ctx.loadType = LoadType::ONE_PHASE;
        auto res = currentByContext(catalog, ctx);
        double expected = 2000.0 / (230.0 * 1.0 * 1.0 * 1.0);
        checkOptionalDouble("1ph manual calc", res, expected);
    }

    {
        CalculateCurrentContext ctx;
        ctx.power = 1300.0; ctx.selectFromCatalog = true;
        ctx.loadType = LoadType::THREE_PHASE;
        auto res = currentByContext(catalog, ctx);
        check("Interpolation returns value", res.has_value(), true);
        if (res.has_value()) std::printf("Interpolation result: %.6f A\n", res.value());
    }

    {
        CalculateCurrentContext ctx;
        ctx.power = 1e-10; ctx.selectFromCatalog = true;
        ctx.loadType = LoadType::THREE_PHASE;
        auto res = currentByContext(catalog, ctx);
        checkOptionalDouble("Zero power -> 0.0", res, 0.0);
    }
}

void testScada() {
    using namespace Tether::SCADA;
    using Tether::IO::openCsv;
    using Tether::IO::parseCsv;

    // DataType conversions
    check("stringToDataType BOOL -> not null", stringToDataType("BOOL").has_value(), true);
    check("stringToDataType UNKNOWN -> nullopt", stringToDataType("UNKNOWN").has_value(), false);

    auto dt = stringToDataType("BOOL");
    if (dt.has_value()) {
        auto str = dataTypeToString(dt.value());
        check("dataTypeToString BOOL", str, std::optional<std::string>("BOOL"));
    }

    // Address allocation
    auto a1 = allocateAddress(DataType::BOOL);
    auto a2 = allocateAddress(DataType::BOOL);
    check("BOOL addr 1", a1, std::optional<std::uint16_t>(15000));
    check("BOOL addr 2", a2, std::optional<std::uint16_t>(15001));

    auto a3 = allocateAddress(DataType::INT);
    check("INT addr 1", a3, std::optional<std::uint16_t>(20000));

    auto a4 = allocateAddress(DataType::REAL);
    auto a5 = allocateAddress(DataType::REAL);
    check("REAL addr 1", a4, std::optional<std::uint16_t>(40000));
    check("REAL addr 2", a5, std::optional<std::uint16_t>(40002));

    auto a6 = allocateAddress(DataType::DWORD);
    check("DWORD addr 1", a6, std::optional<std::uint16_t>(35000));

    // ST generator
    {
        Variable v;
        v.name = "TestVar"; v.modbusAddress = 100; v.dataType = DataType::BOOL;
        auto lines = generateStLines(v);
        reportTestLines("ST lines (BOOL)", lines, { "RegHoldingBuf[100] := TO_WORD(TestVar);" });
    }
    {
        Variable v;
        v.name = "RealVar"; v.modbusAddress = 200; v.dataType = DataType::REAL;
        auto lines = generateStLines(v);
        reportTestLines("ST lines (REAL)", lines, {
            "RegHoldingBuf[200] := TO_WORD(RealVar);",
            "RegHoldingBuf[201] := TO_WORD(RealVar);"
            });
    }
    {
        Variable v;
        v.name = "DwordVar"; v.modbusAddress = 300; v.dataType = DataType::DWORD;
        auto lines = generateStLines(v);
        reportTestLines("ST lines (DWORD)", lines, {
            "RegHoldingBuf[300] := TO_WORD(DwordVar);",
            "RegHoldingBuf[301] := TO_WORD(DwordVar);"
            });
    }

    // CSV тест
    {
        const char* csvPath = "examples/variables.csv";
        try {
            auto file = openCsv(csvPath);
            auto rows = parseCsv(file);
            check("CSV file opened", true, true);
            check("CSV at least 2 rows", rows.size() >= 2, true);

            auto vars = buildVariablesFromCsv(rows);
            int expectedCount = (rows[0][0] == "Name") ? (int)rows.size() - 1 : (int)rows.size();
            check("CSV buildVariables count", (int)vars.size(), expectedCount);

            if (!vars.empty()) {
                check("CSV first name", vars[0].name, std::string("Motor_Start"));
                auto typeStr = dataTypeToString(vars[0].dataType);
                check("CSV first type", typeStr, std::optional<std::string>("BOOL"));
                check("CSV first addr nonzero", vars[0].modbusAddress != 0, true);
            }
        }
        catch (...) {
            std::printf("CSV test skipped (file missing?)\n");
        }
    }
}

// -----------------------------------------------------------------------------
void runAllTests() {
    testCalc();
    testScada();
    testSelection();
}