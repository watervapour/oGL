#version 330 core
in vec2 TexCoords;

struct Material{
	sampler2D texture_diffuse1;
};

uniform Material material;

out vec4 FragColor;

void main(){
	FragColor = texture(material.texture_diffuse1, TexCoords);
}
