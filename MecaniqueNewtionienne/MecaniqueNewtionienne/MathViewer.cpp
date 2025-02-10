#include "MathViewer.h"

#include <format>

MathViewer::MathViewer()
{
    // Use a default font in current OS
#ifdef _WIN32
    m_font.loadFromFile("C:/Windows/Fonts/arial.ttf");
#elif __linux__
    m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#elif __apple__
    m_font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
#else
#   error "Unsupported Platform"
#endif
}

void MathViewer::drawUi(sf::RenderTarget& target, const sf::RenderStates states) const
{
    // Viewport data
    sf::Text viewport_x_label(std::format("Viewport X: [{};{}]", m_viewportX.first, m_viewportX.second), m_font, 20);
    viewport_x_label.setPosition(10, 10);
    viewport_x_label.setFillColor(sf::Color::White);

    sf::Text viewport_y_label(std::format("Viewport Y: [{};{}]", m_viewportY.first, m_viewportY.second), m_font, 20);
    viewport_y_label.setPosition(10, 35);
    viewport_y_label.setFillColor(sf::Color::White);

    const sf::Vector2f mouse_pos = target.mapPixelToCoords(sf::Mouse::getPosition(*dynamic_cast<sf::RenderWindow*>(&target)));
    sf::Text mouse_position_label(std::format("({:.2f}; {:.2f})", mouse_pos.x, mouse_pos.y), m_font, 15);
    mouse_position_label.setPosition(
        static_cast<float>(target.getSize().x) - mouse_position_label.getLocalBounds().width - 10,
        static_cast<float>(target.getSize().y) - mouse_position_label.getLocalBounds().height - 10);

    // Drawings
    const auto old_view = target.getView();
    target.setView(m_view);
    target.draw(viewport_x_label, states);
    target.draw(viewport_y_label, states);
    target.draw(mouse_position_label, states);
    target.setView(old_view);
}

void MathViewer::drawAxis(sf::RenderTarget& target, const sf::RenderStates states) const
{
    // Axis
    sf::VertexArray horizontal_axis(sf::Lines, 2);
    horizontal_axis[0].position = {static_cast<float>(m_viewportX.first), 0};
    horizontal_axis[1].position = {static_cast<float>(m_viewportX.second), 0};
    horizontal_axis[0].color = sf::Color::White;
    horizontal_axis[1].color = sf::Color::White;

    sf::VertexArray vertical_axis(sf::Lines, 2);
    vertical_axis[0].position = {0, static_cast<float>(m_viewportY.first)};
    vertical_axis[1].position = {0, static_cast<float>(m_viewportY.second)};
    vertical_axis[0].color = sf::Color::White;
    vertical_axis[1].color = sf::Color::White;

    // Ticks
    sf::VertexArray horizontal_axis_ticks(sf::Lines, 2 * (m_viewportX.second - m_viewportX.first));
    for (int i = 0; i < m_viewportX.second - m_viewportX.first; ++i)
    {
        horizontal_axis_ticks[2 * i].position = {static_cast<float>(m_viewportX.first + i), 0.1f};
        horizontal_axis_ticks[2 * i + 1].position = {static_cast<float>(m_viewportX.first + i), -0.1f};
        horizontal_axis_ticks[2 * i].color = sf::Color::White;
        horizontal_axis_ticks[2 * i + 1].color = sf::Color::White;
    }

    sf::VertexArray vertical_axis_ticks(sf::Lines, 2 * (m_viewportY.second - m_viewportY.first));
    for (int i = 0; i < m_viewportY.second - m_viewportY.first; ++i)
    {
        vertical_axis_ticks[2 * i].position = {0.1f, static_cast<float>(m_viewportY.first + i)};
        vertical_axis_ticks[2 * i + 1].position = {-0.1f, static_cast<float>(m_viewportY.first + i)};
        vertical_axis_ticks[2 * i].color = sf::Color::White;
        vertical_axis_ticks[2 * i + 1].color = sf::Color::White;
    }

    // Labels
    sf::Text horizontal_axis_label("X", m_font, 20);
    horizontal_axis_label.setPosition(
        static_cast<float>(target.getSize().x) - horizontal_axis_label.getLocalBounds().width - 5,
        static_cast<float>(target.getSize().y) / 2 + horizontal_axis_label.getLocalBounds().height / 2);

    sf::Text vertical_axis_label("Y", m_font, 20);
    vertical_axis_label.setPosition(
        static_cast<float>(target.getSize().x) / 2 + vertical_axis_label.getLocalBounds().width,
        vertical_axis_label.getLocalBounds().height / 2);

    // Drawings
    const auto old_view = target.getView();
    target.draw(horizontal_axis, states);
    target.draw(vertical_axis, states);
    target.draw(horizontal_axis_ticks, states);
    target.draw(vertical_axis_ticks, states);
    target.draw(vertical_axis_ticks, states);

    target.setView(m_view);
    target.draw(horizontal_axis_label, states);
    target.draw(vertical_axis_label, states);
    target.setView(old_view);
}

void MathViewer::drawFunction(sf::RenderTarget& target, const sf::RenderStates states, const MathFunction& fn) const
{
    const sf::Color function_color = fn.color();
    const std::size_t vertex_count = target.getSize().x;
    const float step = static_cast<float>(m_viewportX.second - m_viewportX.first) / static_cast<float>(target.getSize().x);

    sf::VertexArray line(sf::LinesStrip, vertex_count);
    for (unsigned int i = 0; i < vertex_count; i++)
    {
        const float x = static_cast<float>(m_viewportX.first) + static_cast<float>(i) * step;
        line[i].position = sf::Vector2f(x, static_cast<float>(fn.calculatePoint(x) * -1));
        line[i].color = function_color;
    }

    target.draw(line, states);
}

void MathViewer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
    const sf::View view(sf::FloatRect(static_cast<float>(m_viewportX.first), static_cast<float>(m_viewportY.first),
                                      static_cast<float>(std::abs(m_viewportX.first) + std::abs(m_viewportX.second)),
                                      static_cast<float>(std::abs(m_viewportY.first) + std::abs(m_viewportY.second))));
    target.setView(view);

    // Ui
    drawUi(target, states);

    // Axis
    drawAxis(target, states);

    // Functions
    for (const MathFunction& fn : m_functions)
        drawFunction(target, states, fn);
}
