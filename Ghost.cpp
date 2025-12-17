#include "Ghost.hpp"
#include "PacMan.hpp"
#include "Pathfinder.hpp"

Ghost::Ghost(int x, int y, sf::Color color, int delay) : moveDelay(delay), originalColor(color) {
    gridPosition = {x, y};
    pathIndex = 0;
    shape.setRadius(TILE_SIZE / 2.0f - 3);
    shape.setFillColor(color);
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    updateScreenPosition();
}

void Ghost::update(const std::vector<std::string>& map, const PacMan& pacman, bool isScared) {
    if (isScared) shape.setFillColor(sf::Color(100, 100, 255));
    else shape.setFillColor(originalColor);

    moveCounter++;
    if (moveCounter < moveDelay) {
        return;
    }
    moveCounter = 0;

    sf::Vector2i target = pacman.gridPosition;

    // --- 鬼魂逃跑邏輯 ---
    if (isScared) {
        int h = static_cast<int>(map.size());
        int w = static_cast<int>(map[0].size());
        std::vector<sf::Vector2i> corners = {
            {1, 1}, {w - 2, 1}, {1, h - 2}, {w - 2, h - 2}
        };
        float maxDist = -1.0f;
        for (const auto& corner : corners) {
            float dist = Pathfinder::calculateHeuristic(corner.x, corner.y, pacman.gridPosition.x, pacman.gridPosition.y);
            if (dist > maxDist) {
                maxDist = dist;
                target = corner;
            }
        }
    }
    // ------------------

    path = Pathfinder::findPath(map, gridPosition, target);
    pathIndex = 0;
    if (!path.empty() && pathIndex < path.size()) {
        gridPosition = path[pathIndex];
    }
    updateScreenPosition();
}

void Ghost::updateScreenPosition() { 
    shape.setPosition({
        static_cast<float>(gridPosition.x * TILE_SIZE + TILE_SIZE / 2.0f), 
        static_cast<float>(gridPosition.y * TILE_SIZE + TILE_SIZE / 2.0f)
    }); 
}