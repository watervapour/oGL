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
uniform vec3 cameraPos;
uniform samplerCube skybox;

out vec4 FragColor;

void main(){
    vec3 I = normalize(fs_in.FragPos - cameraPos);
    vec3 R = reflect(I, normalize(fs_in.Normal));
    //vec3 R = refract(I, normalize(fs_in.Normal), (1/1.52));
    
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}