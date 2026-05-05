#version 430 core

out vec4 frag_color;

in vec2 coord;
uniform float r;
uniform sampler2D sampler;
uniform sampler2D sampler1;
float lum(vec3 p){
	return dot(p, vec3(0.299, 0.587, 0.114));
}
void main(){
	float dog = (1 + r) * texture2D(sampler1, coord).r - r * texture2D(sampler, coord).r;
	if (dog > 0.15)
		frag_color = vec4(1);
	else{
		frag_color = vec4(0, 0, 0, 1);
	}
}