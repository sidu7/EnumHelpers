# EnumHelpers

A lightweight, header-only C++ library that provides compile-time utilities for converting C++ enums to their string representations.

## Features

- **Compile-time Enum to String Conversion**: Convert enum values to their string names at compile time using reflection-like capabilities
- **Bidirectional Conversion**: Convert strings back to enum values with `enum_from_string`
- **Negative Enum Support**: Full support for enums with negative values
- **Header-Only Library**: No external dependencies or build complications—just include `EnumHelpers.h`
- **Cross-Platform Support**: Works with Clang, GCC, and MSVC compilers
- **Zero Runtime Overhead**: String extraction happens at compile time using constexpr

## How It Works

EnumHelpers uses compiler-specific function signatures to extract enum names at compile time. The library leverages:

- **Compiler Introspection**: Uses `__PRETTY_FUNCTION__` (Clang/GCC) or `__FUNCSIG__` (MSVC) to extract enum names from the function signature
- **Compile-Time Filtering**: Automatically filters out invalid enum values (those with numeric casts) and builds a packed table of valid enums
- **String Views**: Efficient compile-time string handling using `std::string_view`

## Usage

### Basic Example

```cpp
#include "EnumHelpers.h"
#include <iostream>

enum TestEnum
{
    EnumValue0,
    EnumValue1,
    EnumValue2,
    EnumValue3,
    EnumValue4,
};

int main()
{
    TestEnum testValue = EnumValue2;
    std::cout << EnumHelpers::enum_to_string(testValue) << std::endl; // Output: EnumValue2
    
    return 0;
}
```

### Bidirectional Conversion

```cpp
enum Status { Success = 0, Failure = -1, Pending = 1 };

// Convert enum to string
std::string status_str = EnumHelpers::enum_to_string(Failure); // "Failure"

// Convert string back to enum
auto status = EnumHelpers::enum_from_string<Status>("Failure"); // std::optional<Status>
if (status.has_value()) {
    std::cout << "Parsed status: " << static_cast<int>(status.value()) << std::endl; // -1
}
```

### Supported Enum Types

The library works with any standard C++ enum:

```cpp
enum class Color { Red, Green, Blue };
enum Status { Success, Failure, Pending };
enum class SignedStatus { Error = -5, Idle = 0, Active = 5 };
```

Both unscoped and scoped enums are supported, including enums with negative values.

## API Reference

### `enum_to_string(E value)`

Converts an enum value to its string representation.

**Template Parameters:**
- `E` - The enum type

**Parameters:**
- `value` - The enum value to convert

**Returns:**
- `std::string` - The name of the enum value, or an empty string if not found

**Example:**
```cpp
Color myColor = Color::Red;
std::string colorName = EnumHelpers::enum_to_string(myColor); // "Red"
```

### `enum_from_string(const std::string& name)`

Converts a string to its corresponding enum value.

**Template Parameters:**
- `E` - The enum type

**Parameters:**
- `name` - The string representation of the enum value

**Returns:**
- `std::optional<E>` - The enum value if found, `std::nullopt` otherwise

**Example:**
```cpp
auto color = EnumHelpers::enum_from_string<Color>("Red");
if (color) {
    std::cout << "Found color!" << std::endl;
}
```

## Configuration

### Enum Value Range

The library supports enums with values in the range **[-32, 31]** by default. This is defined by:
- `MinEnumSize = -32`
- `MaxEnumSize = 32`

To support a different range, modify these constants in the internal namespace:

```cpp
constexpr int MinEnumSize = -64;  // For negative values down to -64
constexpr int MaxEnumSize = 64;   // For positive values up to 63
```

The total range span must not exceed what your compiler can handle within `std::index_sequence`.

## Requirements

- **C++17 or later** (for `std::string_view`, `constexpr` features, `std::index_sequence`, and `std::optional`)
- One of the following compilers:
  - Clang
  - GCC
  - MSVC

## Building

This is a header-only library. Simply include `EnumHelpers.h` in your project.

### Using CMake

```cmake
target_include_directories(your_target PUBLIC path/to/EnumHelpers)
```

### Manual Inclusion

Copy `EnumHelpers.h` to your project and include it:

```cpp
#include "EnumHelpers.h"
```

## Technical Details

### Compile-Time Processing

1. **Extract Names**: Uses compiler introspection to extract enum names from template function signatures
2. **Filter Valid Values**: Identifies which numeric indices correspond to actual enum values (vs. casts to invalid numbers)
3. **Build Lookup Table**: Creates a compact array of valid enum entries at compile time
4. **Runtime Lookup**: Performs a simple linear search in the pre-built table during runtime

This approach ensures:
- No runtime parsing or reflection overhead
- String data is baked into the binary at compile time
- Minimal memory footprint

### Negative Enum Support

The library handles negative enum values by using a configurable `MinEnumSize` constant. The internal lookup table spans from `MinEnumSize` to `MaxEnumSize`, allowing full support for negative values within the configured range.

## Limitations

- Maximum supported enum value range is determined by `MaxEnumSize - MinEnumSize` (default: 64 values)
- Requires explicit enum value names—aliases with the same value may not be reliably distinguished
- Relies on compiler-specific function signature formatting
- `enum_from_string` is case-sensitive and requires exact string matches
