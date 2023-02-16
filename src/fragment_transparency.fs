#version 330 core

struct Material {
    sampler2D tex;
};

in vec2 TexCoords;

out vec4 FragColor;
out float gl_FragDepth;
uniform Material material;

void main(){
    gl_FragDepth = gl_FragCoord.z;
    vec4 texColor = texture(material.tex, TexCoords);
    FragColor = texColor;
    if (gl_FrontFacing){
        if (gl_FragCoord.x < 400){
            FragColor += vec4(0.0, 0.0, 0.4, 0.2);
            gl_FragDepth += 0.01;
        } else {
            FragColor += vec4(-0.5, 0.4, 0.0, 0.7);
            gl_FragDepth -= 0.01;
        }
        
    }
}