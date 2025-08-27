// No version directive - let SFML handle it
uniform vec2 viewportPosition;
uniform vec2 viewportSize;
uniform float fadeWidth;
uniform sampler2D texture;
void main()
{
    vec2 viewportScreenMin = viewportPosition;
    vec2 viewportScreenMax = viewportPosition + viewportSize;
    
    float leftDist = gl_FragCoord.x - viewportScreenMin.x;
    float rightDist = viewportScreenMax.x - gl_FragCoord.x;
    
    // Calculate fade from both edges
    float leftFade = clamp(leftDist / fadeWidth, 0.0, 1.0);
    float rightFade = clamp(rightDist / fadeWidth, 0.0, 1.0);
    
    // Combine both fades (multiplication gives smoother result)
    float alpha = leftFade * rightFade;
    
    // Output as grayscale
    vec4 color = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = vec4(color.rgb, color.a * alpha);

}
