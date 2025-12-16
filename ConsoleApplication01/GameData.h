#pragma once

#include "GameTypes.h"
#include <vector>

// 游戏状态
extern GameState gameState;

// 玩家
extern Player player;

// 游戏对象容器
extern std::vector<Bullet> bullets;
extern std::vector<Enemy> enemies;
extern std::vector<Particle> particles;

// 输入状态
extern bool keys[256];
extern int mouseX, mouseY;

// 游戏计时
extern float spawnTimer;
extern float gameTime;
extern int wave;