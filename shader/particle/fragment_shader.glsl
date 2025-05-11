#version 330 core
in vec3 particlePosition;
out vec4 FragColor;

uniform vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
uniform vec3 minPosition = vec3(-0.5, -0.5, -0.5);
uniform vec3 maxPosition = vec3(0.5, 0.5, 0.5);

void main() {
  vec2 coord = gl_PointCoord * 2.0 - 1.0;
  float r2 = dot(coord, coord);
  if(r2 > 1.0) {
    discard;
  }

  vec3 normal = normalize(vec3(coord, sqrt(1.0 - r2)));
  float diffuse = max(dot(normal, lightDir), 0.0);
  float ambient = 0.3;

  vec3 normalizedPosition = (particlePosition - minPosition) / (maxPosition - minPosition);
  vec3 gradientColor = vec3(normalizedPosition.x, normalizedPosition.y, normalizedPosition.z);

  vec3 color = gradientColor * (ambient + diffuse);
  FragColor = vec4(color, 1.0);
}