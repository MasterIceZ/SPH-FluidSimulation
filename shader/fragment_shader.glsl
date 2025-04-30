#version 330 core
out vec4 FragColor;

uniform vec3 inputColor;
uniform vec2 resolution;

void main() {
  vec2 fragCoord = gl_FragCoord.xy / resolution;
  vec2 center = vec2(0.5, 0.5);
  float dist = distance(fragCoord, center);

  float gradient = smoothstep(0.0, 0.5, dist);
  vec3 color = mix(vec3(1.0, 1.0, 1.0), inputColor, gradient);

  FragColor = vec4(color, 1.0);
}