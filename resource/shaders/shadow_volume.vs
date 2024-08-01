#version 330

layout (location = 0) in vec3 Position; 
layout (location = 1) in vec3 Normal;                                            
layout (location = 2) in vec2 TexCoord;                                             
                                              
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec3 PosL;


out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;
                                                                                    
void main()                                                                         
{                                                                                   
    PosL = Position;

    vs_out.Normal = transpose(inverse(mat3(model))) * Normal;

    vs_out.TexCoords = TexCoord;
}
