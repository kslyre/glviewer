#version 120
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
attribute vec3 vertex;
attribute vec3 normal;
attribute vec4 view;
varying vec4 norm;
varying vec4 viewo;

void main(void)
{
    norm = modelViewMatrix * vec4(normal, 0.0);
    viewo = normalize(view);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1);
}
