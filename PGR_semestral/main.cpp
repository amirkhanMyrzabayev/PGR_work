/**
 * \file hello-world.cpp
 * \brief Your first OpenGL application.
 * \author Tomas Barak
 */

#include <iostream>
#include "pgr.h"
#include "InputManager.h"
#include "Camera.h"
#include "Object.h"
#include "parametry.h"

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 1024;
const char* WIN_TITLE = "PRG_Semestral";

float lastX = WIN_WIDTH / 2.0;
float lastY = WIN_WIDTH / 2.0;
bool firstMouse = true;
bool isLeftMousePressed = false;

GLuint shaderProgram = 0;
GLuint arrayBuffer = 0;
GLuint vao = 0;


std::vector<Object*> sceneObjects;


InputManager inputManager;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void keyPressed(unsigned char key, int x, int y) {
    inputManager.pressKey(key);
    if (inputManager.keys['1']) camera.setCameraState(freeCamera);
    else if (inputManager.keys['2']) camera.setCameraState(staticFirst);
    else if (inputManager.keys['3']) camera.setCameraState(staticSecond);
    //if (inputManager.keys[key]) std::cout << "key " << key << " is pressed" << std::endl;
}

void keyRealesed(unsigned char key, int x, int y) {
    inputManager.releaseKey(key);
    //if (inputManager.keys[key] == false) std::cout << "key " << key << " is realesed" << std::endl;
}

void specialKeyPressed(int key, int x, int y) {
    inputManager.pressSpecialKey(key);
}

void specialKeyRealesed(int key, int x, int y) {
    inputManager.releaseSpecialKey(key);
}

void mouseClickCallback(int button, int state, int xpos, int ypos) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            firstMouse = true;
            isLeftMousePressed = true;
        }
        else if (state == GLUT_UP) {
            isLeftMousePressed = false;
        }
    }
}

void mouseCallback(int xpos, int ypos) {
    if (!isLeftMousePressed) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float offset_x = xpos - lastX;
    float offset_y = ypos - lastY;
    camera.processMouseMovement(offset_x, offset_y);
    lastX = xpos;
    lastY = ypos;
    glutPostRedisplay();
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
      //pgr::createShaderFromFile(GL_VERTEX_SHADER, "Shaders/basic.vert"),
      //pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/basic.frag"),
      pgr::createShaderFromFile(GL_VERTEX_SHADER, "Shaders/3d_basic.vert"),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/3d_basic.frag"),
      0
    };
    shaderProgram = pgr::createProgram(shaders);

    for (auto objInfo : SCENE_OBJECTS_SETUP) {
        sceneObjects.push_back(new Object(objInfo.path, shaderProgram,
                                            objInfo.position, 
                                            objInfo.rotation, 
                                            objInfo.scale));
    }

    //myBarStand = new Object("Assets/cartoon_building/cartoon_building.txt", shaderProgram);
    //myBarStand->setSRP(glm::vec3(0.9f, -0.5f, -0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f));
    //myBarStand->setPosition(glm::vec3(0.0f));
    //myBarStand->setRotation(glm::vec3(glm::radians(-90.f), 0.0f, glm::radians(90.0f)));
    //myBarStand->setScale(glm::vec3(1.0f));
    //static const float vertices[] = {
    //  0.5f,  0.5f,
    //  -0.5f, -0.5f,
    //  0.5f, -0.5f,

    //   -0.5f,  0.5f,
    //   -0.5f, -0.5f,
    //  0.5f, 0.5f,
    //};

    //glGenBuffers(1, &arrayBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //GLint positionLoc = glGetAttribLocation(shaderProgram, "position");
    //glEnableVertexAttribArray(positionLoc);
    //glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);


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

    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    for (auto obj : sceneObjects) {
        obj->draw();
    }
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
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyRealesed);
    glutMotionFunc(mouseCallback);
    glutMouseFunc(mouseClickCallback);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("pgr init failed, required OpenGL not supported?");

    init();

    std::cout << "Hello triangle!" << std::endl;
    glutTimerFunc(33, timerFunc, 0);
    glutMainLoop();
    return 0;
}

