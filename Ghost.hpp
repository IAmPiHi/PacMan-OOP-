#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.hpp"

// 前向宣告，避免循環 include
class PacMan; 

class Ghost {
public:
    sf::CircleShape shape;
    sf::Vector2i gridPosition;
    std::vector<sf::Vector2i> path;
    size_t pathIndex;
    int moveCounter = 0;
    int moveDelay;
    sf::Color originalColor;

    Ghost(int x, int y, sf::Color color, int delay);
    void update(const std::vector<std::string>& map, const PacMan& pacman, bool isScared);
    void updateScreenPosition();
};

#endif // GHOST_HPP