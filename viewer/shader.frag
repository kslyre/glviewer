#version 120
uniform vec4 color;
varying vec4 norm;
varying vec4 viewo;

void main(void)
{
    //vec3 normal = modelViewMatrix * normals;
    vec4 reflected = reflect(-normalize(viewo), norm);
    //vec3 r = 2*dot(normalize(view), normal)*normal - view;
    vec4 spec = 0.5 * 0.2 * pow(dot(viewo, reflected), 0.4) * vec4(1,1,1,1); //normalize(view), reflected
    vec4 diff = dot(viewo, norm)*vec4(0,0,0,1);
    gl_FragColor = color + spec;
}
