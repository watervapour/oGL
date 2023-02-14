#version 330 core
struct Material {
    sampler2D texture_diffuse1;
};
in vec2 TexCoords;
uniform Material material;

out vec4 FragColor;
void main(){
    FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0);
}