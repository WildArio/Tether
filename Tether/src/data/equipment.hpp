#pragma once

namespace Tether::Data {

    enum class Characteristic { A, B, C, D };

    struct Breaker {
        double nominalCurrent;      // A
        Characteristic characteristic;
        int poles;                  // 1..4
    };

    struct Cable {
        double crossSection;        // m² (e.g., 2.5e-6 for 2.5 mm²)
        double maxCurrent;          // A (maximum continuous current)
        double resistancePerMeter;  // Ω/m (conductor resistance at 20°C)
    };

    struct Motor {
        double power;               // W (active power)
        double nominalCurrent;      // A
        double voltage;             // V (RMS line-to-line)
        double cosPhi;              // power factor (dimensionless)
        double efficiency;          // efficiency (0..1, not percent)
        double startCurrentRatio;   // I_start / I_n (dimensionless)
    };
}
