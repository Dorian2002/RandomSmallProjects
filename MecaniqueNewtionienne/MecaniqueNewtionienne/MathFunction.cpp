#include "MathFunction.h"

MathFunction::MathFunction(std::function<long double(long double)> function, const sf::Color color)
	: m_function(std::move(function)), m_color(color)
{
}

MathFunction::MathFunction(MathFunction&& other) noexcept
	: m_function{ std::move(other.m_function) }, m_color{other.m_color}
{
}

MathFunction& MathFunction::operator=(const MathFunction& other)
{
	if (this == &other)
		return *this;
	m_function = other.m_function;
	m_color = other.m_color;
	return *this;
}

MathFunction& MathFunction::operator=(MathFunction&& other) noexcept
{
	if (this == &other)
		return *this;
	m_function = std::move(other.m_function);
	m_color = other.m_color;
	return *this;
}
