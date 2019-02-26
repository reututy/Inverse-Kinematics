#version 130


varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform sampler2D texture1;
uniform int type;

void main()
{
    vec3 tmp = color0;
    gl_FragColor = texture(texture1,texCoord0);

}
