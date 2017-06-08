#version 120
uniform vec4 color;
uniform sampler2D texture;
varying vec2 texcoord;
varying vec4 norm;
varying vec4 viewo;

void main(void)
{
    vec4 reflected = reflect(-viewo, norm);
    //vec3 r = 2*dot(normalize(view), normal)*normal - view;
    vec4 spec = 0.4 * pow(dot(viewo, reflected), 1) * vec4(1,1,1,1);
    //spec = clamp(spec, 0, 1);
    vec4 diff = 0.1*dot(viewo, norm)*vec4(0.1,0.1,0.1,1);
    //diff = clamp(diff, 0, 1);

    gl_FragColor = texture2D(texture, texcoord) + color + diff + spec;
}
