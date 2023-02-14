#version 330 core
struct Material {
    sampler2D texture_diffuse1;
};

in vec3 FragPos;
in vec3 Normal;
uniform Material material;
uniform vec3 cameraPos;
uniform samplerCube skybox;

out vec4 FragColor;

void main(){
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    //vec3 R = refract(I, normalize(Normal), (1/1.52));
    
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}