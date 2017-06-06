uniform vec4 color;
attribute vec3 view;
attribute vec3 normal;

void main(void)
{
    vec3 reflected = reflect(-normalize(view)*normal);
    float Ispec = 0.5 * 0.3 * pow(dot(view*reflected), 8);
    gl_FragColor = color;
}
