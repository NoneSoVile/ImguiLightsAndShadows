#pragma once
#include "Common.h"
#include "RenderState.h"
#include <GLFW/glfw3.h>
#include <string>
using std::string;
class Shader;
#define MAX_RESULT_LEN 10

class MouseEvent{
public:
	virtual void onScroll(float dxScreen, float dyScreen){}
	virtual void onFling(float vx, float vy){}
};

class KeyEvent{
public:
    virtual void processInput(GLFWwindow *window){}
};

class ShaderTester: public RenderState, public MouseEvent, public KeyEvent
{
protected:

    string resourceFolder = "resource/";
    //The shaders to tested
    std::shared_ptr<Shader> sumOfArrayShader;

    //the size of workgroup
    uint Size_X;
    uint Size_Y;
    uint Size_Z;

    uint work_groups_x;
    uint work_groups_y;
    uint work_groups_z;

    //The buffers and textures
    GLuint inputDataBufferToSum;
    GLuint singleGroupOutputBuffer;
    GLuint outputDataBufferSummed;
    uint inputDataSize;
    uint outputDataSize;
    uint singleGroupOutputDataSize;
    uint alignedDataSize;
    float* inputData;
    float outputData[MAX_RESULT_LEN];
    float expectedSum[MAX_RESULT_LEN];
public:
    void init();
    void run(float w, float h);
};