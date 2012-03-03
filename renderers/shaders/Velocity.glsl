-- Vertex

#version 130
uniform mat4 mvpMatrix;
in vec2 position;
in vec2 velocity;
void main() {
  // Transform position by modelview and projection matrices.
  gl_Position = mvpMatrix * vec4(position, 1.0, 1.0);
} 


-- Geometry

#version 130


-- Fragment

#version 130
out vec4 fragcolor;
void main() {
  fragcolor = vec4(1.0);
}
