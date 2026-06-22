#pragma once
#include <cassert>
#include <algorithm>
#include <optional>
#include <vector>
#include "equipment.hpp"

namespace Tether::Data::Detail {

    template <typename ItemType, typename Extractor>
    auto findFirstGreaterOrEqual(const std::vector<ItemType>& catalog, double value, Extractor extract) {
        return  std::lower_bound(catalog.begin(), catalog.end(), value,
            [&extract](const ItemType& elem, double val) { return extract(elem) < val; });
    }

    template <typename ItemType, typename Extractor>
    const ItemType& findNearest(const std::vector<ItemType>& catalog, double value, Extractor extract) {
        assert(!catalog.empty() && "Empty catalog in findNearest");

        auto item{ findFirstGreaterOrEqual(catalog, value, extract) };

        if (item == catalog.begin()) {
            return *item;
        }

        if (item == catalog.end()) {
            return *std::prev(item);
        }

        auto prevItem{ std::prev(item) };

        double curDiff{ std::abs(extract(*item) - value) };
        double prevDiff{ std::abs(extract(*prevItem) - value) };

        constexpr double relEpsilon{ 1e-8 };

        if (std::abs(prevDiff - curDiff) < relEpsilon) {
            return *prevItem;
        }

        return (prevDiff < curDiff ? *prevItem : *item);
    }
}

namespace Tether::Data {

    std::optional<Breaker> selectEquipment(const std::vector<Breaker>& catalog, double current, double heatCoef = 1.0);

	std::optional<Cable> selectEquipment(const std::vector<Cable>& catalog, double value);

    std::optional<Motor> selectEquipment(const std::vector<Motor>& catalog, double value);
}
    