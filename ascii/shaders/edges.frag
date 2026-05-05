#version 430 core

out vec4 frag_color;

in vec2 coord;

uniform vec2 u_res;
uniform float edge_thr;
uniform sampler2D sampler;   // scene
const float pi = 3.14159265359;
float lum(vec3 p){
	return dot(p, vec3(0.299, 0.587, 0.114));
}
float gaussian(float a, float b, float c, float x){
	return  a * exp(-((x - b)*(x - b) / (2 * c * c)));
}
// Quantize to 4 sectors
int quantizedEdge(float angle) {
    // angle in [0, PI)
    float a = angle / pi; // normalize to [0, 1)
    
    if (a < 0.125 || a >= 0.875){ 
		return 3;
	}
    else if (a < 0.375){
		return 2;
	}
    else if (a < 0.625){
		return 1;  
    }
	else {
		return 0;
	}
}
void main()
{
	vec2 texel = 1.f / u_res;
	float tl = lum(texture2D(sampler, coord - texel).rgb);
	float l = lum(texture2D(sampler, vec2(coord.x - texel.x, coord.y)).rgb);
	float bl = lum(texture2D(sampler, vec2(coord.x - texel.x, coord.y + texel.y)).rgb);
	float t = lum(texture2D(sampler, vec2(coord.x, coord.y - texel.y)).rgb);
	float m = lum(texture2D(sampler, coord).rgb);
	float b = lum(texture2D(sampler, vec2(coord.x, coord.y + texel.y)).rgb);
	float tr = lum(texture2D(sampler, vec2(coord.x + texel.x, coord.y - texel.y)).rgb);
	float r = lum(texture2D(sampler, vec2(coord.x + texel.x, coord.y)).rgb);
	float br = lum(texture2D(sampler, coord + texel).rgb);

	float gx = -tl - l * 2 - bl + tr + r * 2 + br;
	float gy = -tl - t * 2 - tr + bl + b * 2 + br;
	if (length(vec2(gx,gy) ) < edge_thr){
	frag_color = vec4(0, 0, 0, 0);
	
	}
	else{
		int dir = -1;
		float angle = atan(gy, gx); // range: [-PI, PI]

// Fold into [0, PI) — edges are direction-agnostic
		if (angle < 0.0) angle += pi;
		dir = quantizedEdge(angle);
		if (0 == dir){
			frag_color = vec4(1,0, 0, 1);
		}
		else if (1 == dir){
			frag_color = vec4(1,1, 0, 1);
			
		}
		else if (2 == dir){
			frag_color = vec4(0,0, 1, 1);
		}
		else if (3 == dir){
			frag_color = vec4(0,1, 0, 1);
		}
//		float angle = atan(gy, gx);
//		angle = angle / (2.0 * pi) + 0.5;
//		angle = clamp(angle, 0, 0.99);
//		angle = floor(angle * 10);
//		if ((0 == angle) || (4 == angle) || (5 == angle) || (9 == angle)){
//			dir = 0;
//			frag_color = vec4(1,0, 0, 1);
//		}
//		else if ((1 == angle) || (6 == angle)){
//			frag_color = vec4(1,1, 0, 1);
//			dir = 1;
//		}
//		else if ((2 == angle) || (7 == angle)){
//			frag_color = vec4(0,0, 1, 1);
//			dir = 2;
//		}
//		else if ((3 == angle) || (8 == angle)){
//			frag_color = vec4(0,1, 0, 1);
//			dir = 3;
//		}
//float angle = atan(gy, gx);
//
//// collapse opposite directions
//angle = mod(angle + pi, pi);
//
//// quantize into 4 sectors
//int dir = int(floor(angle / pi * 4.0)) % 4;
//
//if (dir == 0)
//{
//    frag_color = vec4(1,0,0,1);
//}
//else if (dir == 1)
//{
//    frag_color = vec4(1,1,0,1);
//}
//else if (dir == 2)
//{
//    frag_color = vec4(0,0,1,1);
//}
//else
//{
//    frag_color = vec4(0,1,0,1);
//}
//		frag_color = vec4(vec3(angle), 1);
	
	}
//	frag_color = floor(frag_color * 10)/10.f;

}