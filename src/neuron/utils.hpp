#pragma once

#include <variant>

namespace neuron {

    namespace detail {
        // Copied from https://stackoverflow.com/questions/52303316/get-index-by-type-in-stdvariant
        template<typename VariantType, typename T, std::size_t index = 0>
        consteval std::size_t _variant_index() {
            static_assert(std::variant_size_v<VariantType> > index, "Type not found in variant");
            if constexpr (index == std::variant_size_v<VariantType>) {
                return index;
            } else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
                return index;
            } else {
                return _variant_index<VariantType, T, index + 1>();
            }
        }
    }

    template<typename VariantType, typename T>
    constexpr std::size_t variant_index = detail::_variant_index<VariantType, T>();

}
