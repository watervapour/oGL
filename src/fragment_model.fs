#version 330 core

struct Material {
    sampler2D texture_diffuse1;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

void main(){
    FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0); ;
}