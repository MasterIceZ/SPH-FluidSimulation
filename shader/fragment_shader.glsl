#version 330 core
in float edgeFactor;
out vec4 FragColor;

uniform vec3 inputColor;

void main() {
  float intensity = smoothstep(0.0, 1.0, edgeFactor);
  vec3 color = mix(inputColor, vec3(1.0, 1.0, 1.0), intensity);
  
  FragColor = vec4(inputColor, 1.0);
}