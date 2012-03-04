-- Vertex

#version 130
in vec2 position;
out vec2 vPosition;
void main() {
  // Pass the object-space coordinate to the geometry shader.
  vPosition = position;
} 


-- Geometry

#version 130
uniform mat4 mvpMatrix;
uniform vec1 cellWidth;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
in vec2 vPosition[];
void main() {
  // Use four offset vertices from the centerpoint to create the cell face.
  float halfWidth = cellWidth / 2;
  gl_Position = mvpMatrix * (vPosition[0] + vec2(-halfWidth, -halfWidth));
  EmitVertex();
  gl_Position = mvpMatrix * (vPosition[0] + vec2( halfWidth, -halfWidth));
  EmitVertex();
  gl_Position = mvpMatrix * (vPosition[0] + vec2( halfWidth,  halfWidth));
  EmitVertex();
  gl_Position = mvpMatrix * (vPosition[0] + vec2(-halfWidth,  halfWidth));
  EmitVertex();
}


-- Fragment

#version 130
uniform vec4 color;
out vec4 fragcolor;
void main() {
  fragcolor = color;
}
