# EnumHelpers

A lightweight, header-only C++ library that provides compile-time utilities for converting C++ enums to their string representations.

## Features

- **Compile-time Enum to String Conversion**: Convert enum values to their string names at compile time using reflection-like capabilities
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

### Supported Enum Types

The library works with any standard C++ enum:

```cpp
enum class Color { Red, Green, Blue };
enum Status { Success, Failure, Pending };
```

Both unscoped and scoped enums are supported.

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

## Configuration

### Maximum Enum Size

The library currently supports enums with up to **32 values** by default. This limit is defined by the `MaxEnumSize` constant in the internal namespace. To support larger enums, modify:

```cpp
constexpr int MaxEnumSize = 64; // For enums with up to 64 values
```

## Requirements

- **C++17 or later** (for `std::string_view`, `constexpr` features, and `std::index_sequence`)
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

## Limitations

- Maximum of 32 enum values (configurable)
- Requires explicit enum value names—aliases with the same value may not be reliably distinguished
- Relies on compiler-specific function signature formatting

## License

[Specify your license here]

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.
