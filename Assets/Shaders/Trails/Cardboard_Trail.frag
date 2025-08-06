#version 330 core
uniform sampler2D trailTexture;
uniform vec2 scale = vec2(1.0,1.0);
uniform vec2 offset = vec2(0.0,0.0);

in vec4 vColor;
in vec2 vPosition; // Screen-space position

out vec4 FragColor;


void main()
{
    // Tile the texture based on screen position
    vec2 uv = (vPosition * scale) + offset;
    vec4 texColor = texture(trailTexture, fract(uv)); // fract() creates repeating pattern
    
    // Combine with original color (preserves your alpha fading)
    FragColor = texColor * vColor;
}