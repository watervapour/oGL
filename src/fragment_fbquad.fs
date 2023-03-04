#version 330 core
in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;

out vec4 FragColor;

const float offset = 1.0 / 300.0;
void main(){
    vec3 screenColor = texture(screenTexture, fs_in.TexCoords).rgb;
    FragColor = vec4(screenColor, 1.0);

    // inverted colors
    // vec3 invertedColor = vec3(1.0 - screenColor);
    // FragColor = vec4(invertedColor, 1.0);
    
    // greyscale
    // float average = (0.2126 * screenColor.r 
    //                 + 0.7152 * screenColor.g 
    //                 + 0.0722 * screenColor.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    float blurKernel[9] = float[](
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 1.0/16,
        1.0/16, 2.0/16, 1.0/16
    );
    float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    float edgeDetectKernel[9] = float[](
        1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );
    float sobelXKernel[9] = float[](
        1, 0, -1,
        2, 0, -2,
        1, 0, -1
    );
    float sobelYKernel[9] = float[](
         1,  2,  1,
         0,  0,  0,
        -1, -2, -1
    );
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0,     offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0),
        vec2( 0.0,     0.0),
        vec2( offset,  0.0),
        vec2(-offset, -offset),
        vec2( 0.0,    -offset),
        vec2( offset, -offset)
    );
    vec3 matrixColor = vec3(0.0);
    vec3 sobelX = vec3(0.0);
    vec3 sobelY = vec3(0.0);
    for(int i = 0; i < 9; i++){
        vec3 sampleTex = vec3(texture(screenTexture, fs_in.TexCoords.st + offsets[i]));
        matrixColor += sampleTex * sharpenKernel[i];
        //sobelX += sampleTex * sobelXKernel[i];
        //sobelY += sampleTex * sobelYKernel[i];
    }
    //matrixColor = sqrt(sobelX*sobelX + sobelY*sobelY);
    //FragColor = vec4(matrixColor, 1.0);
}