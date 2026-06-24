#version 430 core

in vec2 coord;
uniform vec2 res;
uniform ivec3 color_counts;
uniform sampler2D sampler;
uniform float spread;
out vec4 frag_color;
const int bayer2[2 * 2] = {
    0, 2,
    3, 1
};

const int bayer4[4 * 4] = {
    0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
};

const int bayer8[8 * 8] = {
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,  
    12, 44,  4, 36, 14, 46,  6, 38, 
    60, 28, 52, 20, 62, 30, 54, 22,  
    3, 35, 11, 43,  1, 33,  9, 41,  
    51, 19, 59, 27, 49, 17, 57, 25, 
    15, 47,  7, 39, 13, 45,  5, 37, 
    63, 31, 55, 23, 61, 29, 53, 21
};
float getbayer4(int x, int y){
    return bayer4[(x % 4) +( y % 4) * 4] * (1.0 / 16.0) - 0.5f;
}
void main(){
    vec3 color = texture(sampler, coord).rgb;
    int x = int(coord.x) * int(res.x);       	
    int y = int(coord.y) * int(res.y);
    float bayer = getbayer4(x, y);
    vec3 _output = color + spread * bayer;
//    _output.r = floor((color_counts.r - 1.0) * _output.r + 0.5) / (color_counts.r - 1.0);
//    _output.g = floor((color_counts.g - 1.0) * _output.g + 0.5) / (color_counts.g - 1.0);
//    _output.b = floor((color_counts.b - 1.0) * _output.b + 0.5) / (color_counts.b - 1.0);

    frag_color = vec4(_output, 1);

}