#include <iostream>
#include "EnumHelpers.h"

enum Colors
{
	Red = -10,
	Green = 3,
	Blue = 7,
	Yellow,
	Purple	
};

enum class Fruits
{
	Apple = -1,
	Banana,
	Orange = 4,
	Pear,
	Strawberry
};

int main()
{
	Colors colorValue = Green;
	std::cout << EnumHelpers::enum_to_string(colorValue) << std::endl; // Green
	std::cout << static_cast<int32_t>(EnumHelpers::enum_from_string<Colors>("Yellow").value_or(Colors::Red)) << std::endl; // 8


	Fruits fruitValue = Fruits::Apple;
	std::cout << EnumHelpers::enum_to_string(fruitValue) << std::endl; // Apple
	std::cout << static_cast<int32_t>(EnumHelpers::enum_from_string<Fruits>("Banana").value_or(Fruits::Apple)) << std::endl; // 0

	return 0;
}