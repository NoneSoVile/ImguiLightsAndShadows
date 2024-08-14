#version 310 es
precision highp float;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;

// Input level parameters
uniform float inputBlack;   // e.g., 0.0
uniform float inputWhite;   // e.g., 1.0
uniform float gamma;        // e.g., 1.0 (midpoint)

// Output level parameters
uniform float outputBlack;  // e.g., 0.0
uniform float outputWhite;  // e.g., 1.0
void main()
{
    color = texture(ourTexture, TexCoord);

    // Step 1: Remap input levels
    color.rgb = (color.rgb - inputBlack) / (inputWhite - inputBlack);
    
    // Step 2: Apply gamma correction
    color.rgb = pow(color.rgb, vec3(1.0 / gamma));
    
    // Step 3: Remap to output levels
    color.rgb = color.rgb * (outputWhite - outputBlack) + outputBlack;
    
    // Step 4: Clamp the final output to ensure it's within [0, 1] range
    color.rgb = clamp(color.rgb, 0.0, 1.0);

    
}