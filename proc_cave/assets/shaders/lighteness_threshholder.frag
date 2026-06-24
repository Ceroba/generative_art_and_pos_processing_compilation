#version 430 core
out vec4 frag_color;
in vec2 coord;
uniform sampler2D sampler;
uniform float threshhold;


void main(){
	vec4 color = texture(sampler, coord);
	float lightness = dot(color.rgb, vec3(0.299f, 0.587f, 0.114f));
	if (lightness > 0.55){
		frag_color = color;
	}
	else{
		frag_color = vec4(0, 0, 0, 0);
	}
}

