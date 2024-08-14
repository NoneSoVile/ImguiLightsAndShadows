#include "ColorLevels.h"
#include "Shader.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include <string>

class Shader;


void ColorLevels::init(int w, int h) {
	/*initialize variables*/
	loadVars();

    /*load shaders*/
	loadShader();

    /*create vertices*/
    loadMesh();


    /*create and load textures*/
    loadTexture();

}

void ColorLevels::loadVars() {
	scale = vec2f(1, 1);
}

void ColorLevels::loadShader(){
 	/*create shaders
    */
    string vertexShaderFile = resourceFolder + std::string("shaders/basic.vert");
    string fragShaderFile = resourceFolder + std::string("shaders/colorlevels.frag");

    renderShader = std::make_shared<Shader>();
    renderShader->setShader(vertexShaderFile.c_str(), fragShaderFile.c_str());
    if (!renderShader->isValid()) {
        printf("failed to create shader: %s\n", vertexShaderFile.c_str());
    }
    else {
        printf("succeeded to create shader: %s  programid = %d\n", 
        vertexShaderFile.c_str(), renderShader->program);
    }
}

void ColorLevels::loadMesh()
{
    GLfloat vertices[] = {
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,    //RB       from the view direction of -z
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,     //RT 
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,    //LT
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f     //LB
	};

	GLuint indices[] = {
	0,1,3,            
	1,2,3             
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1 , &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); CHECK_GL;

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER , sizeof(indices) , indices , GL_STATIC_DRAW ); CHECK_GL;

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); CHECK_GL;

	// TexCoord attribute
	glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 5*sizeof(GLfloat) , (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1); CHECK_GL;

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	CHECK_GL;
}

void ColorLevels::loadTexture()
{
    string name = "resource/images/sanmiguel_cam25.png";
	cv::Mat img = cv::imread(name, cv::IMREAD_UNCHANGED);
	if (img.empty())
	{
		printf("failed on opening  texture image %s \n", name.c_str());
		return;
	}
	    // Verify that the image is indeed 16-bit per channel
    if (img.depth() == CV_16U) {
        std::cout << "Image loaded successfully as 16-bit per channel." << std::endl;
        std::cout << "Image dimensions: " << img.cols << "x" << img.rows << std::endl;
    } else {
        std::cerr << "Unexpected image depth: " << img.depth() << std::endl;
    }


	cv::Mat img_display;
	cv::Size s = img.size();
	int h = s.height;
	int w = s.width;
	if (img.depth() == CV_16U) {
		img.convertTo(img_display, CV_8U, 1.0 / 256.0);  // Scale down to 8-bit for display
		s = img_display.size();
		h = s.height;
		w = s.width;
		//cv::imshow("8-bit Image", img_display);
		cv::cvtColor(img_display, img_display, cv::COLOR_RGBA2BGRA);
	}
	else {
		cv::cvtColor(img, img_display, cv::COLOR_RGB2BGRA);
	}
	
	printf("Created a new texture with id = %d h = %d, w = %d, DEPTH = %d\n", ourTexture, h, w, img_display.depth());
	//cv::waitKey(0);
	SetupTextureData(ourTexture, w, h, img_display.data);
	
}

void ColorLevels::updateUI(int w, int h) {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::SetNextWindowBgAlpha(0.5f);
    
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Helloxxx, world!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Settings");               // Display some text (you can use a format strings too)


        
		ImGui::NewLine();ImGui::NewLine();	
		ImGui::SliderFloat("inputBlack", &inputBlack, 0, 10);
		ImGui::SliderFloat("inputWhite", &inputWhite, 0, 10);
		ImGui::SliderFloat("gamma", &gamma, 0, 3);
		ImGui::SliderFloat("outputBlack", &outputBlack, 0, 10);
		ImGui::SliderFloat("outputWhite", &outputWhite, 0, 10);

       

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void ColorLevels::drawSprite(int w, int h, vec2f offset){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderShader->Use();
    renderShader->UseAndBindTexture("ourTexture", ourTexture);
	renderShader->setUniform2fv("scale", scale, 1);
	renderShader->setUniform2fv("translation", offset, 1);
	renderShader->setUniform1f("inputBlack", inputBlack);
	renderShader->setUniform1f("outputBlack", outputBlack);
	renderShader->setUniform1f("inputWhite", inputWhite);
	renderShader->setUniform1f("outputWhite", outputWhite);
	renderShader->setUniform1f("gamma", gamma);
	glBindVertexArray(VAO); CHECK_GL;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); CHECK_GL;
	GLuint positionHandle = 0;
	GLuint texcoordHandle = 1;
    glEnableVertexAttribArray(positionHandle);
    glEnableVertexAttribArray(texcoordHandle);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); CHECK_GL;

    glDisableVertexAttribArray(positionHandle);
	glDisableVertexAttribArray(texcoordHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

vec2f ColorLevels::ndcToScreen(vec2f ndcPosition){
	vec2f outPos;
	ndcPosition.y *= -1;
	outPos = ndcPosition * screenDim / 2.0 + screenDim/2.0;
	return outPos;
}
    
vec2f ColorLevels::screenToNdc(vec2f screenPosition){
	vec2f outPos;
	outPos = (2.0*screenPosition - screenDim) /screenDim;
	outPos.y *= -1;
	return outPos;
}



void ColorLevels::run(float w, float h) {
	static float dt = 0;
	//stepSimulation(w, h, dt);
    glViewport(0, 0, w, h);
	drawSprite(w, h, vec2f(0, 0));
	
    updateUI(w, h);
}
