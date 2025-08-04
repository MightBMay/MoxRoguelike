#version 330 core
uniform sampler2D trailTexture;


in vec4 vColor;
in vec2 vPosition; // Screen-space position

out vec4 FragColor;


void main()
{
    // Tile the texture based on screen position
    vec2 uv = (vPosition * scale) + offset;
    vec4 texColor = texture(trailTtexture, fract(uv)); // fract() creates repeating pattern
    
    // Combine with original color (preserves your alpha fading)
    FragColor = texColor * vColor;
}