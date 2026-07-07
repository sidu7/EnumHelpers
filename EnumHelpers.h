#pragma once

#include <string>
#include <string_view>
#include <array>
#include <utility>

#ifndef FUNCNAME
#if defined(__clang__) || defined(__GNUC__)
#define FUNCNAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNCNAME __FUNCSIG__
#endif
#endif

namespace EnumHelpers
{
    constexpr int MaxEnumSize = 32;

    // ==============================================================================
    // 1. A Struct to hold our valid pairs
    // ==============================================================================
    template <typename E>
    struct EnumEntry
    {
        E value;
        std::string_view name;
    };

    // ==============================================================================
    // 2. The Extractor
    // ==============================================================================
    template <typename E, E V>
    constexpr std::string_view extract_name()
    {
        std::string_view name = FUNCNAME;
        size_t start = name.find_last_of(',') + 1;
        size_t end = name.find_last_of('>');
        return name.substr(start, end - start);
    }

    // ==============================================================================
    // 3. The "Smart" Table Builder
    // ==============================================================================
    template <typename E, std::size_t... Indices>
    constexpr auto build_packed_table(std::index_sequence<Indices...>)
    {

        // A. Build a temporary compile-time array of ALL strings (including garbage)
        constexpr std::array<std::string_view, sizeof...(Indices)> temp_names = {
            extract_name<E, static_cast<E>(Indices)>()...
        };

        // B. Count the valid ones! 
        // If the compiler outputs a cast like "(AIState)5", it has a parenthesis.
        // So, if there is NO parenthesis, it is a valid enum name. We count those.
        constexpr std::size_t valid_count = (
            ((temp_names[Indices].find('(') == std::string_view::npos) ? 1 : 0) + ...
            );

        // C. Create a perfectly sized array to hold only the good entries
        std::array<EnumEntry<E>, valid_count> packed_table{};

        // D. Loop through the temporary array and copy only the valid ones
        std::size_t packed_idx = 0;
        for (std::size_t i = 0; i < temp_names.size(); ++i)
        {
            if (temp_names[i].find('(') == std::string_view::npos)
            {
                // Save both the integer value AND the string name
                packed_table[packed_idx] = EnumEntry<E>{ static_cast<E>(i), temp_names[i] };
                packed_idx++;
            }
        }

        return packed_table; // The compiler throws the 'temp_names' array in the trash!
    }

    // ==============================================================================
    // 4. The User-Facing Function
    // ==============================================================================
    template <typename E>
    constexpr std::string enum_to_string(E value)
    {
        // Generate the perfectly packed table (checks 0 through 15)
        constexpr auto table = build_packed_table<E>(std::make_index_sequence<MaxEnumSize>{});

        // Because the table is packed, we can't just use array[index] anymore.
        // We do a blazing-fast linear search over our tiny valid table.
        for (const auto& entry : table)
        {
            if (entry.value == value)
            {
                return std::string(entry.name);
            }
        }
        return "";
    }
};