#pragma once

struct Utils
{
	template<typename T>
	T minValue(T valueA, T valueB)
	{
		return (valueA < valueB) ? valueA : valueB;
	};

	template<typename T>
	T maxValue(T valueA, T valueB)
	{
		return (valueA > valueB) ? valueA : valueB;
	};
};