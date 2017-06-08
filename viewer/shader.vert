#version 120
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 v_texcoord;
attribute vec4 view;
varying vec2 texcoord;
varying vec4 norm;
varying vec4 viewo;

void main(void)
{
    norm = modelViewMatrix * vec4(normal, 0.0);
    viewo = normalize(view);
    texcoord = v_texcoord;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1);
}
