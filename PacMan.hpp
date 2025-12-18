#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.hpp"

class PacMan {
public:
    sf::VertexArray shape;
    sf::Vector2i gridPosition;
    sf::Vector2i nextDirection;    
    sf::Vector2i currentDirection; 
    float currentAngle = 0.0f;
    sf::Color currentColor = sf::Color::Yellow;

    // --- 修改：平滑動畫所需的變數 ---
    float currentMouthHalfAngle; // 當前嘴巴張開的一半角度 (0.0 ~ 40.0)
    bool isMouthOpening;         // true = 正在張開, false = 正在閉合
    // ----------------------------

    PacMan(int x, int y);
    void setDirection(sf::Vector2i dir);
    void update(const std::vector<std::string>& map);
    void updateAnimation(); // 負責平滑更新角度
    void updateGeometry();
    void reset(sf::Vector2i startPos);
};


#endif // PACMAN_HPP
