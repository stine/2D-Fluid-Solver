-- Vertex

#version 150
in  vec2 position;
in  vec2 velocity;
out vec2 vPosition;
out vec2 vVelocity;
void main() {
  // Pass along position and velocity values.
  vPosition = position;
  vVelocity = velocity;
} 


-- Geometry

#version 150
uniform mat4 mvpMatrix;
uniform vec2 maxVelocity;
uniform float cellWidth;
in  vec2 vPosition[];
in  vec2 vVelocity[];
out vec4 gColor;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
vec3 getJetColor(float value) {
  float fourValue = 4*value;
  float red   = min(fourValue - 1.5, -fourValue + 4.5);
  float green = min(fourValue - 0.5, -fourValue + 3.5);
  float blue  = min(fourValue + 0.5, -fourValue + 2.5);
  return clamp( vec3(red, green, blue), 0.0, 1.0 );
}
void main() {
  // Calculate color from this velocity's magnitude relative to the max in the sim.
  float velRatio = abs(length(vVelocity[0]) / length(maxVelocity));
  gColor = vec4(getJetColor(velRatio), 0.5);
  
  // Calculate four vertices to create an 'arrow'.
  vec2 velUnit = normalize(vVelocity[0]);
  float thick  = cellWidth * 0.5;
  float thin   = cellWidth * 0.05;
  vec2 offsets[4];
  offsets[0] = velUnit * velRatio * thick;
  offsets[1] = vec2(cross(vec3(velUnit, 0.0), vec3(0.0, 0.0, 1.0))) * velRatio * thin;
  offsets[2] = -offsets[1];
  offsets[3] = -velUnit * velRatio * thin;
  
  for(int i = 0; i < 4; ++i) {
    gl_Position = mvpMatrix * vec4(vPosition[0] + offsets[i], 0.0, 1.0);
    EmitVertex();
  }
}


-- Fragment

#version 150
in  vec4 gColor;
out vec4 fragcolor;
void main() {
  fragcolor = gColor;
}
