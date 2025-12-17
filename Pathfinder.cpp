#include "Pathfinder.hpp"
#include <queue>
#include <map>
#include <algorithm>

struct Node {
    int y, x;
    int parent_y, parent_x;
    float gCost;
    float hCost;
    float fCost() const { return gCost + hCost; }
    bool operator>(const Node& other) const { return fCost() > other.fCost(); }
};

float Pathfinder::calculateHeuristic(int x1, int y1, int x2, int y2) {
    return static_cast<float>(std::abs(x1 - x2) + std::abs(y1 - y2));
}

std::vector<sf::Vector2i> Pathfinder::findPath(const std::vector<std::string>& map, sf::Vector2i start, sf::Vector2i end) {
    std::vector<sf::Vector2i> path;
    if (start == end) return path;
    int mapHeight = static_cast<int>(map.size());
    int mapWidth = static_cast<int>(map[0].size());
    
    if (start.x < 0 || start.x >= mapWidth || start.y < 0 || start.y >= mapHeight ||
        end.x < 0 || end.x >= mapWidth || end.y < 0 || end.y >= mapHeight) {
        return path;
    }

    Node startNode = {start.y, start.x, -1, -1, 0.0f, calculateHeuristic(start.x, start.y, end.x, end.y)};
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    openList.push(startNode);
    std::map<int, Node> allNodes;
    allNodes[start.y * mapWidth + start.x] = startNode;
    std::vector<bool> closedList(mapWidth * mapHeight, false);
    int dy[] = {0, 0, 1, -1};
    int dx[] = {1, -1, 0, 0};

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();
        int currentIdx = currentNode.y * mapWidth + currentNode.x;
        
        if (closedList[currentIdx]) continue;
        closedList[currentIdx] = true;

        if (currentNode.y == end.y && currentNode.x == end.x) {
            Node temp = currentNode;
            while (temp.parent_y != -1) {
                path.push_back({temp.x, temp.y});
                temp = allNodes[temp.parent_y * mapWidth + temp.parent_x];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            int newY = currentNode.y + dy[i];
            int newX = currentNode.x + dx[i];
            int newIdx = newY * mapWidth + newX;
            if (newY < 0 || newY >= mapHeight || newX < 0 || newX >= mapWidth || map[newY][newX] == '#' || closedList[newIdx]) continue;
            float gCostNew = currentNode.gCost + 1.0f;
            float hCostNew = calculateHeuristic(newX, newY, end.x, end.y);
            if (allNodes.find(newIdx) == allNodes.end() || gCostNew < allNodes[newIdx].gCost) {
                Node neighborNode = {newY, newX, currentNode.y, currentNode.x, gCostNew, hCostNew};
                allNodes[newIdx] = neighborNode;
                openList.push(neighborNode);
            }
        }
    }
    return path;
}