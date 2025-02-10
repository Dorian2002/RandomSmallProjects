// Copyright (C) 2023 by Fabian INGREMEAU

#pragma once

#include "SFML/Graphics/Color.hpp"

#include <functional>

/**
 * @brief Representation of a drawable mathematical function.
 * @see MathFunctionViewer::addFunction for usage.
 * @code
 * //Example:
 * // Represents the y=x^2 function in red.
 * MathFunction fn([](const long double x) { return x * x; }, sf::Color::Red);
 * @endcode
 */
class MathFunction
{
    friend class MathFunctionViewer;

public:
    /**
     * @brief Creates a new drawable function for the @ref MathFunctionViewer
     * @param function The mathematical function to draw
     * @param color The color of the drawing
     */
    explicit MathFunction(std::function<long double(long double)> function, sf::Color color = sf::Color::White);
    ~MathFunction() = default;

    MathFunction(const MathFunction& other) = default;
    MathFunction(MathFunction&& other) noexcept;
    MathFunction& operator=(const MathFunction& other);
    MathFunction& operator=(MathFunction&& other) noexcept;

    /**
     * @brief Calculate the y position of the function at the given x position
     * @param x The x position
     * @return The y position
     */
    [[nodiscard]] long double calculatePoint(const long double x) const { return std::invoke(m_function, x); }

    /**
     * @brief Sets the drawing color of the function
     * @param color The new color value
     */
    void setColor(const sf::Color color) { m_color = color; }


    /**
     * @return The color of the function
     */
    [[nodiscard]] sf::Color color() const { return m_color; }

private:
    std::function<long double(long double)> m_function;
    sf::Color m_color;
};
