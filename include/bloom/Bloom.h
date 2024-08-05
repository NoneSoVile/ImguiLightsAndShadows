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
class Bloom: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;


protected:

    int bloom = 1;
    bool bloomKeyPressed = false;
    float exposure = 1.0f;


    std::shared_ptr<SKShader> shaderBloom;
    std::shared_ptr<SKShader> shaderLight;
    std::shared_ptr<SKShader> shaderBlur;
    std::shared_ptr<SKShader> shaderBloomFinal;

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    unsigned int woodTexture = 0;
    unsigned int containerTexture = 0;


    unsigned int hdrFBO;
    unsigned int colorBuffers[2];
    unsigned int rboDepth;
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;
    glm::vec3 cameraTranslation;
    glm::vec3 lookAt;
    
    
    glm::vec3 lightPos;
    SKCamera camera;
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;

protected:
    virtual void loadVars();
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();
    unsigned int loadTexture(char const * path);

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);

    void renderScene(SKShader &shader, float w, float h);
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