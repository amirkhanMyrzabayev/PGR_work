#include <iostream>
#include <map>
#include "pgr.h"
#include "InputManager.h"
#include "Camera.h"
#include "AnimatedObject.h"
#include "SegmentedArm.h"
#include "Skybox.h"
#include "parametry.h"

// Global variables
float lastX = WIN_WIDTH / 2.0; ///< center of the screen x 
float lastY = WIN_WIDTH / 2.0; ///< center of the screen y
bool firstMouse = true;
bool isLeftMousePressed = false;
float lastFrameTime = 0.0f;

// Skybox shader position
GLuint skyboxShader = 0;

// Lights vectors
std::vector<std::unique_ptr<DirectionalLight>> dirLights;
std::vector<std::unique_ptr<PointLight>> pointLights;
std::vector<std::unique_ptr<SpotLight>> spotLights;

// Main light shaders to setup lights in them
std::vector<GLuint> lightShaders;

// Global storages of objects and collision circles
std::vector<std::unique_ptr<Object>> sceneObjects;
std::vector<std::unique_ptr<AnimatedObject>> animatedObjects;
std::vector<std::unique_ptr<SpriteObject>> spriteObjects;
std::vector<std::unique_ptr<Object>> transparentObjects;
std::unique_ptr<Skybox> skybox;
std::unique_ptr<SegmentedArm> segmentedArm;
std::vector<std::pair<glm::vec3, float>> collisionCircles;

// Global managers and camera
InputManager inputManager;
Camera camera(STATIC_CAMERAS[0].position, CAMERA_PATH);
MeshManager globalMeshManager;
ShaderManager globalShaderManager;


/// @brief Handles normal keyboard button presses.
/// @param key The character of the pressed key.
/// @param x Mouse X position when pressed.
/// @param y Mouse Y position when pressed.
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

/// @brief Handles normal keyboard button releases.
/// @param key The character of the released key.
void keyReleased(unsigned char key, int x, int y) {
    inputManager.releaseKey(key);
}

/// @brief Handles special keys like arrows or F1-F12.
/// @param key The code for the special key.
void specialKeyPressed(int key, int x, int y) {
    inputManager.pressSpecialKey(key);
    if (key == GLUT_KEY_F1) {
        camera.setCameraState(splineCamera);
    }
    else if (key == GLUT_KEY_F2)
    {
        camera.setCameraState(movingWithObject);
    }
}



/// @brief Handles special key releases.
/// @param key The code for the special key.
void specialKeyReleased(int key, int x, int y) {
    inputManager.releaseSpecialKey(key);
}


/// @brief Finds an object in the scene using its unique ID.
/// @param id The ID to look for.
/// @return Pointer to the object if found, nullptr otherwise.
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

/// @brief Checks what was clicked and toggles its light or animation.
/// @param id The ID of the object that was clicked.
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


/// @brief Handles mouse button clicks and performs stencil-buffer picking.
/// @param button Which mouse button was clicked.
/// @param state Pressed or released.
/// @param xpos Mouse horizontal position.
/// @param ypos Mouse vertical position.
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

/// @brief Updates camera rotation when the mouse moves while being held.
/// @param xpos Current horizontal mouse position.
/// @param ypos Current vertical mouse position.
void mouseCallback(int xpos, int ypos) {
    if (!isLeftMousePressed) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if (isLeftMousePressed && camera.getCameraState() != movingWithObject) {
        float offset_x = xpos - lastX;
        float offset_y = lastY - ypos;
        camera.processMouseMovement(offset_x, offset_y);
    }
    lastX = xpos;
    lastY = ypos;
    glutPostRedisplay();
}


/// @brief Main logic loop that updates physics, movement, and camera.
/// @param value Timer value (unused).
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
        obj->update(currentFrameTime);
    }
    for (auto const& animObj : animatedObjects) {
        animObj->update(deltaTime);
    }

    dirLights[0]->update(currentFrameTime);
    segmentedArm->update(deltaTime);
    if (camera.getCameraState() == movingWithObject) camera.moveWithObject(animatedObjects[0]->getPosition() + glm::vec3(0.0f, 0.2f, 0.0f),
                                                                            animatedObjects[0]->getYaw());
    else camera.move(inputManager, collisionCircles, deltaTime);
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 0);
}

/// @brief Loads resources, creates objects, and sets up OpenGL state.
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

        else animatedObjects.push_back(std::make_unique<AnimatedObject>(animObjInfo, globalShaderManager, globalMeshManager, &(*spriteObjects[0])));
        animatedObjects.back()->setId(objId);
        objId++;
    }
    segmentedArm = std::make_unique<SegmentedArm>(ARM_CYLINDER_SETUP, globalShaderManager, globalMeshManager, glm::vec3(0.0f, CYLINDER_HEIGHT, 0.0f));
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


    for (auto const& shaderName : LIGHT_SHADERS) {
        lightShaders.push_back(globalShaderManager.getShaderProgram(shaderName));
    }
    // fog
    globalShaderManager.setFogInShaders(FOG_COLOR, FOG_START, FOG_END);
}


/// @brief The main rendering function that draws everything to the screen.
void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glm::mat4 proj = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::vec3 cameraPos = camera.getPosition();
    skybox->draw(view, proj);

    // light setup
    for (auto const& shaderProgram : lightShaders) {
        glUseProgram(shaderProgram);
        int i = 0;
        for (i = 0; i < dirLights.size(); i++) {
            if (i == MAX_DIR_LIGHTS) {
                std::cerr << "Maximum directional lights " << MAX_DIR_LIGHTS << " exceeded: ignoring remaining" << std::endl;
                break;
            }
            dirLights[i]->bindUniforms(shaderProgram, i);
        }
        GLint numLightLoc = glGetUniformLocation(shaderProgram, "numDirLights");
        glUniform1i(numLightLoc, i);
        for (i = 0; i < pointLights.size(); i++) {
            if (i == MAX_POINT_LIGHTS) {
                std::cerr << "Maximum point lights " << MAX_POINT_LIGHTS << " exceeded: ignoring remaining" << std::endl;
                break;
            }
            pointLights[i]->bindUniforms(shaderProgram, i);
        }
        numLightLoc = glGetUniformLocation(shaderProgram, "numPointLights");
        glUniform1i(numLightLoc, i);
        for (int i = 0; i < spotLights.size(); i++) {
            if (i == MAX_SPOT_LIGHTS) {
                std::cerr << "Maximum spot lights " << MAX_SPOT_LIGHTS << " exceeded: ignoring remaining" << std::endl;
                break;
            }
            spotLights[i]->bindUniforms(shaderProgram, i);
        }
        numLightLoc = glGetUniformLocation(shaderProgram, "numSpotLights");
        glUniform1i(numLightLoc, i);
    }
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

    segmentedArm->draw(view, proj, cameraPos);
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


    for (auto const& spriteObj : spriteObjects)
    {
        if (spriteObj->isClock()) {
            spriteObj->drawClock(view, lastFrameTime, dirLights[0]->getSpeed());
        }
    }
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
    glutKeyboardUpFunc(keyReleased);
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
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

