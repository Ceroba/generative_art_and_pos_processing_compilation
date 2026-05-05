#version 430 core

out vec4 frag_color;

in vec2 coord;

uniform vec2 u_res;
uniform float char_size;
uniform int virtical_pass;
uniform float standered_div;
uniform int rad;
uniform sampler2D sampler;
void main(){
	frag_color = texture2D(sampler, coord);
}