-- Vertex

#version 130
uniform mat4 mvpMatrix;
in vec3 position;
void main() {
  // Transform position by modelview and projection matrices.
  gl_Position = mvpMatrix * vec4(position, 1.0);
} 


-- Fragment

#version 130
uniform vec4 color;
out vec4 fragcolor;
void main() {
  fragcolor = color;
}
