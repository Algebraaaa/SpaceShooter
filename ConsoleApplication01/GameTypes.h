#pragma once

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <vector>

// 窗口尺寸
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 游戏状态
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };

// 玩家飞船
struct Player {
    float x, y;
    float width, height;
    int health;
    int score;
    float shootCooldown;

    Player() : x(400), y(50), width(40), height(50), health(100), score(0), shootCooldown(0) {}
};

// 子弹
struct Bullet {
    float x, y;
    float speed;
    bool active;
    bool isPlayerBullet;

    Bullet(float px, float py, float s, bool player)
        : x(px), y(py), speed(s), active(true), isPlayerBullet(player) {
    }
};

// 敌人
struct Enemy {
    float x, y;
    float speedX, speedY;
    int health;
    bool active;
    int type; // 0: 普通, 1: 快速, 2: 坦克
    float shootTimer;

    Enemy(float px, float py, int t)
        : x(px), y(py), speedX(0), speedY(1), health(1), active(true), type(t), shootTimer(0) {
        if (type == 0) {
            health = 1;
            speedY = 1.5f;
        }
        else if (type == 1) {
            health = 1;
            speedY = 3.0f;
            speedX = (rand() % 3 - 1) * 2.0f;
        }
        else if (type == 2) {
            health = 3;
            speedY = 0.8f;
        }
    }
};

// 粒子效果
struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float r, g, b;

    Particle(float px, float py) : x(px), y(py), life(1.0f) {
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 2.0f + (rand() % 100) / 50.0f;
        vx = cos(angle) * speed;
        vy = sin(angle) * speed;
        r = 0.8f + (rand() % 20) / 100.0f;
        g = 0.3f + (rand() % 30) / 100.0f;
        b = 0.1f;
    }
};