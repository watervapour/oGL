#version 330 core
in vec3 ourColor;
out vec4 FragColor;
uniform float cyclic;


void main(){
    FragColor = vec4(0.05, cyclic, 0.5, 1.0);
}