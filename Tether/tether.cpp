#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "tether.hpp"

// Вспомогательные функции для безопасного ввода чисел
double getDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double val;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int getInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int val;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

char getChar(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        char c;
        if (std::cin >> c) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return c;
        }
        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Прототип функции запуска всех тестов (определена в tests.cpp)
void runAllTests();

int main() {
    // Загружаем глобальные каталоги (могут использоваться в меню)
    const auto& breakerCatalog = Tether::Data::loadBreakerCatalog();
    const auto& cableCatalog = Tether::Data::loadCableCatalog();
    const auto& motorCatalog = Tether::Data::loadMotorCatalog();

    while (true) {
        std::cout << "\n=== Tether Engineering Calculator ===\n";
        std::cout << "1. Calculate three-phase current\n";
        std::cout << "2. Calculate single-phase current\n";
        std::cout << "3. Select breaker\n";
        std::cout << "4. Select cable\n";
        std::cout << "5. Select motor\n";
        std::cout << "6. SCADA utilities\n";
        std::cout << "7. Run tests\n";
        std::cout << "0. Exit\n";
        int choice = getInt("Your choice: ");

        if (choice == 0) {
            std::cout << "Exiting.\n";
            break;
        }

        switch (choice) {
        case 1: {
            double P = getDouble("Enter power (W): ");
            double U = getDouble("Enter voltage (V): ");
            double cosPhi = getDouble("Enter cos(phi) (0..1): ");
            double eff = getDouble("Enter efficiency (0..1): ");
            Tether::Calc::CalculateCurrentContext ctx;
            ctx.power = P;
            ctx.voltage = U;
            ctx.cosPhi = cosPhi;
            ctx.eff = eff;
            ctx.selectFromCatalog = false;
            ctx.loadType = Tether::Calc::LoadType::THREE_PHASE;
            auto current = Tether::Calc::currentByContext(motorCatalog, ctx);
            if (current.has_value())
                std::cout << "Calculated current: " << current.value() << " A\n";
            else
                std::cout << "Calculation failed.\n";
            break;
        }
        case 2: {
            double P = getDouble("Enter power (W): ");
            double U = getDouble("Enter voltage (V): ");
            double cosPhi = getDouble("Enter cos(phi) (0..1): ");
            double eff = getDouble("Enter efficiency (0..1): ");
            Tether::Calc::CalculateCurrentContext ctx;
            ctx.power = P;
            ctx.voltage = U;
            ctx.cosPhi = cosPhi;
            ctx.eff = eff;
            ctx.selectFromCatalog = false;
            ctx.loadType = Tether::Calc::LoadType::ONE_PHASE;
            auto current = Tether::Calc::currentByContext(motorCatalog, ctx);
            if (current.has_value())
                std::cout << "Calculated current: " << current.value() << " A\n";
            else
                std::cout << "Calculation failed.\n";
            break;
        }
        case 3: {
            double current = getDouble("Enter calculated current (A): ");
            double heatCoef = getDouble("Enter heat coefficient (e.g. 1.0): ");
            auto res = Tether::Data::selectBreaker(breakerCatalog, current, heatCoef);
            if (res.has_value())
                std::cout << "Selected breaker: " << res->nominalCurrent << " A\n";
            else
                std::cout << "Breaker catalog is empty.\n";
            break;
        }
        case 4: {
            double current = getDouble("Enter calculated current (A): ");
            auto res = Tether::Data::selectCable(cableCatalog, current);
            if (res.has_value()) {
                std::cout << "Selected cable: " << res->maxCurrent << " A, "
                    << res->crossSection * 1e6 << " mm2\n";
            }
            else {
                std::cout << "Cable catalog is empty.\n";
            }
            break;
        }
        case 5: {
            double power = getDouble("Enter mechanical power (W): ");
            auto res = Tether::Data::selectMotor(motorCatalog, power);
            if (res.has_value())
                std::cout << "Selected motor: " << res->power << " W, "
                << res->nominalCurrent << " A\n";
            else
                std::cout << "Motor catalog is empty.\n";
            break;
        }
        case 6: {
            std::cout << "\n-- SCADA utilities --\n";
            std::cout << "a) Convert string to DataType\n";
            std::cout << "b) Convert DataType to string\n";
            std::cout << "c) Allocate Modbus address\n";
            std::cout << "d) Generate ST lines\n";
            std::cout << "e) Parse CSV variables\n";
            char sub = getChar("Choose sub-option: ");
            if (sub == 'a') {
                std::string s;
                std::cout << "Enter type string (e.g. BOOL, INT, REAL): ";
                std::cin >> s;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto dt = Tether::SCADA::stringToDataType(s);
                if (dt.has_value())
                    std::cout << "DataType value: " << static_cast<int>(dt.value()) << "\n";
                else
                    std::cout << "Unknown type.\n";
            }
            else if (sub == 'b') {
                int v = getInt("Enter DataType value (0=BOOL, 1=INT, 2=UINT, 3=REAL, 4=WORD, 5=DWORD, 6=UNKNOWN): ");
                if (v >= 0 && v <= 6) {
                    auto str = Tether::SCADA::dataTypeToString(static_cast<Tether::SCADA::DataType>(v));
                    if (str.has_value())
                        std::cout << "String: " << str.value() << "\n";
                    else
                        std::cout << "Unknown type.\n";
                }
                else {
                    std::cout << "Invalid value.\n";
                }
            }
            else if (sub == 'c') {
                std::string s;
                std::cout << "Enter DataType string: ";
                std::cin >> s;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto dt = Tether::SCADA::stringToDataType(s);
                if (dt.has_value()) {
                    auto addr = Tether::SCADA::allocateAddress(dt.value());
                    if (addr.has_value())
                        std::cout << "Allocated Modbus address: " << addr.value() << "\n";
                    else
                        std::cout << "Allocation failed.\n";
                }
                else {
                    std::cout << "Unknown type.\n";
                }
            }
            else if (sub == 'd') {
                Tether::SCADA::Variable v;
                std::cout << "Enter variable name: ";
                std::cin >> v.name;
                v.modbusAddress = getInt("Enter Modbus address: ");
                std::string ts;
                std::cout << "Enter DataType string: ";
                std::cin >> ts;
                auto dt = Tether::SCADA::stringToDataType(ts);
                if (!dt.has_value()) {
                    std::cout << "Unknown type.\n";
                    break;
                }
                v.dataType = dt.value();
                auto lines = Tether::SCADA::generateStLines(v);
                if (lines.has_value()) {
                    std::cout << "Generated ST code:\n";
                    for (const auto& line : lines.value())
                        std::cout << line << "\n";
                }
                else {
                    std::cout << "Generation failed.\n";
                }
            }
            else if (sub == 'e') {
                std::string path = "examples/variables.csv";
                std::cout << "Reading CSV from: " << path << "\n";
                try {
                    auto file = Tether::IO::openCsv(path);
                    auto rows = Tether::IO::parseCsv(file);
                    auto vars = Tether::SCADA::buildVariablesFromCsv(rows);
                    std::cout << "Parsed " << vars.size() << " variables.\n";
                    for (const auto& var : vars) {
                        auto ts = Tether::SCADA::dataTypeToString(var.dataType);
                        std::cout << var.name << " @ " << var.modbusAddress
                            << " (" << (ts.has_value() ? ts.value() : "?") << ")\n";
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "CSV error: " << e.what() << "\n";
                }
            }
            else {
                std::cout << "Invalid sub-option.\n";
            }
            break;
        }
        case 7: {
            std::cout << "Running tests...\n";
            runAllTests();
            std::cout << "All tests passed successfully.\n";
            break;
        }
        default:
            std::cout << "Invalid option.\n";
        }
    }
    return 0;
}