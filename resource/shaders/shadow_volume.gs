#version 330

layout (triangles_adjacency) in;    // six vertices in
layout (triangle_strip, max_vertices = 18) out; // 4 per quad * 3 triangle vertices + 6 for near/far caps

in vec3 PosL[]; // an array of 6 vertices (triangle with adjacency)

uniform vec3 lightPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

float EPSILON = 0.0001;

// Emit a quad using a triangle strip
void EmitQuad(vec3 StartVertex, vec3 EndVertex)
{    
    mat4 mvp = projection * view * model;

    // Vertex #1: the starting vertex (just a tiny bit below the original edge)
    vec3 LightDir = normalize(StartVertex - lightPos);   
    gl_Position = projection *  view * vec4((StartVertex + LightDir * EPSILON), 1.0);
    EmitVertex();
 
    // Vertex #2: the starting vertex projected to infinity
    gl_Position = projection *  view * vec4(LightDir, 0.0);
    EmitVertex();
    
    // Vertex #3: the ending vertex (just a tiny bit below the original edge)
    LightDir = normalize(EndVertex - lightPos);
    gl_Position = projection *  view * vec4((EndVertex + LightDir * EPSILON), 1.0);
    EmitVertex();
    
    // Vertex #4: the ending vertex projected to infinity
    gl_Position = projection *  view * vec4(LightDir , 0.0);
    EmitVertex();

    EndPrimitive();            
}


void main()
{
    mat4 mvp = projection * view * model;
    vec3 e1 = PosL[2] - PosL[0];
    vec3 e2 = PosL[4] - PosL[0];
    vec3 e3 = PosL[1] - PosL[0];
    vec3 e4 = PosL[3] - PosL[2];
    vec3 e5 = PosL[4] - PosL[2];
    vec3 e6 = PosL[5] - PosL[0];

    vec3 Normal = normalize(cross(e1,e2));
    vec4 pos0_4 = model * vec4(PosL[0], 1.0);
    vec4 pos2_4 = model * vec4(PosL[2], 1.0);
    vec4 pos4_4 = model * vec4(PosL[4], 1.0);
    vec3 pos0 = pos0_4.xyz;
    vec3 pos2 = pos2_4.xyz;
    vec3 pos4 = pos4_4.xyz;

    vec3 LightDir = normalize(lightPos - pos0);

    // Handle only light facing triangles
    if (dot(Normal, LightDir) > 0) { //

        Normal = cross(e3,e1);

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = pos0;
            vec3 EndVertex = pos2;
            EmitQuad(StartVertex, EndVertex);
        }

        Normal = cross(e4,e5);
        LightDir = lightPos - pos2;

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = pos2;
            vec3 EndVertex = pos4;
            EmitQuad(StartVertex, EndVertex);
        }

        Normal = cross(e2,e6);
        LightDir = lightPos - pos4;

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = pos4;
            vec3 EndVertex = pos0;
            EmitQuad(StartVertex, EndVertex);
        }

        // render the front cap

        LightDir = (normalize(pos0 - lightPos));
        gl_Position = mvp * vec4((pos0 + LightDir * EPSILON), 1.0);
        EmitVertex();

        LightDir = (normalize(pos2 - lightPos));
        gl_Position = mvp * vec4((pos2 + LightDir * EPSILON), 1.0);
        EmitVertex();

        LightDir = (normalize(pos4 - lightPos));
        gl_Position = mvp * vec4((pos4 + LightDir * EPSILON), 1.0);
        EmitVertex();
        EndPrimitive();
 
        // render the back cap
        LightDir = pos0 - lightPos;
        gl_Position = mvp * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = pos4 - lightPos;
        gl_Position = mvp * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = pos2 - lightPos;
        gl_Position = mvp * vec4(LightDir, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}