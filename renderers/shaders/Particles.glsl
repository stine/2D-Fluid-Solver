-- Vertex

uniform mat4 mvpMatrix;

in vec3 position;

void main() {
  // Transform position by modelview and projection matrices.
  gl_Position = mvpMatrix * vec4(position, 1.0);
} 

-- Fragment

uniform vec4 color;

out vec4 fragcolor;

void main() {
  fragcolor = vec4(1.0);
}
