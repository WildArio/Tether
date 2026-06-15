#pragma once

// Data structures
#include "data/equipment.hpp"
#include "data/catalog.hpp"

// Calculation modules
#include "calc/consts.hpp"
#include "calc/current.hpp"
#include "calc/voltage.hpp"

// Calculation modules
#include "selection/breaker_selection.hpp"
#include "selection/cable_selection.hpp"
#include "selection/motor_selection.hpp"

// Core utilities
#include "core/algorithm_utils.hpp"
#include "core/math_utils.hpp"
#include "core/io/csv_parser.hpp"

// SCADA
#include "scada/address_allocator.hpp"
#include "scada/data_type.hpp"
#include "scada/st_generator.hpp"
#include "scada/variable.hpp"
