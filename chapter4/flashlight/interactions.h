//指定回调函数来控制flashlight应用程序的交互
//显示一个可交互式改变的图像。每个图像元素对应该位置到参考点的距离，这个参考点可以被交互式地移动。
#pragma once
#ifndef INTERACTIONS_H
#define INTERACTIONS_H
#define W 600//图像维度
#define H 600
#define DELTA 5//每按下箭头键参考点移动的距离（以像素为单位）
#define  TITLE_STRING "flashlight:distance image display app"//标题栏中显示的文本
int2 loc = { W / 2, H / 2 }; //参考点的位置，图像的中心
bool dragMode = false;

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        dragMode = !dragMode;
    }

    if (key == 27) {
        exit(0);
    }

    glutPostRedisplay();//在每个回调函数的结尾被调用，根据交互输入计算出一个新的图像用于显示（调用main.cpp中的display()）
}

void mouseMove(int x, int y) {
    if (dragMode) {
        return;
    }

    loc.x = x;
    loc.y = y;
    glutPostRedisplay();
}

void mouseDrag(int x, int y) {
    if (!dragMode) {
        return;
    }

    loc.x = x;
    loc.y = y;
    glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        loc.x -= DELTA;
    }

    if (key == GLUT_KEY_RIGHT) {
        loc.x += DELTA;
    }

    if (key == GLUT_KEY_UP) {
        loc.y -= DELTA;
    }

    if (key == GLUT_KEY_DOWN) {
        loc.y += DELTA;
    }

    glutPostRedisplay();
}

void printInstructions() {
    printf("flashlight interactions\n");
    printf("a: toggle mouse tracking mode\n");
    printf("arrow keys: move ref location\n");
    printf("esc: close graphics window\n");
}
#endif // !INTERACTIONS_H
