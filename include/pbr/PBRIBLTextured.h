#pragma once
#include "Common.h"
#include "ShaderTester.h"
#include <string>
#include "NvMath.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sk_Camera.h"
#include "SK_Model.h"
using MB::vec2i;
using MB::vec2f;
using MB::matrix4f;

#define NR_LIGHTS 4
class SKShader;
class PBRIBLTextured: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;

    PBRIBLTextured();


protected:
    std::shared_ptr<SKShader> pbrShader;
    std::shared_ptr<SKShader> equirectangularToCubemapShader;
    std::shared_ptr<SKShader> irradianceShader;
    std::shared_ptr<SKShader> prefilterShader;
    std::shared_ptr<SKShader> brdfShader;
    std::shared_ptr<SKShader> backgroundShader;


    glm::vec3 lightPositions[NR_LIGHTS];
    glm::vec3 lightColors[NR_LIGHTS];
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    unsigned int captureFBO;
    unsigned int captureRBO;
    unsigned int envCubemap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;
    unsigned int irradianceMap;

    unsigned int ironAlbedoMap;
    unsigned int ironNormalMap;
    unsigned int ironMetallicMap;
    unsigned int ironRoughnessMap;
    unsigned int ironAOMap;

    // gold
    unsigned int goldAlbedoMap;
    unsigned int goldNormalMap;
    unsigned int goldMetallicMap;
    unsigned int goldRoughnessMap;
    unsigned int goldAOMap;

    // grass
    unsigned int grassAlbedoMap;
    unsigned int grassNormalMap;
    unsigned int grassMetallicMap;
    unsigned int grassRoughnessMap;
    unsigned int grassAOMap;

    // plastic
    unsigned int plasticAlbedoMap;
    unsigned int plasticNormalMap;
    unsigned int plasticMetallicMap;
    unsigned int plasticRoughnessMap;
    unsigned int plasticAOMap;

    // wall
    unsigned int wallAlbedoMap;
    unsigned int wallNormalMap;
    unsigned int wallMetallicMap;
    unsigned int wallRoughnessMap;
    unsigned int wallAOMap;


    unsigned int sphereVAO = 0;
    unsigned int indexCount;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;
    glm::vec3 cameraTranslation;
    glm::vec3 lookAt;
    
    
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
    unsigned int loadTexture(char const * path, bool relativeToAbsolute = true);

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);

    void renderScene(SKShader &shader, float w, float h);
    void renderCube();
    void renderQuad();
    void renderSphere();

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