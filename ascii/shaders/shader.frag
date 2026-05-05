#version 430 core

out vec4 frag_color;

in vec2 coord;

uniform vec2 u_res;
uniform float char_size;

uniform sampler2D sampler;   // scene
uniform sampler2D sampler2;  // ascii atlas
uniform sampler2D sampler3;   // scene edges
uniform sampler2D sampler4;   // edges atlas

void main()
{
    vec2 cell = vec2(char_size);
    vec2 cells = u_res / cell;
    vec4 c = vec4(1, 1, 1, 1);
    vec2 cellUV = fract(coord * cells);
        float padding = 0.02;
    cellUV = mix(vec2(padding), vec2(1.0 - padding), cellUV);
    vec4 edges = texture(sampler3, coord);
        vec2 uv = floor(coord * cells) / cells;
        vec3 col = texture(sampler, uv).rgb;
	int dir;
	 if (vec4(1,0, 0, 1) == edges){
		dir = 2;
	}
	else if (vec4(1,1, 0, 1) == edges){
		dir = 1;
	}
	else if (vec4(0,0, 1, 1) == edges){
		dir = 3;
	}
	else if (vec4(0,1, 0, 1) == edges){
		dir = 0;
	}
	else{
		dir = -1;
	}
    if (edges.a < 0.99){
        float lum = pow(dot(col, vec3(0.299, 0.587, 0.114)), 1);
        lum = clamp(lum, 0.0, 0.999);
        int index = int(lum * 10.0);
        float cols = 10.0;
        float rows = 1.0;
        vec2 atlasUV;
        atlasUV.x = (float(index) + cellUV.x) / cols;
        atlasUV.y = cellUV.y / rows;
        c = texture(sampler2, atlasUV);
    }
    else{
        float cols = 5.0;
        float rows = 1.0;
        vec2 atlasUV;
        int index = 1 + dir;
        atlasUV.x = (float(index) + cellUV.x) / cols;
        atlasUV.y = cellUV.y / rows;
        c = texture(sampler4, atlasUV);
    }
    frag_color = c ;
}