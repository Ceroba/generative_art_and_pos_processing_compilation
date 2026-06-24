#version 430 core
precision highp float;
out vec4 frag_color;

in vec2 coord;
uniform sampler2D sampler;
uniform sampler2D sampler2;

void main(){
	vec3 color = texture(sampler,coord).rgb;
	vec3 blured = texture(sampler2,coord).rgb;
	frag_color = vec4(1.0f * blured + color, 1);
}