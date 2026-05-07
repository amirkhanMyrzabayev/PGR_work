/**
 * \file hello-world.cpp
 * \brief Your first OpenGL application.
 * \author Tomas Barak
 */

#include <iostream>
#include <map>
#include "pgr.h"
#include "InputManager.h"
#include "Camera.h"
#include "AnimatedObject.h"
#include "Skybox.h"
#include "parametry.h"

float lastX = WIN_WIDTH / 2.0;
float lastY = WIN_WIDTH / 2.0;
bool firstMouse = true;
bool isLeftMousePressed = false;
float lastFrameTime = 0.0f;


GLuint skyboxShader = 0;

FogPositions fogPositions;
GLint mainLightShader;

std::vector<std::unique_ptr<DirectionalLight>> dirLights;
std::vector<std::unique_ptr<PointLight>> pointLights;
std::vector<std::unique_ptr<SpotLight>> spotLights;


std::vector<std::unique_ptr<Object>> sceneObjects;
std::vector<std::unique_ptr<AnimatedObject>> animatedObjects;
std::vector<std::unique_ptr<SpriteObject>> spriteObjects;
std::vector<std::unique_ptr<Object>> transparentObjects;
std::unique_ptr<Skybox> skybox;
std::vector<std::pair<glm::vec3, float>> collisionCircles;


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

Object* getObjectById(int id) {
    if (id == 0) return nullptr;
    for (auto& obj : sceneObjects) {
        if (obj->getId() == id) return obj.get();
    }
    for (auto& animObj : animatedObjects) {
        if (animObj->getId() == id) return animObj.get();
    }
    return nullptr;
}

void handlePicking(int id) {
    Object* clickedObj = getObjectById(id);
    if (clickedObj == nullptr) return;
    
    AnimatedObject* animObj = dynamic_cast<AnimatedObject*>(clickedObj);
    if (animObj == nullptr) {
        if (clickedObj->hasLight()) {
            clickedObj->switchLight();
        }
        
    }
    else {
        animObj->switchAnimation();
    }
}

void mouseClickCallback(int button, int state, int xpos, int ypos) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            firstMouse = true;
            isLeftMousePressed = true;
            int readY = glutGet(GLUT_WINDOW_HEIGHT) - ypos - 1;
            unsigned char clickedId = 0;
            glReadPixels(xpos, readY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &clickedId);
            std::cout << "clicked id " << (int)clickedId << std::endl;
            handlePicking(static_cast<int>(clickedId));
        }
        else if (state == GLUT_UP) {
            isLeftMousePressed = false;
        }
    }
    lastX = xpos;
    lastY = ypos;
}

void mouseCallback(int xpos, int ypos) {
    if (!isLeftMousePressed) return;
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
    lastX = xpos;
    lastY = ypos;
    glutPostRedisplay();
}

void timerFunc(int value) {
    float currentFrameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    for (auto const& obj : sceneObjects) {
        if (obj->isTextureAnimated) {
            glm::mat4 texMat = glm::mat4(1.0f);
            texMat = glm::translate(texMat, glm::vec3(currentFrameTime * 0.2f, 0.0f, 0.0f));
            texMat = glm::rotate(texMat, currentFrameTime * 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
            obj->setTextureMatrix(texMat);
        }
    }
    for (auto const& animObj : animatedObjects) {
        animObj->update(deltaTime);
    }
    //dirLights[0]->update(currentFrameTime);
    camera.move(inputManager, collisionCircles);
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    skyboxShader = globalShaderManager.getShaderProgram("Shaders/skybox");
    skybox = std::make_unique<Skybox>(SKYBOX_FACES, skyboxShader);


    //lights
    for (auto const& setup : DIR_LIGHTS_SETUP) {
        if (dirLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum directional lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        dirLights.push_back(std::make_unique<DirectionalLight>(setup));
    }

    for (auto const& setup : POINT_LIGHTS_SETUP) {
        if (pointLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum point lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        pointLights.push_back(std::make_unique<PointLight>(setup));
    }

    for (auto const& setup : SPOT_LIGHTS_SETUP) {
        if (spotLights.size() == MAX_POINT_LIGHTS) {
            std::cerr << "Maximum spot lights exceeded: ignoring remaining" << std::endl;
            break;
        }
        spotLights.push_back(std::make_unique<SpotLight>(setup));
    }

    int objId = 1;
    for (auto const& objInfo : SCENE_OBJECTS_SETUP) {
        sceneObjects.push_back(std::make_unique<Object>(objInfo,globalShaderManager, globalMeshManager));
        sceneObjects.back()->isTextureAnimated = objInfo.isTexAnim;
        sceneObjects.back()->setId(objId);
        if (objInfo.pointLightIndex != -1) {
            sceneObjects.back()->setLight(pointLights[objInfo.pointLightIndex].get());
        } if (objInfo.spotLightIndex != -1) {
            sceneObjects.back()->setLight(spotLights[objInfo.spotLightIndex].get());
        }
        objId++;
    }
    for (auto const& spriteInfo : SPRITE_OBJECTS_SETUP) {
        spriteObjects.push_back(std::make_unique<SpriteObject>(spriteInfo, globalShaderManager, globalMeshManager));
    }
    for (auto const& animObjInfo : ANIMATED_OBJECTS_SETUP) {
        if (spriteObjects.size() == 0) animatedObjects.push_back(std::make_unique<AnimatedObject>(animObjInfo, globalShaderManager, globalMeshManager, nullptr));

        else animatedObjects.push_back(std::make_unique<AnimatedObject>(animObjInfo, globalShaderManager, globalMeshManager, std::move(spriteObjects[0])));
        animatedObjects.back()->setId(objId);
        objId++;
    }
    for (auto const& transpObjInfo : TRANSPARENT_OBJECTS_SETUP) {
        transparentObjects.push_back(std::make_unique<Object>(transpObjInfo, globalShaderManager, globalMeshManager));
    }
    int borderIndex = 0;
    ObjectSetup curObject;
    glm::vec3 rotation = glm::vec3(0.0f, -glm::radians(90.0f), 0.0f);
    for (float x = MIN_X; x < MAX_X; x += TILE_SIZE) {
        rotation.y = -rotation.y;
        for (float z = MIN_Z; z < MAX_Z; z += TILE_SIZE) {
            if (x == MIN_X || x == MAX_X-TILE_SIZE || z == MIN_Z || z == MAX_Z-TILE_SIZE) {
                curObject = { BORDER_OBJECTS_PATHS[borderIndex % BORDER_OBJECTS_PATHS.size()], 
                              mainLightShaderName, glm::vec3(x, 0.5f, z), glm::vec3(0.0f), glm::vec3(0.05f) };
                sceneObjects.push_back(std::make_unique<Object>(curObject, globalShaderManager, globalMeshManager));
                collisionCircles.push_back({ curObject.position, STONE_COLLISION_RADIUS });
                borderIndex++;
            }
            else {
                curObject = { tilePath, mainLightShaderName, glm::vec3(x, 0.0f, z), rotation, glm::vec3(15.0f)};
                sceneObjects.push_back(std::make_unique<Object>(curObject, globalShaderManager, globalMeshManager));
            }
        }
    }


    mainLightShader = globalShaderManager.getShaderProgram(mainLightShaderName);
    // fog
    globalShaderManager.setFogInShaders(FOG_COLOR, FOG_START, FOG_END);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClearStencil(0);
    glm::mat4 proj = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::vec3 cameraPos = camera.getPosition();
    skybox->draw(view, proj);

    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    // light setup
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

    for (auto const& obj : sceneObjects) {
        glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
        if (obj->getId() > 0) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }
        else {
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
        obj->draw(view, proj, cameraPos);
    }

    
    for (auto const& animObj : animatedObjects) {
        glStencilFunc(GL_ALWAYS, animObj->getId(), 0xFF);
        if (animObj->getId() > 0) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }
        else {
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
        animObj->draw(view, proj, cameraPos);
    }
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    std::map<float, Object*> sortedTransparent;

    for (auto const& obj : transparentObjects) {
        float dist = glm::distance(cameraPos, obj->getPosition());
        sortedTransparent[dist] = obj.get();
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto it = sortedTransparent.rbegin(); it != sortedTransparent.rend(); it++) {
        it->second->draw(view, proj, cameraPos);
    }
    glDisable(GL_BLEND);



    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
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

