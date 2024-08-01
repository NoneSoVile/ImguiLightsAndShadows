#version 330

layout (location = 0) in vec3 Position; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool reverse_normals;
void main()
{   
    mat4 mvp = projection * view * model;
    if(reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
    else
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;

    vs_out.TexCoords = aTexCoords;
    gl_Position = mvp * vec4(Position, 1.0);
}
