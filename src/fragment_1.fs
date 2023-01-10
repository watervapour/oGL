#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 lightColor;

void main(){
    vec4 C1 = mix(texture(tex1, TexCoord),
                    texture(tex2, TexCoord), 0.2);
    FragColor = C1 * vec4(lightColor, 1.0); ;
}