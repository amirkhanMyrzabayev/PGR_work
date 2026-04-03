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

glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 5.0f);


std::vector<Object*> sceneObjects;


InputManager inputManager;
Camera camera(STATIC_CAMERAS[0].position);
MeshManager globalMeshManager;

void keyPressed(unsigned char key, int x, int y) {
    inputManager.pressKey(key);
    if (inputManager.keys['1']) {
        camera.setCameraState(freeCamera);
        firstMouse = true;
    }
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    GLuint shaders[] = {
      //pgr::createShaderFromFile(GL_VERTEX_SHADER, "Shaders/basic.vert"),
      //pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/basic.frag"),
      pgr::createShaderFromFile(GL_VERTEX_SHADER, "Shaders/3d_light.vert"),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/3d_light.frag"),
      0
    };
    shaderProgram = pgr::createProgram(shaders);

    for (auto objInfo : SCENE_OBJECTS_SETUP) {
        sceneObjects.push_back(new Object(objInfo.path, shaderProgram, 
                                            globalMeshManager,
                                            objInfo.position, 
                                            objInfo.rotation, 
                                            objInfo.scale));
    }

}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glm::mat4 proj = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();

    GLint projLocation = glGetUniformLocation(shaderProgram, "projection");
    GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
    GLint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    GLint cameraPosLocation = glGetUniformLocation(shaderProgram, "viewPos");

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
    glUniform3fv(cameraPosLocation, 1, glm::value_ptr(camera.getPosition()));

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

