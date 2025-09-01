#pragma once
#include "Common.h"
#include "ShaderTester.h"
#include <string>
#include "NvMath.h"
using MB::vec2i;
using MB::vec2f;
using MB::matrix4f;

class Shader;
#define MAX_PARTICLES 4
class ColorLevels: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;


protected:
    std::shared_ptr<Shader> renderShader;
    GLuint VBO;
	GLuint VAO;
	GLuint EBO;

    GLuint writeTexture = 0;
    GLuint ourTexture = 0;


    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;

    // Input level parameters
    float inputBlack = 0;   // e.g., 0.0
    float inputWhite = 1;   // e.g., 1.0
    float gamma = 1;        // e.g., 1.0 (midpoint)

    // Output level parameters
    float outputBlack = 0;  // e.g., 0.0
    float outputWhite = 1;  // e.g., 1.0

protected:
    virtual void loadVars();
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);



public:
    virtual void init(int w, int h);
    virtual void run(float w, float h);

public:
    vec2f ndcToScreen(vec2f ndcPosition);
    vec2f screenToNdc(vec2f screenPosition);


};