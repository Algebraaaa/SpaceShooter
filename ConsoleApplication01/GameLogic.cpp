#include "GameLogic.h"
#include "GameData.h"
#include <algorithm>

// 碰撞检测
bool checkCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 &&
            y1 < y2 + h2 && y1 + h1 > y2);
}

// 创建爆炸效果
void createExplosion(float x, float y, int count) {
    for (int i = 0; i < count; i++) {
        particles.push_back(Particle(x, y));
    }
}

// 更新游戏逻辑
void updateGame(float deltaTime) {
    if (gameState != PLAYING) return;

    gameTime += deltaTime;

    // 更新玩家位置
    if (keys['w'] || keys['W']) player.y += 5.0f;
    if (keys['s'] || keys['S']) player.y -= 5.0f;
    if (keys['a'] || keys['A']) player.x -= 5.0f;
    if (keys['d'] || keys['D']) player.x += 5.0f;

    // 限制玩家在屏幕内
    if (player.x < player.width / 2) player.x = player.width / 2;
    if (player.x > WINDOW_WIDTH - player.width / 2) player.x = WINDOW_WIDTH - player.width / 2;
    if (player.y < player.height / 2) player.y = player.height / 2;
    if (player.y > WINDOW_HEIGHT - player.height / 2) player.y = WINDOW_HEIGHT - player.height / 2;

    // 更新射击冷却
    player.shootCooldown -= deltaTime;

    // 射击
    if (keys[' '] && player.shootCooldown <= 0) {
        bullets.push_back(Bullet(player.x, player.y + player.height / 2, 8.0f, true));
        player.shootCooldown = 0.2f;
    }

    // 更新子弹
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].isPlayerBullet) {
            bullets[i].y += bullets[i].speed;
        }
        else {
            bullets[i].y -= bullets[i].speed;
        }

        if (bullets[i].y < 0 || bullets[i].y > WINDOW_HEIGHT) {
            bullets[i].active = false;
        }
    }

    // 生成敌人
    spawnTimer += deltaTime;
    float spawnInterval = 2.0f - wave * 0.1f;
    if (spawnInterval < 0.5f) spawnInterval = 0.5f;

    if (spawnTimer > spawnInterval) {
        spawnTimer = 0;
        int type = rand() % 100;
        if (type < 60) type = 0;
        else if (type < 85) type = 1;
        else type = 2;

        enemies.push_back(Enemy((float)(rand() % (WINDOW_WIDTH - 60) + 30), 
                                (float)(WINDOW_HEIGHT + 30), type));
    }

    // 更新敌人
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i].x += enemies[i].speedX;
        enemies[i].y -= enemies[i].speedY;

        enemies[i].shootTimer += deltaTime;
        if (enemies[i].shootTimer > 1.5f && enemies[i].y < WINDOW_HEIGHT - 100) {
            bullets.push_back(Bullet(enemies[i].x, enemies[i].y - 20, 4.0f, false));
            enemies[i].shootTimer = 0;
        }

        if (enemies[i].x < 30 || enemies[i].x > WINDOW_WIDTH - 30) {
            enemies[i].speedX = -enemies[i].speedX;
        }

        if (enemies[i].y < -30) {
            enemies[i].active = false;
        }

        if (checkCollision(player.x - player.width / 2, player.y - player.height / 2,
                          player.width, player.height,
                          enemies[i].x - 20, enemies[i].y - 20, 40, 40)) {
            player.health -= 10;
            enemies[i].active = false;
            createExplosion(enemies[i].x, enemies[i].y);
        }
    }

    // 子弹碰撞检测
    for (size_t i = 0; i < bullets.size(); i++) {
        if (!bullets[i].isPlayerBullet) {
            if (checkCollision(player.x - player.width / 2, player.y - player.height / 2,
                              player.width, player.height,
                              bullets[i].x - 2, bullets[i].y - 8, 4, 16)) {
                player.health -= 5;
                bullets[i].active = false;
                createExplosion(bullets[i].x, bullets[i].y, 5);
            }
        }
        else {
            for (size_t j = 0; j < enemies.size(); j++) {
                if (enemies[j].active && checkCollision(
                    enemies[j].x - 20, enemies[j].y - 20, 40, 40,
                    bullets[i].x - 2, bullets[i].y - 8, 4, 16)) {

                    enemies[j].health--;
                    bullets[i].active = false;

                    if (enemies[j].health <= 0) {
                        enemies[j].active = false;
                        player.score += (enemies[j].type + 1) * 10;
                        createExplosion(enemies[j].x, enemies[j].y);
                    }
                    break;
                }
            }
        }
    }

    // 更新粒子
    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        particles[i].life -= deltaTime * 2.0f;
    }

    // 移除失效对象
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return !b.active; }), bullets.end());
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return !e.active; }), enemies.end());
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.life <= 0; }), particles.end());

    // 检查波数
    if (enemies.empty() && spawnTimer > 3.0f) {
        wave++;
        spawnTimer = 0;
    }

    // 游戏结束
    if (player.health <= 0) {
        gameState = GAME_OVER;
    }
}

// 重置游戏
void resetGame() {
    player = Player();
    bullets.clear();
    enemies.clear();
    particles.clear();
    gameTime = 0;
    spawnTimer = 0;
    wave = 1;
    gameState = PLAYING;
}