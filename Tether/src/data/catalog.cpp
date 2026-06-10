#include <optional>
#include <vector>
#include "catalog.hpp"

namespace Tether::Data {
    namespace {

        // Breaker catalog (generation for now)
        std::vector<Breaker> createBreakerCatalog() {
            std::vector<Breaker> result;

            // Standard nominal currents (small and large)
            const std::vector<double> smallCurrents = {
                1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 13.0, 16.0,
                20.0, 25.0, 32.0, 40.0, 50.0, 63.0, 80.0, 100.0, 125.0
            };
            const std::vector<double> largeCurrents = {
                160.0, 250.0, 400.0, 630.0, 800.0, 1000.0, 1250.0, 1600.0
            };

            // Characteristic A (electronic protection, only small currents up to 16A)
            for (double i : smallCurrents) {
                if (i <= 16.0) {
                    result.push_back({ i, Characteristic::A, 3 });
                }
            }

            // Characteristic B (general, up to 125A)
            for (double i : smallCurrents) {
                result.push_back({ i, Characteristic::B, 3 });
            }

            // Characteristic C (general, small currents + some large up to 400A)
            for (double i : smallCurrents) {
                result.push_back({ i, Characteristic::C, 3 });
            }
            for (double i : largeCurrents) {
                if (i <= 400.0) {
                    result.push_back({ i, Characteristic::C, 3 });
                }
            }

            // Characteristic D (motor protection, from 16A upward, all large)
            for (double i : smallCurrents) {
                if (i >= 16.0) {
                    result.push_back({ i, Characteristic::D, 3 });
                }
            }
            for (double i : largeCurrents) {
                result.push_back({ i, Characteristic::D, 3 });
            }

            return result;
        }

        const std::vector<Breaker> breakerCatalog = createBreakerCatalog();

        // Cable catalog (manual for now)
        const std::vector<Cable> cableCatalog = {
            // cross_section(m²), max_current(A), resistance_per_meter(Ohm/m)
            { 1.5e-6,       19,         12.3 / 1000.0 },
            { 2.5e-6,       25,         7.38 / 1000.0 },
            { 4.0e-6,       35,         4.60 / 1000.0 },
            { 6.0e-6,       42,         3.07 / 1000.0 },
            { 10.0e-6,      55,         1.84 / 1000.0 },
            { 16.0e-6,      75,         1.15 / 1000.0 },
            { 25.0e-6,      95,         0.74 / 1000.0 },
            { 35.0e-6,      120,        0.52 / 1000.0 },
            { 50.0e-6,      145,        0.37 / 1000.0 },
            { 70.0e-6,      180,        0.26 / 1000.0 },
            { 95.0e-6,      220,        0.194 / 1000.0 },
            { 120.0e-6,     260,        0.153 / 1000.0 },
            { 150.0e-6,     305,        0.122 / 1000.0 },
            { 185.0e-6,     350,        0.099 / 1000.0 },
            { 240.0e-6,     445,        0.077 / 1000.0 }
        };

        // Motor catalog (manual for now)
        const std::vector<Motor> motorCatalog = {
            {  370.0,  1.30, 380.0, 0.70, 0.620, 4.7 },
            {  550.0,  1.80, 380.0, 0.72, 0.650, 4.7 },
            {  750.0,  2.29, 380.0, 0.73, 0.690, 5.3 },
            { 1100.0,  3.18, 380.0, 0.74, 0.720, 5.5 },
            { 1500.0,  4.00, 380.0, 0.75, 0.760, 5.5 },
            { 2200.0,  5.60, 380.0, 0.76, 0.790, 6.5 },
            { 3000.0,  7.40, 380.0, 0.76, 0.810, 6.5 },
            { 4000.0,  9.75, 380.0, 0.76, 0.820, 6.5 },
            { 5500.0, 12.90, 380.0, 0.77, 0.840, 6.5 },
            { 7500.0, 17.20, 380.0, 0.77, 0.860, 6.5 },
            {11000.0, 24.50, 380.0, 0.78, 0.875, 6.5 },
            {15000.0, 31.60, 380.0, 0.81, 0.890, 7.0 },
            {18500.0, 38.60, 380.0, 0.81, 0.900, 7.0 },
            {22000.0, 44.70, 380.0, 0.84, 0.915, 7.0 },
            {30000.0, 59.30, 380.0, 0.85, 0.920, 7.0 },
            {37000.0, 71.00, 380.0, 0.86, 0.925, 7.0 },
            {45000.0, 86.00, 380.0, 0.86, 0.925, 7.0 },
            {55000.0,104.00, 380.0, 0.86, 0.928, 7.0 },
            {75000.0,142.00, 380.0, 0.86, 0.935, 6.7 },
            {90000.0,169.00, 380.0, 0.86, 0.938, 6.7 },
            {110000.0,207.00, 380.0, 0.86, 0.940, 6.7 },
            {132000.0,245.00, 380.0, 0.87, 0.942, 6.7 },
            {160000.0,292.00, 380.0, 0.88, 0.945, 6.7 },
            {200000.0,365.00, 380.0, 0.88, 0.945, 6.7 },
            {250000.0,457.00, 380.0, 0.88, 0.945, 6.7 },
        };

    }

    std::vector<Breaker> loadBreakerCatalog() { return breakerCatalog; }
    std::vector<Cable> loadCableCatalog() { return cableCatalog; }
    std::vector<Motor> loadMotorCatalog() { return motorCatalog; }



}