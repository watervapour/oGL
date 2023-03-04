#version 330 core
struct Material {
    sampler2D texture_diffuse1;
};
in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;
uniform Material material;

out vec4 FragColor;
void main(){
    FragColor = vec4(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, 1.0);
}