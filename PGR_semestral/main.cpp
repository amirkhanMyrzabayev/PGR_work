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
#include "Skybox.h"
#include "parametry.h"

float lastX = WIN_WIDTH / 2.0;
float lastY = WIN_WIDTH / 2.0;
bool firstMouse = true;
bool isLeftMousePressed = false;
bool isRightMousePressed = false;


GLuint skyboxShader = 0;

std::vector<DirectionalLight*> dirLights;
std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;


std::vector<Object*> sceneObjects;
Skybox* skybox = nullptr;


InputManager inputManager;
Camera camera(STATIC_CAMERAS[0].position);
MeshManager globalMeshManager;
ShaderManager globalShaderManager;

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
}

void specialKeyPressed(int key, int x, int y) {
    inputManager.pressSpecialKey(key);
}

void specialKeyRealesed(int key, int x, int y) {
    inputManager.releaseSpecialKey(key);
}

void mouseClickCallback(int button, int state, int xpos, int ypos) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            firstMouse = true;
            isRightMousePressed = true;
            camera.startTrackball((float)xpos, (float)ypos, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
        }
        else if (state == GLUT_UP) {
            isRightMousePressed = false;
        }
    }
    else if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            firstMouse = true;
            isLeftMousePressed = true;

        }
        else if (state == GLUT_UP) {
            isLeftMousePressed = false;
        }
    }
    lastX = xpos;
    lastY = ypos;
}

void mouseCallback(int xpos, int ypos) {
    if (!(isLeftMousePressed || isRightMousePressed)) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if (isLeftMousePressed) {
        float offset_x = xpos - lastX;
        float offset_y = lastY - ypos;
        camera.processMouseMovement(offset_x, offset_y);
    }
    else if (isRightMousePressed) camera.processTrackballMovement((float)xpos, (float)ypos, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
    lastX = xpos;
    lastY = ypos;
    glutPostRedisplay();
}

void timerFunc(int value) {
    float timeScale = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    for (auto obj : sceneObjects) {
        if (obj->isTextureAnimated) {
            glm::mat4 texMat = glm::mat4(1.0f);
            texMat = glm::translate(texMat, glm::vec3(timeScale * 0.2f, 0.0f, 0.0f));
            texMat = glm::rotate(texMat, timeScale * 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
            obj->setTextureMatrix(texMat);
        }
    }
    dirLights[0]->update(timeScale);
    camera.move(inputManager);
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    skyboxShader = globalShaderManager.getShaderProgram("Shaders/skybox");
    skybox = new Skybox(SKYBOX_FACES, skyboxShader);


    for (auto objInfo : SCENE_OBJECTS_SETUP) {
        sceneObjects.push_back(new Object(objInfo,globalShaderManager, globalMeshManager));
        sceneObjects.back()->isTextureAnimated = objInfo.isTexAnim;
    }
    int borderIndex = 0;
    ObjectSetup curObject;
    glm::vec3 rotation = glm::vec3(0.0f, -glm::radians(90.0f), 0.0f);
    for (float x = MIN_X; x < MAX_X; x += TILE_SIZE) {
        rotation.y = -rotation.y;
        for (float z = MIN_Z; z < MAX_Z; z += TILE_SIZE) {
            if (x == MIN_X || x == MAX_X-TILE_SIZE || z == MIN_Z || z == MAX_Z-TILE_SIZE) {
                curObject = { BORDER_OBJECTS_PATHS[borderIndex % BORDER_OBJECTS_PATHS.size()], 
                              mainLightShaderName, glm::vec3(x, 0.0f, z), glm::vec3(0.0f), glm::vec3(0.05f) };
                sceneObjects.push_back(new Object(curObject, globalShaderManager, globalMeshManager));
            }
            else {
                curObject = { tilePath, mainLightShaderName, glm::vec3(x, 0.0f, z), rotation, glm::vec3(15.0f)};
                sceneObjects.push_back(new Object(curObject, globalShaderManager, globalMeshManager));

            }
        }
    }


    //lights
    for (auto& setup : DIR_LIGHTS_SETUP) {
        if (dirLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum directional lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        dirLights.push_back(new DirectionalLight(setup));
    }

    for (auto& setup : POINT_LIGHTS_SETUP) {
        if (pointLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum point lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        pointLights.push_back(new PointLight(setup));
    }

    for (auto& setup : SPOT_LIGHTS_SETUP) {
        if (spotLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum spot lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        spotLights.push_back(new SpotLight(setup));
    }

}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();

    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    GLint mainLightShader = globalShaderManager.getShaderProgram(mainLightShaderName);
    glUseProgram(mainLightShader);
    int i = 0;
    for (i = 0; i < dirLights.size(); i++) {
        if (i == MAX_DIR_LIGHTS) {
            std::cerr << "Maximum directional lights " << MAX_DIR_LIGHTS << " exceeded: ignoring remaining" << std::endl;
            break;
        }
        dirLights[i]->bindUniforms(mainLightShader, i);
    }
    GLint numLightLoc = glGetUniformLocation(mainLightShader, "numDirLights");
    glUniform1i(numLightLoc, i);
    for (i = 0; i < pointLights.size(); i++) {
        if (i == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum point lights " << MAX_POINT_LIGHTS << " exceeded: ignoring remaining" << std::endl;
            break;
        }
        pointLights[i]->bindUniforms(mainLightShader, i);
    }
    numLightLoc = glGetUniformLocation(mainLightShader, "numPointLights");
    glUniform1i(numLightLoc, i);
    for (int i = 0; i < spotLights.size(); i++) {
        if (i == MAX_SPOT_LIGHTS) {
            std::cerr << "Maximum spot lights " << MAX_SPOT_LIGHTS << " exceeded: ignoring remaining" << std::endl;
            break;
        }
        spotLights[i]->bindUniforms(mainLightShader, i);
    }
    numLightLoc = glGetUniformLocation(mainLightShader, "numSpotLights");
    glUniform1i(numLightLoc, i);
    for (auto obj : sceneObjects) {
        obj->draw(view, proj, camera.getPosition());
    }
    skybox->draw(view, proj);
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

