// Copyright (C) 2023 by Fabian INGREMEAU

#pragma once

#include "MathFunction.h"

#include "SFML/Graphics.hpp"

#include <functional>

/**
 * @brief A class that can be used to draw a MathFunction to a sf::RenderTarget.
 * @code
 * // Example:
 * auto window = sf::RenderWindow(sf::VideoMode(800, 800), "Function Viewer");
 * MathViewer viewer;
 *  viewer.setViewportSize({ -10, 10 }, { -10, 10 });
 *   viewer.setView(window.getDefaultView());
 *    MathFunction fn([](const long double x) { return x * x; }, sf::Color::Red);
 *
 * while (window.isOpen())
 * {
 *     sf::Event event {};
 *     while (window.pollEvent(event))
 *     {
 *          if (event.type == sf::Event::Closed)
 *              window.close();
 *     }
 *      window.clear();
 *     window.draw(viewer);
 *     window.display();
 *  }
 * @endcode
 */
class MathViewer final : public sf::Drawable
{
public:
    /**
     * @brief Creates a new viewer to see @ref MathFunction.
     */
    explicit MathViewer();
    ~MathViewer() override = default;

    MathViewer(const MathViewer& other) = delete;
    MathViewer(MathViewer&& other) noexcept = delete;
    MathViewer& operator=(const MathViewer& other) = delete;
    MathViewer& operator=(MathViewer&& other) noexcept = delete;

    /**
     * @brief Sets the viewport size where the function will be drawn.
     * @param x The horizontal range of the viewport
     * @param y The vertical range of the viewport
     */
    void setViewportSize(const std::pair<int, int>& x, const std::pair<int, int>& y)
    {
        m_viewportX = x;
        m_viewportY = y;
    }

    /**
     * @brief Sets the visible area view in the window
     * @param view The new visible area view
     */
    void setView(const sf::View& view) { m_view = view; }

    /**
     * @brief Adds a function in the drawing queue
     * @param fn The function to add to the graph
     */
    void addFunction(const MathFunction& fn) { m_functions.push_back(fn); }

    /**
     * @brief Removes a function from the drawing queue
     * @param fn The function to remove from the graph
     */
    void removeFunction(const MathFunction& fn) { m_functions.push_back(fn); }

    /**
     * @return The functions in the drawing queue
     */
    [[nodiscard]] std::vector<MathFunction> functions() const noexcept { return m_functions; }

    /**
     * @return The viewport size of the viewer
     */
    [[nodiscard]] std::pair<std::pair<int, int>, std::pair<int, int>> viewportSize() const noexcept
    {
        return std::make_pair(m_viewportX, m_viewportY);
    }

protected:
    void drawUi(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawAxis(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawFunction(sf::RenderTarget& target, sf::RenderStates states, const MathFunction& fn) const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::pair<int, int> m_viewportX = {-10, 10}, m_viewportY = {-10, 10};
    std::vector<MathFunction> m_functions = {};
    sf::View m_view = {};
    sf::Font m_font = {};
};
