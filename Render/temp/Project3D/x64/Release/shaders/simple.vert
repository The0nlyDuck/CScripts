// a simple shader 
#version 410 
 
layout( location = 0 ) in vec4 Position; 
layout( location = 1 ) in vec4 Normal;

out vec3 vNormal;
 
uniform mat4 ProjectionViewModel; 

// we need this matrix to transform the normal 
uniform mat4 ModelMatrix;

void main() { 
 vNormal = (ModelMatrix * Normal).xyz; 
 gl_Position = ProjectionViewModel * Position; 
} 