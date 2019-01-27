#include <stdio.h>//加载合适的头文件
#include <stdlib.h>
#include "kernel.h"
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif // _WIN32
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif // __APPLE__
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include "interactions.h"

GLuint pbo = 0;
GLuint tex = 0;
struct cudaGraphicsResource* cuda_pbo_resource;

void render() {
    uchar4* d_out = 0;
    cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
    cudaGraphicsResourceGetMappedPointer((void**)&d_out, NULL, cuda_pbo_resource);
    kernelLauncher(d_out, W, H, loc);
    cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);
}

void drawTexture() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);//建立二维纹理图像，创建四边形图像
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, H);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(W, H);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(W, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void display() { //窗口中显示的内容
    render();//计算新像素值
    drawTexture();//画OPENGL纹理
    glutSwapBuffers();//交换显示缓冲区。
    //双缓冲是一种用来提高图形程序效率的常见技术。
    //一个缓冲区提供可被读取用于显示的内存，与此同时，另一个缓冲区提供一段内存，保证下一帧的内容能够被写入。
    //在图形序列的帧与帧之间，两个缓冲区交换它们的读/写角色。
}

void initGLUT(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(W, H);
    glutCreateWindow(TITLE_STRING);
#ifndef __APPLE__
    glewInit();
#endif // !__APPLE__

}

void initPixelBuffer() { //初始化像素缓冲区
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, 4 * W * H * sizeof(GLubyte), 0, GL_STREAM_DRAW);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsMapFlagsWriteDiscard);
    //CUDA注册OPENGL缓冲区
    //如果映射成功，则将缓冲区内存的控制交给CUDA来进行写输出。
    //如果没有映射成功，则返回缓冲区内存的控制给OPENGL用于显示。
}

void exitfunc() {
    if (pbo) {
        cudaGraphicsUnregisterResource(cuda_pbo_resource);
        glDeleteBuffers(1, &pbo);
        glDeleteTextures(1, &tex);
    }
}

int main(int argc, char** argv) {
    printInstructions();//将一些用户指令打印到命令窗口
    initGLUT(&argc, argv);//初始化GLUT库，并且设置图像窗口的规格，包括显示模式（RGBA),缓冲区（double型），尺寸（W*H）和标题。
    gluOrtho2D(0, W, H, 0);//建立视觉变换（简单的投影）。
    glutKeyboardFunc(keyboard);//键盘和鼠标的交互
    glutSpecialFunc(handleSpecialKeypress);
    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseDrag);
    glutDisplayFunc(display);
    initPixelBuffer();
    glutMainLoop();//
    atexit(exitfunc);//执行清理工作，删除OPENGL的像素缓冲和纹理。
    return 0;
}
