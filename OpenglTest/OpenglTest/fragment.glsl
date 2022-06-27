#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
//    FragColor = vec4(lightColor, 1.0) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    FragColor = vec4(lightColor * objectColor, 1.0);
}
