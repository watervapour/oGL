#version 330 core

struct Material {
    sampler2D tex;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

void main(){
    vec4 texColor = texture(material.tex, TexCoords);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor;
}