#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main(){
    vec4 C1 = mix(texture(tex1, TexCoord),
                    texture(tex2, TexCoord), 0.2);
    FragColor = mix(C1, vec4(ourColor, 1.0), 0.5);
}