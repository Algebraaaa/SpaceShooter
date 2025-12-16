#pragma once

// 碰撞检测
bool checkCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2);

// 爆炸效果
void createExplosion(float x, float y, int count = 20);

// 游戏更新
void updateGame(float deltaTime);

// 重置游戏
void resetGame();