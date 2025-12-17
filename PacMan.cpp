#include "PacMan.hpp"
#include <cmath>

PacMan::PacMan(int x, int y) {
    gridPosition = {x, y};
    nextDirection = {1, 0};
    currentDirection = {1, 0};
    shape.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    
    // --- 初始化動畫狀態 ---
    currentMouthHalfAngle = 0.0f; // 一開始閉嘴
    isMouthOpening = true;        // 準備張開
    // --------------------
    
    updateGeometry();
}

void PacMan::setDirection(sf::Vector2i dir) { 
    nextDirection = dir; 
}

void PacMan::update(const std::vector<std::string>& map) {
    // (這部分的移動邏輯保持不變，不用改)
    int mapH = static_cast<int>(map.size());
    int mapW = static_cast<int>(map[0].size());

    sf::Vector2i checkNext = gridPosition + nextDirection;
    bool canTurn = false;
    if (checkNext.x >= 0 && checkNext.x < mapW && checkNext.y >= 0 && checkNext.y < mapH && map[checkNext.y][checkNext.x] != '#') {
        canTurn = true;
    }

    if (canTurn) {
        currentDirection = nextDirection;
    }

    sf::Vector2i nextPos = gridPosition + currentDirection;
    if (nextPos.x >= 0 && nextPos.x < mapW && nextPos.y >= 0 && nextPos.y < mapH && map[nextPos.y][nextPos.x] != '#') {
        gridPosition = nextPos;
    }

    if (currentDirection.x == 1) currentAngle = 0.0f;
    else if (currentDirection.x == -1) currentAngle = 180.0f;
    else if (currentDirection.y == 1) currentAngle = 90.0f;
    else if (currentDirection.y == -1) currentAngle = 270.0f;

    updateGeometry();
}

// --- 修改：平滑動畫邏輯 ---
void PacMan::updateAnimation() {
    // 設定變化的速度，這裡設為 1.0 表示每一幀變 1 度
    // 你的 FPS 是 30，所以從 0 到 35 度大約需要 1秒多一點 (35幀)
    float speed = 6.0f; 

    if (isMouthOpening) {
        currentMouthHalfAngle += speed;
        if (currentMouthHalfAngle >= 40.0f) {
            currentMouthHalfAngle = 40.0f; // 限制最大值
            isMouthOpening = false;        // 開始閉合
        }
    } else {
        currentMouthHalfAngle -= speed;
        if (currentMouthHalfAngle <= 0.0f) {
            currentMouthHalfAngle = 0.0f;  // 限制最小值
            isMouthOpening = true;         // 開始張開
        }
    }

    // 更新圖形 (讓下一幀繪製時生效)
    updateGeometry();
}

void PacMan::updateGeometry() {
    shape.clear();
    float centerX = gridPosition.x * TILE_SIZE + TILE_SIZE / 2.0f;
    float centerY = gridPosition.y * TILE_SIZE + TILE_SIZE / 2.0f;
    float radius = TILE_SIZE / 2.0f - 2;

    shape.append(sf::Vertex{sf::Vector2f(centerX, centerY), currentColor});

    // --- 修改：直接使用當前的動態角度 ---
    float mouthHalfAngle = currentMouthHalfAngle;
    // ---------------------------------

    int points = 30;

    for (int i = 0; i <= points; ++i) {
        float angleDeg = currentAngle + mouthHalfAngle + (360.0f - 2.0f * mouthHalfAngle) * (static_cast<float>(i) / points);
        float angleRad = angleDeg * PI / 180.0f;
        float px = centerX + radius * std::cos(angleRad);
        float py = centerY + radius * std::sin(angleRad);
        shape.append(sf::Vertex{sf::Vector2f(px, py), currentColor});
    }
}

void PacMan::reset(sf::Vector2i startPos) {
    gridPosition = startPos;
    nextDirection = {1, 0};
    currentDirection = {1, 0};
    currentColor = sf::Color::Yellow;

    // --- 重置動畫狀態 ---
    currentMouthHalfAngle = 0.0f;
    isMouthOpening = true;
    // ------------------

    updateGeometry();
}