#include "Renderer.h"
#include "GameData.h"
#include <sstream>
#include <ctime>

// 文字绘制辅助函数
void drawText(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

void drawCenteredText(float y, const char* text, void* font) {
    int width = glutBitmapLength(font, (const unsigned char*)text);
    drawText((WINDOW_WIDTH - width) / 2.0f, y, text, font);
}

// 绘制飞船
void drawPlayer() {
    glPushMatrix();
    glTranslatef(player.x, player.y, 0);

    // 飞船主体
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, player.height / 2);
    glVertex2f(-player.width / 2, -player.height / 2);
    glVertex2f(player.width / 2, -player.height / 2);
    glEnd();

    // 飞船装饰
    glColor3f(0.8f, 0.9f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, player.height / 4);
    glVertex2f(-player.width / 4, -player.height / 4);
    glVertex2f(player.width / 4, -player.height / 4);
    glEnd();

    // 引擎火焰
    if (keys['w'] || keys['W']) {
        glColor3f(1.0f, 0.5f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-player.width / 4, -player.height / 2);
        glVertex2f(0, -player.height);
        glVertex2f(player.width / 4, -player.height / 2);
        glEnd();
    }

    glPopMatrix();
}

// 绘制敌人
void drawEnemy(const Enemy& enemy) {
    glPushMatrix();
    glTranslatef(enemy.x, enemy.y, 0);

    if (enemy.type == 0) {
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0, -20);
        glVertex2f(-15, 20);
        glVertex2f(15, 20);
        glEnd();
    }
    else if (enemy.type == 1) {
        glColor3f(1.0f, 0.8f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-10, -15);
        glVertex2f(-10, 15);
        glVertex2f(10, 15);
        glVertex2f(10, -15);
        glEnd();

        glColor3f(0.8f, 0.6f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-10, 0);
        glVertex2f(-25, -10);
        glVertex2f(-25, 10);
        glVertex2f(10, 0);
        glVertex2f(25, -10);
        glVertex2f(25, 10);
        glEnd();
    }
    else if (enemy.type == 2) {
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(-20, -20);
        glVertex2f(-20, 20);
        glVertex2f(20, 20);
        glVertex2f(20, -20);
        glEnd();

        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(-15, -15);
        glVertex2f(-15, 15);
        glVertex2f(15, 15);
        glVertex2f(15, -15);
        glEnd();

        float healthRatio = (float)enemy.health / 3.0f;
        glColor3f(1.0f - healthRatio, healthRatio, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-15, 20);
        glVertex2f(-15, 25);
        glVertex2f(-15 + 30 * healthRatio, 25);
        glVertex2f(-15 + 30 * healthRatio, 20);
        glEnd();
    }

    glPopMatrix();
}

// 绘制子弹
void drawBullet(const Bullet& bullet) {
    if (bullet.isPlayerBullet) {
        glColor3f(0.0f, 1.0f, 1.0f);
    }
    else {
        glColor3f(1.0f, 0.0f, 0.0f);
    }

    glBegin(GL_QUADS);
    glVertex2f(bullet.x - 2, bullet.y - 8);
    glVertex2f(bullet.x + 2, bullet.y - 8);
    glVertex2f(bullet.x + 2, bullet.y + 8);
    glVertex2f(bullet.x - 2, bullet.y + 8);
    glEnd();
}

// 绘制粒子
void drawParticles() {
    glBegin(GL_QUADS);
    for (size_t i = 0; i < particles.size(); i++) {
        Particle& p = particles[i];
        float size = 3.0f * p.life;
        glColor4f(p.r, p.g, p.b, p.life);
        glVertex2f(p.x - size, p.y - size);
        glVertex2f(p.x + size, p.y - size);
        glVertex2f(p.x + size, p.y + size);
        glVertex2f(p.x - size, p.y + size);
    }
    glEnd();
}

// 绘制星空背景
void drawBackground() {
    static std::vector<std::pair<float, float>> stars;
    static bool initialized = false;

    if (!initialized) {
        for (int i = 0; i < 100; i++) {
            stars.push_back(std::make_pair(
                (float)(rand() % WINDOW_WIDTH),
                (float)(rand() % WINDOW_HEIGHT)
            ));
        }
        initialized = true;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < stars.size(); i++) {
        glVertex2f(stars[i].first, stars[i].second);
    }
    glEnd();
}

// 绘制UI
void drawUI() {
    std::stringstream ss;

    glColor3f(0.0f, 1.0f, 0.0f);
    ss << "Health: " << player.health;
    drawText(10, WINDOW_HEIGHT - 20, ss.str().c_str());

    ss.str("");
    ss << "Score: " << player.score;
    drawText(10, WINDOW_HEIGHT - 40, ss.str().c_str());

    ss.str("");
    ss << "Wave: " << wave;
    drawText(10, WINDOW_HEIGHT - 60, ss.str().c_str());

    // 生命值条背景
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, WINDOW_HEIGHT - 80);
    glVertex2f(10, WINDOW_HEIGHT - 90);
    glVertex2f(210, WINDOW_HEIGHT - 90);
    glVertex2f(210, WINDOW_HEIGHT - 80);
    glEnd();

    // 生命值条
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, WINDOW_HEIGHT - 80);
    glVertex2f(10, WINDOW_HEIGHT - 90);
    glVertex2f(10 + 200.0f * player.health / 100.0f, WINDOW_HEIGHT - 90);
    glVertex2f(10 + 200.0f * player.health / 100.0f, WINDOW_HEIGHT - 80);
    glEnd();
}

// 绘制菜单
void drawMenu() {
    glColor3f(0.0f, 1.0f, 1.0f);
    drawCenteredText(400, "SPACE SHOOTER", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCenteredText(300, "Controls:", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(270, "WASD - Move", GLUT_BITMAP_HELVETICA_12);
    drawCenteredText(250, "SPACE - Shoot", GLUT_BITMAP_HELVETICA_12);
    drawCenteredText(230, "P - Pause", GLUT_BITMAP_HELVETICA_12);
    drawCenteredText(210, "Mouse - Aim", GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.0f, 1.0f, 0.0f);
    drawCenteredText(150, "Press ENTER to Start", GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawCenteredText(80, "Defeat waves of enemies!", GLUT_BITMAP_HELVETICA_12);
    drawCenteredText(60, "Survive as long as you can!", GLUT_BITMAP_HELVETICA_12);
}

// 绘制游戏结束画面
void drawGameOver() {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCenteredText(400, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

    std::stringstream ss;
    ss << "Final Score: " << player.score;
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCenteredText(300, ss.str().c_str(), GLUT_BITMAP_HELVETICA_18);

    ss.str("");
    ss << "Waves Survived: " << wave;
    drawCenteredText(260, ss.str().c_str(), GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.0f, 1.0f, 0.0f);
    drawCenteredText(180, "Press ENTER to Restart", GLUT_BITMAP_HELVETICA_18);
    drawCenteredText(150, "Press ESC to Exit", GLUT_BITMAP_HELVETICA_18);
}

// 绘制暂停画面
void drawPaused() {
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCenteredText(300, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCenteredText(250, "Press P to Resume", GLUT_BITMAP_HELVETICA_18);
}