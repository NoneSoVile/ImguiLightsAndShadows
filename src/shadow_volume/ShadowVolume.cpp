#include "ShadowVolume.h"
#include "Shader.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include <string>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Sk_Shader.h>
#include <Sk_Camera.h>
#include "Common.h"


class Shader;


void ShadowVolume::init(int w, int h) {
	/*initialize variables*/
	loadVars();

    /*load shaders*/
	loadShader();

    /*create vertices*/
    loadMesh();


    /*create and load textures*/
    loadTexture();

}

std::shared_ptr<SKShader> ShadowVolume::createSKShader(std::string vsFile, std::string fsFile, std::string gsFile){
    char* gsBuf = nullptr;
    if(gsFile != "resource/"){
        gsBuf = (char*)gsFile.c_str();
    }
	std::shared_ptr<SKShader> shader = std::make_shared<SKShader>(vsFile.c_str(), fsFile.c_str(), gsBuf);

    if (!shader->isValid()) {
        printf("failed to create shader: %s\n", vsFile.c_str());
    }
    else {
        printf("succeeded to create shader: %s  programid = %d\n", 
        vsFile.c_str(), shader->getProgramID());
    }
	return shader;
}

void ShadowVolume::loadVars() {
	lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = SKCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    modelTranslation = glm::vec3(0.0f, -0.0f, 0.0f);
}
#define BIND_SHADER(shader, index, nameShader, i) if(i == index){nameShader = shader;} 
void ShadowVolume::loadShader(){
	/*create shaders
	 */
	std::string fileList[][3] = {
        {"shaders/null_technique.vs", "shaders/null_technique.fs", ""},
        {"shaders/shadow_volume.vs", "shaders/shadow_volume.fs", "shaders/shadow_volume.gs"},
        {"shaders/basic_light.vs", "shaders/basic_light.fs", ""},
	};
	for (size_t i = 0; i < ARRAY_SIZE(fileList); i++)
	{
		string vertexShaderFile = resourceFolder + std::string(fileList[i][0]);
		string fragShaderFile = resourceFolder + std::string(fileList[i][1]);
        string geoShaderFile = resourceFolder + std::string(fileList[i][2]);

		std::shared_ptr<SKShader> shader = createSKShader(vertexShaderFile, fragShaderFile, geoShaderFile);
		BIND_SHADER(shader, 0, simpleDepthShader, i);
        BIND_SHADER(shader, 1, stencileShader, i);
        BIND_SHADER(shader, 2, normalSceneShader, i);
	}
}

void ShadowVolume::loadMesh()
{
    if (!box_shadow_volume.LoadMesh("resource/models/adj_box.obj", true)) {
        printf("Mesh box_shadow_volume load failed\n");
    }

    if (!box.LoadMesh("resource/models/adj_box.obj", false)) {
        printf("Mesh normal box load failed\n");
    }
}

void ShadowVolume::loadTexture()
{
    string name = "resource/images/wood.png";
	cv::Mat img = cv::imread(name, cv::IMREAD_UNCHANGED);
	if (img.empty())
	{
		printf("failed on opening  texture image %s \n", name.c_str());
		return;
	}
	cv::cvtColor(img, img, cv::COLOR_RGB2BGRA);
	cv::Size s = img.size();  
	int h = s.height;
	int w = s.width;

	SetupTextureData(ourTexture, w, h, img.data);
	printf("Created a new texture with id = %d h = %d, w = %d\n", ourTexture, h, w);
	woodTexture = ourTexture;

    normalSceneShader->use();
    normalSceneShader->setInt("diffuseTexture", 0);
}

void ShadowVolume::updateUI(int w, int h) {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::SetNextWindowBgAlpha(0.5f);
    
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Helloxxx, world!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Settings");               // Display some text (you can use a format strings too)

        
		ImGui::NewLine();ImGui::NewLine();
		
        bool ret = ImGui::SliderFloat3("model translate", (float*)&modelTranslation, -20.f, 20.0f);
        if(ret){
            camera.RotateCameraByMouseMove(0, 0, modelTranslation);
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void ShadowVolume::drawSprite(int w, int h, vec2f offset){

}

vec2f ShadowVolume::ndcToScreen(vec2f ndcPosition){
	vec2f outPos;
	ndcPosition.y *= -1;
	outPos = ndcPosition * screenDim / 2.0 + screenDim/2.0;
	return outPos;
}
    
vec2f ShadowVolume::screenToNdc(vec2f screenPosition){
	vec2f outPos;
	outPos = (2.0*screenPosition - screenDim) /screenDim;
	outPos.y *= -1;
	return outPos;
}

void ShadowVolume::onScroll(float dxScreen, float dyScreen){
	camera.RotateCameraByMouseMove(dxScreen, dyScreen, modelTranslation);
}

void ShadowVolume::onFling(float vx, float vy){

}

void ShadowVolume::run(float w, float h) {
	static float dt = 0;
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, w, h);
   
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderSceneIntoDepth(*simpleDepthShader, w, h); CHECK_GL;
    //updateUI(w, h);
    //return;
    glEnable(GL_STENCIL_TEST);

    RenderShadowVolIntoStencil(*stencileShader, w, h); CHECK_GL;

    glDrawBuffer(GL_BACK);
    // Draw only if the corresponding stencil value is zero
    glStencilFunc(GL_EQUAL, 0x0, 0xFF);
    // prevent update to the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); CHECK_GL;

    glDepthMask(GL_FALSE);
    normalSceneShader->use();
    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    normalSceneShader->setMat4("projection", projection);
    normalSceneShader->setMat4("view", view);
    // set lighting uniforms
    normalSceneShader->setVec3("lightPos", lightPos);
    normalSceneShader->setVec3("viewPos", camera.Position);
    normalSceneShader->setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    renderScene(*normalSceneShader, box, w, h); CHECK_GL;

	updateUI(w, h);
}

void ShadowVolume::RenderSceneIntoDepth(SKShader &shader, float w, float h){
    glDrawBuffer(GL_NONE); CHECK_GL;
    shader.use();
    float near_plane = 0.09f;
    float far_plane = 100.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)w / (float)h, near_plane, far_plane);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    // set lighting uniforms
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.Position);
    shader.setFloat("far_plane", far_plane);
    renderScene(shader, box, w, h); CHECK_GL;
    glDrawBuffer(GL_BACK); CHECK_GL;
}
    
void ShadowVolume::RenderShadowVolIntoStencil(SKShader &shader, float w, float h){
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    // Set the stencil test per the depth fail algorithm
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    stencileShader->use();
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)w / (float)h, near_plane, far_plane);
    glm::mat4 view = camera.GetViewMatrix();
    stencileShader->setMat4("projection", projection);
    stencileShader->setMat4("view", view);
    // set lighting uniforms
    stencileShader->setVec3("lightPos", lightPos);
    renderScene(shader, box_shadow_volume, w, h, true);


    // Restore local stuff
    glDisable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
}

// renders the 3D scene
// --------------------
void ShadowVolume::renderScene(SKShader &shader, Mesh& mesh, float w, float h, bool isShadowVolume)
{
 // room cube
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(29.0f));
    shader.setMat4("model", model);
    if (!isShadowVolume) {
        glDisable(GL_CULL_FACE); CHECK_GL; // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
        shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        mesh.Render(); CHECK_GL;
        shader.setInt("reverse_normals", 0); // and of course disable it
        glEnable(GL_CULL_FACE); CHECK_GL;
    }

    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    mesh.Render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    mesh.Render(); CHECK_GL;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    mesh.Render();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    mesh.Render();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    mesh.Render();
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------

void ShadowVolume::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

