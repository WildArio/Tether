// test_current.cpp
#include <cassert>
#include <optional>
#include <vector>
#include <cstdio>
#include <cmath>
#include "tether.hpp"

constexpr double EPS = 1e-6;

void report_test(const char* name, std::optional<double> actual, double expected) {
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

void report_test_bool(const char* name, bool actual, bool expected) {
    std::printf("%s %s: got %s, expected %s\n",
        (actual == expected) ? "Yes" : "No",
        name,
        actual ? "true" : "false",
        expected ? "true" : "false"
    );
    if (actual != expected) std::abort();
}

int main() {
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
        double expected = 1500.0 / (380.0 * 0.9 * Tether::Calc::SQRT3 * 0.85);
        report_test("Three-phase manual calc", result, expected);
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
        report_test("Single-phase manual calc", result, expected);
    }

    {
        auto catalog = Tether::Data::loadMotorCatalog();
        Tether::Calc::CalculateCurrentContext ctx;
        ctx.power = 1300.0;
        ctx.selectFromCatalog = true;
        ctx.loadType = Tether::Calc::LoadType::THREE_PHASE;

        auto result = Tether::Calc::currentByContext(catalog, ctx);
        report_test_bool("Interpolation returns value", result.has_value(), true);
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
        report_test("Zero power -> 0.0", result, 0.0);
    }

    std::printf("\nAll good tests passed with detailed output!\n");
    return 0;
}