#version 330 core

struct Material {
    sampler2D tex;
};

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


out vec4 FragColor;
out float gl_FragDepth;
uniform Material material;

void main(){
    vec4 texColor = texture(material.tex, fs_in.TexCoords);
    FragColor = texColor;
    if (gl_FrontFacing){
        if (gl_FragCoord.x < 400){
            FragColor += vec4(0.0, 0.0, 0.4, 0.2);
        } else {
            FragColor += vec4(-0.5, 0.4, 0.0, 0.7);
        }
        
    }
}