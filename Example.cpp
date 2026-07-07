#include <iostream>
#include "EnumHelpers.h"

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
	std::cout << EnumHelpers::enum_to_string(testValue) << std::endl; // EnumValue2

	return 0;
}