// test_current.cpp
#include <cassert>
#include <cstdio>
#include <cmath>
#include <optional>
#include <string>
#include <vector>
#include "tether.hpp"

constexpr double EPS = 1e-6;

void reportTest(const char* name, std::optional<double> actual, double expected) {
    if (!actual.has_value()) {
        std::printf("%s: got nullopt, expected %.6f\n", name, expected);
        std::abort();
    }
    double diff = std::abs(actual.value() - expected);
    bool pass = (diff < EPS);
    std::printf("%s %s: got %.6f, expected %.6f (diff=%.2e)\n",
        pass ? "Yes" : "No",
        name,
        actual.value(),
        expected,
        diff
    );
    if (!pass) std::abort();
}

void reportTestBool(const char* name, bool actual, bool expected) {
    bool pass = (actual == expected);
    std::printf("%s %s: got %s, expected %s\n",
        pass ? "Yes" : "No",
        name,
        actual ? "true" : "false",
        expected ? "true" : "false"
    );
    if (!pass) std::abort();
}

void reportTestInt(const char* name, int actual, int expected) {
    bool pass = (actual == expected);
    std::printf("%s %s: got %d, expected %d\n",
        pass ? "Yes" : "No",
        name,
        actual,
        expected
    );
    if (!pass) std::abort();
}

void reportTestString(const char* name, const std::string& actual, const std::string& expected) {
    bool pass = (actual == expected);
    std::printf("%s %s: got \"%s\", expected \"%s\"\n",
        pass ? "Yes" : "No",
        name,
        actual.c_str(),
        expected.c_str()
    );
    if (!pass) std::abort();
}

void reportTestOptionalInt(const char* name, std::optional<std::uint16_t> opt, int expected) {
    bool has = opt.has_value();
    bool pass = has && (opt.value() == expected);
    std::printf("%s %s: got %s%d, expected %d\n",
        pass ? "Yes" : "No",
        name,
        has ? "" : "(nullopt) ",
        has ? static_cast<int>(opt.value()) : 0,
        expected
    );
    if (!pass) std::abort();
}

void reportTestOptionalString(const char* name, std::optional<std::string> opt, const char* expected) {
    bool has = opt.has_value();
    bool pass = has && (opt.value() == expected);
    std::printf("%s %s: got %s%s, expected %s\n",
        pass ? "Yes" : "No",
        name,
        has ? "" : "(nullopt) ",
        has ? opt.value().c_str() : "",
        expected
    );
    if (!pass) std::abort();
}

void reportTestLines(const char* name, std::optional<std::vector<std::string>> opt, const std::vector<std::string>& expected) {
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

int main() {
    // ---------- calc tests ----------
    {
        Tether::Calc::CalculateCurrentContext ctx;
        ctx.power = 1500.0;
        ctx.voltage = 380.0;
        ctx.cosPhi = 0.85;
        ctx.eff = 0.9;
        ctx.selectFromCatalog = false;
        ctx.loadType = Tether::Calc::LoadType::THREE_PHASE;

        auto catalog = Tether::Data::loadMotorCatalog();
        auto result = Tether::Calc::currentByContext(catalog, ctx);
        double expected = 1500.0 / (380.0 * 0.9 * Tether::Constants::SQRT3 * 0.85);
        reportTest("Three-phase manual calc", result, expected);
    }

    {
        Tether::Calc::CalculateCurrentContext ctx;
        ctx.power = 2000.0;
        ctx.voltage = 230.0;
        ctx.cosPhi = 1.0;
        ctx.eff = 1.0;
        ctx.selectFromCatalog = false;
        ctx.loadType = Tether::Calc::LoadType::ONE_PHASE;

        auto catalog = Tether::Data::loadMotorCatalog();
        auto result = Tether::Calc::currentByContext(catalog, ctx);
        double expected = 2000.0 / (230.0 * 1.0 * 1.0 * 1.0);
        reportTest("Single-phase manual calc", result, expected);
    }

    {
        auto catalog = Tether::Data::loadMotorCatalog();
        Tether::Calc::CalculateCurrentContext ctx;
        ctx.power = 1300.0;
        ctx.selectFromCatalog = true;
        ctx.loadType = Tether::Calc::LoadType::THREE_PHASE;

        auto result = Tether::Calc::currentByContext(catalog, ctx);
        reportTestBool("Interpolation returns value", result.has_value(), true);
        if (result.has_value()) {
            std::printf("Interpolation result: %.6f A (for 1300 W)\n", result.value());
        }
    }

    {
        auto catalog = Tether::Data::loadMotorCatalog();
        Tether::Calc::CalculateCurrentContext ctx;
        ctx.power = 1e-10;
        ctx.selectFromCatalog = true;
        ctx.loadType = Tether::Calc::LoadType::THREE_PHASE;

        auto result = Tether::Calc::currentByContext(catalog, ctx);
        reportTest("Zero power -> 0.0", result, 0.0);
    }

    // ---------- scada tests ----------
    using namespace Tether::SCADA;

    // DataType conversions
    {
        auto dt = stringToDataType("BOOL");
        reportTestBool("stringToDataType(BOOL) has value", dt.has_value(), true);
        if (dt.has_value()) {
            auto str = dataTypeToString(dt.value());
            reportTestOptionalString("dataTypeToString(BOOL)", str, "BOOL");
        }
    }
    {
        auto dt = stringToDataType("INT");
        reportTestBool("stringToDataType(INT) has value", dt.has_value(), true);
        if (dt.has_value()) {
            auto str = dataTypeToString(dt.value());
            reportTestOptionalString("dataTypeToString(INT)", str, "INT");
        }
    }
    {
        auto dt = stringToDataType("REAL");
        reportTestBool("stringToDataType(REAL) has value", dt.has_value(), true);
        if (dt.has_value()) {
            auto str = dataTypeToString(dt.value());
            reportTestOptionalString("dataTypeToString(REAL)", str, "REAL");
        }
    }
    {
        auto dt = stringToDataType("DWORD");
        reportTestBool("stringToDataType(DWORD) has value", dt.has_value(), true);
        if (dt.has_value()) {
            auto str = dataTypeToString(dt.value());
            reportTestOptionalString("dataTypeToString(DWORD)", str, "DWORD");
        }
    }
    {
        auto dt = stringToDataType("UNKNOWN");
        reportTestBool("stringToDataType(UNKNOWN) returns nullopt", dt.has_value(), false);
    }

    // Address allocation
    {
        auto a1 = allocateAddress(DataType::BOOL);
        auto a2 = allocateAddress(DataType::BOOL);
        auto a3 = allocateAddress(DataType::INT);
        auto a4 = allocateAddress(DataType::REAL);
        auto a5 = allocateAddress(DataType::REAL);
        auto a6 = allocateAddress(DataType::DWORD);
        auto a7 = allocateAddress(DataType::DWORD);

        reportTestOptionalInt("BOOL addr 1", a1, 15000);
        reportTestOptionalInt("BOOL addr 2", a2, 15001);
        reportTestOptionalInt("INT  addr 1", a3, 20000);
        reportTestOptionalInt("REAL addr 1", a4, 40000);
        reportTestOptionalInt("REAL addr 2", a5, 40002);
        reportTestOptionalInt("DWORD addr 1", a6, 35000);
        reportTestOptionalInt("DWORD addr 2", a7, 35002);
    }

    // ST generator
    {
        Tether::SCADA::Variable v;
        v.name = "TestVar";
        v.modbusAddress = 100;
        v.dataType = DataType::BOOL;
        auto lines = generateStLines(v);
        std::vector<std::string> expected = { "RegHoldingBuf[100] := TO_WORD(TestVar);" };
        reportTestLines("ST lines (BOOL)", lines, expected);
    }
    {
        Tether::SCADA::Variable v;
        v.name = "RealVar";
        v.modbusAddress = 200;
        v.dataType = DataType::REAL;
        auto lines = generateStLines(v);
        std::vector<std::string> expected = {
            "RegHoldingBuf[200] := TO_WORD(RealVar);",
            "RegHoldingBuf[201] := TO_WORD(RealVar);"
        };
        reportTestLines("ST lines (REAL)", lines, expected);
    }
    {
        Tether::SCADA::Variable v;
        v.name = "DwordVar";
        v.modbusAddress = 300;
        v.dataType = DataType::DWORD;
        auto lines = generateStLines(v);
        std::vector<std::string> expected = {
            "RegHoldingBuf[300] := TO_WORD(DwordVar);",
            "RegHoldingBuf[301] := TO_WORD(DwordVar);"
        };
        reportTestLines("ST lines (DWORD)", lines, expected);
    }

    // ================ SCADA REAL CSV FILE TEST ================
    {
        using namespace Tether::SCADA;
        using Tether::IO::openCsv;
        using Tether::IO::parseCsv;

        const char* csvPath = "examples/variables.csv";
        bool fileOpened = false;
        try {
            auto file = openCsv(csvPath);
            fileOpened = true;
            auto rows = parseCsv(file);
            reportTestBool("CSV file opened", fileOpened, true);

            if (rows.size() < 2) {
                std::printf("No CSV rows: expected >1, got %zu\n", rows.size());
                std::abort();
            }
            std::printf("Yes CSV row count: %zu rows total\n", rows.size());

            // Determine if header exists
            size_t expectedVarCount = (rows[0][0] == "Name") ? rows.size() - 1 : rows.size();
            auto vars = Tether::SCADA::buildVariablesFromCsv(rows);
            reportTestInt("CSV buildVariables count", (int)vars.size(), (int)expectedVarCount);

            if (!vars.empty()) {
                reportTestString("CSV first variable name", vars[0].name, "Motor_Start");
                auto typeStr = dataTypeToString(vars[0].dataType);
                reportTestOptionalString("CSV first variable type", typeStr, "BOOL");
                reportTestBool("CSV first variable address nonzero", vars[0].modbusAddress != 0, true);
                std::printf("Variable %s has modbus address: %u\n", vars[0].name.c_str(), vars[0].modbusAddress);
            }

            // Find a REAL variable and check address is nonzero
            bool foundReal = false;
            for (const auto& v : vars) {
                if (v.dataType == DataType::REAL) {
                    reportTestBool("CSV REAL address nonzero", v.modbusAddress != 0, true);
                    std::printf("Variable %s has modbus address: %u\n", v.name.c_str(), v.modbusAddress);
                    foundReal = true;
                    break;
                }
            }
            reportTestBool("CSV found REAL variable", foundReal, true);
        }
        catch (const std::exception& e) {
            std::printf("CSV file test failed: %s\n", e.what());
            reportTestBool("CSV file opened successfully", fileOpened, true);
            throw;
        }
    }

    std::printf("\nAll good tests passed with detailed output!\n");
    return 0;
}