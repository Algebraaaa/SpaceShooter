#pragma once

#include "GameTypes.h"

// 文字绘制
void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18);
void drawCenteredText(float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18);

// 游戏对象绘制
void drawPlayer();
void drawEnemy(const Enemy& enemy);
void drawBullet(const Bullet& bullet);
void drawParticles();
void drawBackground();

// UI 绘制
void drawUI();
void drawMenu();
void drawGameOver();
void drawPaused();