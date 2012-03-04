-- Vertex

#version 130
uniform mat4 mvpMatrix;
in vec2 position;
void main() {
  // Pass the object-space coordinate to the geometry shader.
  gl_Position = mvpMatrix * vec4(position, 0.0, 1.0);
} 


-- Geometry

#version 130
#extension GL_EXT_geometry_shader4 : enable
uniform mat4 mvpMatrix;
uniform float cellWidth;
void main() {
  // Use four offset vertices from the centerpoint to create the cell face.
  float halfWidth = cellWidth / 2;
  gl_Position = gl_PositionIn[0] + mvpMatrix * vec4(-halfWidth, -halfWidth, 0.0, 1.0);
  EmitVertex();
  gl_Position = gl_PositionIn[0] + mvpMatrix * vec4( halfWidth, -halfWidth, 0.0, 1.0);
  EmitVertex();
  gl_Position = gl_PositionIn[0] + mvpMatrix * vec4(-halfWidth,  halfWidth, 0.0, 1.0);
  EmitVertex();
  gl_Position = gl_PositionIn[0] + mvpMatrix * vec4( halfWidth,  halfWidth, 0.0, 1.0);
  EmitVertex();
}


-- Fragment

#version 130
uniform vec4 color;
out vec4 fragcolor;
void main() {
  fragcolor = color;
}
