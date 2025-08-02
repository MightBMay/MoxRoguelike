uniform vec2 cameraPos;
const float fadeRadius  = 900;
const vec2 resolution = vec2(1920.0,1080.0);
const float aspectRatio = 1.777777777777778;
const vec2 cameraZoom = resolution;

vec2 worldPos = cameraPos + vec2(
   (gl_FragCoord.x - 0.5 * resolution.x),
    -(gl_FragCoord.y - 0.5 * resolution.y)
);

void main(){

vec2 modifiedWorldPos = vec2(worldPos.x, worldPos.y*aspectRatio);
vec2 modifiedCamPos = vec2(cameraPos.x, cameraPos.y * aspectRatio);
float dist = distance(modifiedWorldPos, modifiedCamPos);
float a = smoothstep(fadeRadius,1920, dist);

gl_FragColor = vec4(0,0,0,a);
}