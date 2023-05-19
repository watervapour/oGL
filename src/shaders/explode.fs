#version 330 core
in vec2 TexCoords;

struct Material{
	sampler2D texture_diffuse1;
};

uniform Material material;

out vec4 FragColor;

void main(){
	vec2 newUVs = vec2(TexCoords.x, 1-TexCoords.y);
	FragColor = texture(material.texture_diffuse1, newUVs);
}
