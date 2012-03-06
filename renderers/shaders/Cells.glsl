-- Vertex

#version 150
in vec2 position;
void main() {
  // Pass the object-space coordinate to the geometry shader.
  gl_Position = vec4(position, 0.0, 1.0);
} 


-- Geometry

#version 150
uniform mat4 mvpMatrix;
uniform float cellWidth;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
void main() {
  // Use four offset vertices from the centerpoint to create the cell face.
  vec4  offset;
  float halfWidth = cellWidth / 2;

  offset = vec4(-halfWidth, -halfWidth, 0.0, 1.0);
  gl_Position = mvpMatrix * (gl_in[0].gl_Position + offset);
  EmitVertex();

  offset = vec4(halfWidth, -halfWidth, 0.0, 1.0);
  gl_Position = mvpMatrix * (gl_in[0].gl_Position + offset);
  EmitVertex();

  offset = vec4(-halfWidth, halfWidth, 0.0, 1.0);
  gl_Position = mvpMatrix * (gl_in[0].gl_Position + offset);
  EmitVertex();

  offset = vec4(halfWidth, halfWidth, 0.0, 1.0);
  gl_Position = mvpMatrix * (gl_in[0].gl_Position + offset);
  EmitVertex();
}


-- Fragment

#version 150
uniform vec4 color;
out vec4 fragcolor;
void main() {
  // TODO color
  fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
}
