#pragma once

#include <algorithm>
#include <optional>
#include <vector>
#include "equipment.hpp"

namespace Tether::Data {

	namespace {

        template <typename ItemType, typename Extractor>
        std::optional<ItemType> findNearest(const std::vector<ItemType>& catalog, double value, Extractor extractor) {
            if (catalog.empty()) {
                return std::nullopt;
            }

            auto item{ std::lower_bound(catalog.begin(), catalog.end(), value, extractor(value) };

            if (item == catalog.begin()) {
                return *item;
            }

            auto prevItem{ std::prev(item) };

            double curDiff{*item - value};
            double prevDiff{*prevItem - value};

            return (curDiff > prevDiff ? *prevItem : *item);
		}
	}

	std::optional<Breaker> selectBreaker(const std::vector<Breaker>& catalog, double value);

	std::optional<Cable> selectCable(const std::vector<Cable>& catalog, double value);

    std::optional<Motor> selectMotor(const std::vector<Motor>& catalog, double value);
}
