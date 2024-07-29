#pragma once
#include "Common.h"
#include "ShaderTester.h"
#include <string>
#include "NvMath.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using MB::vec2i;
using MB::vec2f;
using MB::matrix4f;

class SKShader;
#define MAX_PARTICLES 4
class ShadowMappingDepth: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;


protected:
    std::shared_ptr<SKShader> simpleDepthShader;
    std::shared_ptr<SKShader> debugDepthQuad;
    GLuint VBO;
	GLuint VAO;
	GLuint EBO;

    GLuint writeTexture = 0;
    GLuint ourTexture = 0;


    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    unsigned int planeVAO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    unsigned int woodTexture = 0;
    glm::vec3 lightPos;

protected:
    virtual void loadVars();
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);

    void renderScene(const SKShader &shader);
    void renderCube();
    void renderQuad();

    std::shared_ptr<SKShader> createSKShader(std::string vsFile, std::string fsFile);


public:
    virtual void init(int w, int h);
    virtual void run(float w, float h);

public:
    vec2f ndcToScreen(vec2f ndcPosition);
    vec2f screenToNdc(vec2f screenPosition);


};