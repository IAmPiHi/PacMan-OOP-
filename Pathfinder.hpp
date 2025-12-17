#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <cmath>

class Pathfinder {
public:
    static float calculateHeuristic(int x1, int y1, int x2, int y2);
    static std::vector<sf::Vector2i> findPath(const std::vector<std::string>& map, sf::Vector2i start, sf::Vector2i end);
};

#endif // PATHFINDER_HPP