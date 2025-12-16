#include "GameTypes.h"
#include "GameData.h"
#include "Renderer.h"
#include "GameLogic.h"
#include <ctime>
#include <cstdlib>

// 显示回调
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    if (gameState == MENU) {
        drawMenu();
    }
    else if (gameState == GAME_OVER) {
        drawGameOver();
    }
    else if (gameState == PLAYING || gameState == PAUSED) {
        drawPlayer();

        for (size_t i = 0; i < bullets.size(); i++) {
            drawBullet(bullets[i]);
        }

        for (size_t i = 0; i < enemies.size(); i++) {
            drawEnemy(enemies[i]);
        }

        drawParticles();
        drawUI();

        if (gameState == PAUSED) {
            drawPaused();
        }
    }

    glutSwapBuffers();
}

// 定时器回调
void timer(int value) {
    updateGame(0.016f);
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// 键盘按下
void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;

    if (key == 13) {
        if (gameState == MENU || gameState == GAME_OVER) {
            resetGame();
        }
    }

    if (key == 'p' || key == 'P') {
        if (gameState == PLAYING) {
            gameState = PAUSED;
        }
        else if (gameState == PAUSED) {
            gameState = PLAYING;
        }
    }

    if (key == 27) {
        exit(0);
    }
}

// 键盘抬起
void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

// 鼠标移动
void mouseMotion(int x, int y) {
    mouseX = x;
    mouseY = WINDOW_HEIGHT - y;
}

// 鼠标点击
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gameState == PLAYING) {
        if (player.shootCooldown <= 0) {
            bullets.push_back(Bullet(player.x, player.y + player.height / 2, 8.0f, true));
            player.shootCooldown = 0.2f;
        }
    }
}

// 窗口大小改变
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

// 主函数
int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Space Shooter - OpenGL Game");

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}