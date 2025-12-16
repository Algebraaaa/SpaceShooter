#include "GameData.h"

// 游戏状态
GameState gameState = MENU;

// 玩家
Player player;

// 游戏对象容器
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Particle> particles;

// 输入状态
bool keys[256] = { false };
int mouseX = 0, mouseY = 0;

// 游戏计时
float spawnTimer = 0;
float gameTime = 0;
int wave = 1;