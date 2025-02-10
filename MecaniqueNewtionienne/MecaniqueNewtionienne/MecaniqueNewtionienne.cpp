#include <iostream>
#include <array>
#include "MathFunction.h"
#include "MathViewer.h"

void getTime(float& time, int& height, float& gravity);
std::array<float,10> gravities ={3.70,8.87,9.81,1.62,3.71,24.79,10.44,8.87,11.15,0.62};

int main()
{
    float time=0;
    int height=0;
    int planet = 0;
    std::cout << "Quelle est la planete d'ou vous voulez sauter ?\n";
    std::cout << " 1 - Mercure\n";
    std::cout << " 2 - Venus\n";
    std::cout << " 3 - Terre\n";
    std::cout << " 4 - Lune\n";
    std::cout << " 5 - Mars\n";
    std::cout << " 6 - Jupiter\n";
    std::cout << " 7 - Saturne\n";
    std::cout << " 8 - Uranus\n";
    std::cout << " 9 - Neptune\n";
    std::cout << " 10 - Pluton\n";
    std::cin >> planet;
    std::cout << "Quelle est la hauteur de depart ?\n";
    std::cin >> height;
    getTime(time,height,gravities[planet-1]);
    std::cout << "Vous vous ecraserez en " << time <<"secondes"<< std::endl;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Function Viewer");

    MathViewer viewer;
    viewer.setViewportSize({ 0, 50 }, { -1000, 0 });
    viewer.setView(window.getDefaultView());

    MathFunction fn([](const long double x) { return 2 * x * x * x + 7 * x * x + 4 * x - 3; }, sf::Color::Red);
    MathFunction fn2([](const long double x) { return 1 / x; }, sf::Color::Green);
    MathFunction fn3([](const long double x) { return x * x; }, sf::Color::Blue);
    MathFunction fn4([&](const long double t) { return -(t*t*gravities[planet - 1])/2 + height;}, sf::Color::Red);
    MathFunction fn5([&](const long double t) { return -(t * t * gravities[6]) / 2 + height; }, sf::Color::Green);
    MathFunction fn6([&](const long double t) { return -(t * t * gravities[4]) / 2 + height; }, sf::Color::Blue);
    viewer.addFunction(fn4);
    viewer.addFunction(fn5);
    viewer.addFunction(fn6);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                viewer.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
        }
        window.clear();
        window.draw(viewer);
        window.display();
    }
}

void getTime(float& time,int& height,float& gravity) {
    time = std::sqrt(2 * height / gravity);
}
