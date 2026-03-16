/**
 * \file hello-world.cpp
 * \brief Your first OpenGL application.
 * \author Tomas Barak
 */

#include <iostream>
#include "pgr.h"
#include "InputManager.h"
#include "Camera.h"

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 1024;
const char* WIN_TITLE = "Hello World";

GLuint shaderProgram = 0;
GLuint arrayBuffer = 0;
GLuint vao = 0;



InputManager inputManager;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void keyPressed(unsigned char key, int x, int y) {
    inputManager.pressKey(key);
    if (inputManager.keys[key]) std::cout << "key " << key << " is pressed" << std::endl;
}

void keyRealesed(unsigned char key, int x, int y) {
    inputManager.releaseKey(key);
    if (inputManager.keys[key] == false) std::cout << "key " << key << " is realesed" << std::endl;
}

void timerFunc(int value) {
    camera.move(inputManager);
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 0);
}

void init() {
    glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    GLuint shaders[] = {
      pgr::createShaderFromFile(GL_VERTEX_SHADER, "Shaders/basic.vert"),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/basic.frag"),
      0
    };
    shaderProgram = pgr::createProgram(shaders);

    static const float vertices[] = {
      0.5f,  0.5f,
      -0.5f, -0.5f,
      0.5f, -0.5f,

       -0.5f,  0.5f,
       -0.5f, -0.5f,
      0.5f, 0.5f,
    };

    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint positionLoc = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glm::mat4 proj = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();

    GLint projLocation = glGetUniformLocation(shaderProgram, "projection");
    GLint viewLocation = glGetUniformLocation(shaderProgram, "view");

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow(WIN_TITLE);

    glutDisplayFunc(draw);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyRealesed);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("pgr init failed, required OpenGL not supported?");

    init();

    std::cout << "Hello triangle!" << std::endl;
    glutTimerFunc(33, timerFunc, 0);
    glutMainLoop();
    return 0;
}

