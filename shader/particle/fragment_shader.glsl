#version 330 core
out vec4 FragColor;

uniform vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
uniform vec3 baseColor = vec3(0.2, 0.6, 1.0);

void main() {
  vec2 coord = gl_PointCoord * 2.0 - 1.0;
  float r2 = dot(coord, coord);
  if(r2 > 1.0) {
    discard;
  }

  vec3 normal = normalize(vec3(coord, sqrt(1.0 - r2)));
  float diffuse = max(dot(normal, lightDir), 0.0);
  float ambient = 0.3;

  vec3 color = baseColor * (ambient + diffuse);
  FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}