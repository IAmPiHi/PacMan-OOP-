#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <optional>

#include "Constants.hpp"
#include "PacMan.hpp"
#include "Ghost.hpp"
#include "Pathfinder.hpp" // Ghost 內部用到，這裡不一定需要直接引用，但為了保險起見

enum GameState { GAME_START, PLAYING, GAME_OVER, GAME_WIN };

// 初始地圖
const std::vector<std::string> initialMap = {
    "#####################",
    "#.........#.........#",
    "#.###.###.#.###.###.#",
    "#...................#", 
    "#.###.#.#####.#.###.#",
    "#.....#...#...#.....#",
    "#####.### # ###.#####",
    "    #.#   G   #.#    ", 
    "#####.# ##### #.#####",
    "      #       #      ",
    "#####.# ##### #.#####",
    "    #.# P     #.#    ",
    "#####.# ##### #.#####",
    "#...................#", 
    "#.###.###.#.###.###.#",
    "#...#.....P.....#...#",
    "###.#.#.#####.#.#.###",
    "#.....#...#...#.....#",
    "#.#######.#.#######.#",
    "#...................#",
    "#####################"
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::vector<std::string> map = initialMap;
    int mapWidth = static_cast<int>(map[0].size());
    int mapHeight = static_cast<int>(map.size());

    // --- 預先計算合法的生成點 ---
    std::vector<sf::Vector2i> validSpawnPoints;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (initialMap[y][x] == '.') {
                validSpawnPoints.push_back({x, y});
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(mapWidth * TILE_SIZE), static_cast<unsigned int>(mapHeight * TILE_SIZE)}), "Pac-Man Final");
    window.setFramerateLimit(FPS);

    sf::Font font;
    // arial.ttf為字形檔案
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Warning: arial.ttf not found." << std::endl;
    }

    auto createCenteredText = [&](const std::string& str, sf::Color color, float yOffset) {
        sf::Text text(font);
        text.setString(str);
        text.setCharacterSize(40);
        text.setFillColor(color);
        sf::FloatRect rect = text.getLocalBounds();
        text.setOrigin({rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f});
        text.setPosition({window.getSize().x / 2.0f, window.getSize().y / 2.0f + yOffset});
        return text;
    };

    sf::Text winText = createCenteredText("YOU WIN!", sf::Color::Green, -50);
    sf::Text loseText = createCenteredText("GAME OVER!", sf::Color::Red, -50);
    sf::Text restartText = createCenteredText("Press 'R' to Restart", sf::Color::White, 20);
    sf::Text startText = createCenteredText("Press 'R' to Start", sf::Color::Yellow, 0);

    sf::Text scoreText(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({static_cast<float>(mapWidth * TILE_SIZE - 150), 10.f}); 

    sf::Vector2i pacmanStart;
    sf::Vector2i ghostSpawnPoint;

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (initialMap[y][x] == 'P') pacmanStart = {x, y};
            if (initialMap[y][x] == 'G') ghostSpawnPoint = {x, y};
        }
    }

    PacMan pacman(pacmanStart.x, pacmanStart.y);
    std::vector<Ghost> ghosts;

    GameState gameState = PLAYING;
    int score = 0;

    bool isPoweredUp = false;
    int powerTimer = 0;
    const int POWER_DURATION = 150; 
    int pacmanMoveTimer = 0; 

    const int STAR_RESPAWN_TIME = 360; 
    int starRespawnTimer = STAR_RESPAWN_TIME; 

    bool isFrozen = false;       
    int freezeTimer = 0;         
    const int FREEZE_DURATION = 90;

    auto resetGame = [&]() {
        map = initialMap;
        pacman.reset(pacmanStart);
        ghosts.clear();
        ghosts.emplace_back(ghostSpawnPoint.x - 1, ghostSpawnPoint.y, sf::Color::Red, 12);
        ghosts.emplace_back(ghostSpawnPoint.x, ghostSpawnPoint.y, sf::Color::Cyan, 16); 
        ghosts.emplace_back(ghostSpawnPoint.x + 1, ghostSpawnPoint.y, sf::Color::Magenta, 14);
        
        gameState = PLAYING;
        isPoweredUp = false;
        powerTimer = 0;
        starRespawnTimer = STAR_RESPAWN_TIME;
        isFrozen = false;
        freezeTimer = 0;
        score = 0;
        scoreText.setString("Score: 0");
    };

    resetGame();
    gameState = GAME_START;

    while (window.isOpen()) {
        // SFML 3.0 事件處理
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (gameState == PLAYING) {
                    if (keyPressed->code == sf::Keyboard::Key::W) pacman.setDirection({0, -1});
                    else if (keyPressed->code == sf::Keyboard::Key::S) pacman.setDirection({0, 1});
                    else if (keyPressed->code == sf::Keyboard::Key::A) pacman.setDirection({-1, 0});
                    else if (keyPressed->code == sf::Keyboard::Key::D) pacman.setDirection({1, 0});
                } 
                else if (gameState == GAME_START) {
                    if (keyPressed->code == sf::Keyboard::Key::R) {
                        gameState = PLAYING;
                    }
                }
                else {
                    if (keyPressed->code == sf::Keyboard::Key::R) {
                        resetGame();
                    }
                }
            }
        }

        if (gameState == PLAYING) {
            pacman.updateAnimation();
            // 1. 設定 PacMan 顏色
            if (isFrozen) {
                pacman.currentColor = sf::Color::Cyan;
            } else if (isPoweredUp) {
                if ((powerTimer % 4) < 2) pacman.currentColor = sf::Color::Red;
                else pacman.currentColor = sf::Color::Blue;
            } else {
                pacman.currentColor = sf::Color::Yellow;
            }

            // 2. 移動 PacMan
            sf::Vector2i pacmanPrevPos = pacman.gridPosition; 

            if (isFrozen) {
                pacman.updateGeometry();
                freezeTimer--;
                if (freezeTimer <= 0) isFrozen = false;
            } else {
                pacmanMoveTimer++;
                if (pacmanMoveTimer >= 3) {
                    pacman.update(map);
                    pacmanMoveTimer = 0;
                }
            }

            // 3. 無敵星星倒數
            if (isPoweredUp) {
                powerTimer--;
                if (powerTimer <= 0) isPoweredUp = false;
            }

            // 4. 吃東西與計分
            if (!isFrozen) {
                char& cell = map[pacman.gridPosition.y][pacman.gridPosition.x];
                if (cell == '.') {
                    cell = ' '; 
                    score += 1; 
                    scoreText.setString("Score: " + std::to_string(score));
                } else if (cell == '*') {
                    cell = ' ';
                    isPoweredUp = true;
                    powerTimer = POWER_DURATION;
                    score += 5; 
                    scoreText.setString("Score: " + std::to_string(score));
                    starRespawnTimer = STAR_RESPAWN_TIME;
                }
            }

            // 5. 鬼魂邏輯
            for (auto it = ghosts.begin(); it != ghosts.end(); ) {
                sf::Vector2i ghostPrevPos = it->gridPosition;
                it->update(map, pacman, isPoweredUp);

                // 鬼吃星星
                if (map[it->gridPosition.y][it->gridPosition.x] == '*') {
                    map[it->gridPosition.y][it->gridPosition.x] = ' '; 
                    isFrozen = true;
                    freezeTimer = FREEZE_DURATION;
                    starRespawnTimer = STAR_RESPAWN_TIME;
                }

                // 碰撞檢測
                bool collision = false;
                if (pacman.gridPosition == it->gridPosition) {
                    collision = true;
                } else if (pacman.gridPosition == ghostPrevPos && pacmanPrevPos == it->gridPosition) {
                    collision = true;
                }

                if (collision) {
                    if (isPoweredUp) {
                        it = ghosts.erase(it); 
                        score += 50; 
                        scoreText.setString("Score: " + std::to_string(score));

                        // 鬼魂暴怒機制
                        int newSpeed = 12; 
                        if (ghosts.size() == 2) {
                            newSpeed = 6; 
                        } else if (ghosts.size() == 1) {
                            newSpeed = 4; 
                        }

                        if (ghosts.size() < 3) {
                            for (auto& survivingGhost : ghosts) {
                                survivingGhost.moveDelay = newSpeed;
                            }
                        }
                        continue; 
                    } else {
                        gameState = GAME_OVER;
                        break; 
                    }
                }
                ++it;
            }

            // 6. 星星重生
            bool hasStarOnMap = false;
            for (int y = 0; y < mapHeight; ++y) {
                for (int x = 0; x < mapWidth; ++x) {
                    if (map[y][x] == '*') {
                        hasStarOnMap = true;
                        break;
                    }
                }
            }

            if (!hasStarOnMap) {
                starRespawnTimer--;
                if (starRespawnTimer <= 0) {
                    if (!validSpawnPoints.empty()) {
                        int randIdx = std::rand() % validSpawnPoints.size();
                        sf::Vector2i spot = validSpawnPoints[randIdx];
                        if (map[spot.y][spot.x] != '#') {
                            map[spot.y][spot.x] = '*';
                        }
                        starRespawnTimer = STAR_RESPAWN_TIME; 
                    }
                }
            } else {
                starRespawnTimer = STAR_RESPAWN_TIME;
            }

            if (gameState == PLAYING && ghosts.empty()) {
                gameState = GAME_WIN;
            }
        }

        window.clear(sf::Color::Black);

        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                float px = static_cast<float>(x * TILE_SIZE);
                float py = static_cast<float>(y * TILE_SIZE);
                float centerX = px + TILE_SIZE / 2.0f;
                float centerY = py + TILE_SIZE / 2.0f;

                if (map[y][x] == '#') {
                    sf::RectangleShape wall({static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)});
                    wall.setPosition({px, py});
                    wall.setFillColor(sf::Color::Blue);
                    window.draw(wall);
                } else if (map[y][x] == '.') {
                    sf::CircleShape dot(3);
                    dot.setFillColor(sf::Color(255, 255, 150));
                    dot.setOrigin({dot.getRadius(), dot.getRadius()});
                    dot.setPosition({centerX, centerY});
                    window.draw(dot);
                } else if (map[y][x] == '*') {
                    sf::CircleShape star(6);
                    star.setFillColor(sf::Color::White); 
                    star.setOrigin({star.getRadius(), star.getRadius()});
                    star.setPosition({centerX, centerY});
                    window.draw(star);
                }
            }
        }

        for (const auto& ghost : ghosts) {
            window.draw(ghost.shape);
        }

        window.draw(pacman.shape);
        window.draw(scoreText);

        if (gameState == GAME_OVER) {
            window.draw(loseText);
            window.draw(restartText);
        } else if (gameState == GAME_WIN) {
            window.draw(winText);
            window.draw(restartText);
        } else if (gameState == GAME_START) {
            window.draw(startText);
        }

        window.display();
    }

    return 0;

}
