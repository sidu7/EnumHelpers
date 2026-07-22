#pragma once

#include <string>
#include <string_view>
#include <array>
#include <utility>
#include <optional>

#ifndef FUNCNAME
#if defined(__clang__) || defined(__GNUC__)
#define FUNCNAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNCNAME __FUNCSIG__
#endif
#endif

namespace EnumHelpers
{
    namespace internal
    {
        constexpr int MinEnumSize = -32;
        constexpr int MaxEnumSize = 32;

        template <typename E>
        struct EnumEntry
        {
            E value;
            std::string_view name;
        };

        template <typename E, E V>
        constexpr std::string_view extract_name()
        {
            std::string_view name = FUNCNAME;
            size_t start = name.find_last_of(',') + 1;
            size_t end = name.find_last_of('>');
            name = name.substr(start, end - start);
            size_t enumClassEnd = name.find_last_of(':');
			if (enumClassEnd != std::string_view::npos)
			{
				name = name.substr(enumClassEnd + 1);
			}
            return name;
        }

        template <typename E, std::size_t... Indices>
        constexpr auto build_packed_table(std::index_sequence<Indices...>)
        {

            // A. Build a temporary compile-time array of ALL strings (including garbage)
            constexpr std::array<std::string_view, sizeof...(Indices)> temp_names = {
                extract_name<E, static_cast<E>(Indices + internal::MinEnumSize)>()...
            };

            // B. Count the valid ones! 
            // If the compiler outputs a cast like "(EnumType)5", it has a parenthesis.
            // So, if there is NO parenthesis, it is a valid enum name. We count those.
            constexpr std::size_t valid_count = (
                ((temp_names[Indices].find(')') == std::string_view::npos) ? 1 : 0) + ...
                );

            // C. Create a perfectly sized array to hold only the good entries
            std::array<EnumEntry<E>, valid_count> packed_table{};

            // D. Loop through the temporary array and copy only the valid ones
            std::size_t packed_idx = 0;
            for (std::size_t i = 0; i < temp_names.size(); ++i)
            {
                if (temp_names[i].find(')') == std::string_view::npos)
                {
                    packed_table[packed_idx] = EnumEntry<E>{ static_cast<E>(i + internal::MinEnumSize), temp_names[i] };
                    packed_idx++;
                }
            }

            return packed_table; // The compiler throws the 'temp_names' array in the trash!
        }
    }

    template <typename E>
    constexpr std::string enum_to_string(E value)
    {
        constexpr auto table = internal::build_packed_table<E>(std::make_index_sequence<internal::MaxEnumSize - internal::MinEnumSize>{});

        for (const auto& entry : table)
        {
            if (entry.value == value)
            {
                return std::string(entry.name);
            }
        }
        return "";
    }

    template <typename E>
    constexpr std::optional<E> enum_from_string(const std::string& name)
    {
        constexpr auto table = internal::build_packed_table<E>(std::make_index_sequence<internal::MaxEnumSize - internal::MinEnumSize>{});

        for (const auto& entry : table)
        {
            if (entry.name == name)
            {
                return entry.value;
            }
        }
        return std::nullopt;
    }
};