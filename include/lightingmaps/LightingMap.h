#pragma once
#include "Common.h"
#include "ShaderTester.h"
#include <string>
#include "NvMath.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sk_Camera.h"
using MB::vec2i;
using MB::vec2f;
using MB::matrix4f;

class SKShader;
class LightingMap: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;


protected:
    std::shared_ptr<SKShader> lightCubeShader;
    std::shared_ptr<SKShader> lightingShader;

    unsigned int lightCubeVAO;
    unsigned int VBO, cubeVAO;


    unsigned int diffuseMap = 0;
    unsigned int specularMap = 0;

    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;
    glm::vec3 modelTranslation;
    
    
    glm::vec3 lightPos;
    SKCamera camera;
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

protected:
    virtual void loadVars();
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();
    unsigned int loadTexture(char const * path);

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);

    void renderScene(const SKShader &shader);
    void renderCube();
    void renderQuad();

    std::shared_ptr<SKShader> createSKShader(std::string vsFile, std::string fsFile, std::string gsFile);
    virtual void processInput(GLFWwindow *window);

public:
    virtual void init(int w, int h);
    virtual void run(float w, float h);

public:
    vec2f ndcToScreen(vec2f ndcPosition);
    vec2f screenToNdc(vec2f screenPosition);

    virtual void onScroll(float dxScreen, float dyScreen);
	virtual void onFling(float vx, float vy);
    


};