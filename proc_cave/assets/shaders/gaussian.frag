#version 430 core
precision highp float;
out vec4 frag_color;

in vec2 coord;

uniform vec2 u_res;
uniform int virtical_pass;
uniform float standered_div;
uniform int rad;
uniform sampler2D sampler;   // scene
float gaussian(float x, float c){
	return  exp(-((x * x) / (2 * c * c)));
}
float lum(vec3 p){
	return dot(p, vec3(0.299, 0.587, 0.114));
}
void main(){
	vec2 texel = 1.0 / u_res;
	vec3 avg = vec3(0);
	float tot = 0;
	if (virtical_pass == 1){
		for (int i = -rad; i <= rad; ++i){
			float w = gaussian(float(i),standered_div);
			vec3 col = w * texture2D(sampler, vec2(coord.x, coord.y + texel.y * i)).rgb; 
			avg += col;
			tot += w;
		}
	}
	else {
		for (int i = -rad; i <= rad; ++i){
			float w = gaussian(float(i),standered_div);
			vec3 col = w * texture2D(sampler, vec2(coord.x + texel.x * i, coord.y)).rgb; 
			avg += col;
			tot += w;
		}
	}
	avg /= tot;
	frag_color = vec4(avg, 1.0);

}