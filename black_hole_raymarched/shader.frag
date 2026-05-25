/*#version 330
//out vec4 frag_color;
//in vec4 color;
//in vec2 coord;
//uniform sampler2D sampler;
//
//struct Material{
//	vec3 color;
//};
//struct Hit{
//	float dist;
//	int mat_id;
//};
//float SphereSDF(vec4 sphere, vec3 pos){
//	return length(sphere.xyz - pos) - sphere.w;
//}
//Hit map(vec3 pos){
//	Hit hit0 = {SphereSDF(vec4(0, 0, 0, 0.25), pos), 0};
//	Hit hit1 = {SphereSDF(vec4(0.2, 0, 0, 0.25), pos), 1};
//	return hit0.dist < hit1.dist ? hit0 : hit1;
//}
//Hit RayMarch(vec3 ray_origin, vec3 ray_direction){
//	float trav = 0;
//	int mat_id = -1;
//	for (int i = 0; i < 100; i++){
//		vec3 pos = ray_origin + ray_direction * trav;
//		Hit d = map(pos);
//		trav += d.dist;
//		if (d.dist < 0.00001){
//			mat_id = d.mat_id;
//			break;
//		}
//		if (trav > 200.){
//			break;
//		}
//	}
//	return Hit(trav, mat_id);
//}
//void main() {
//	Material Materials[] = {
//		{vec3(1, 0, 0)},
//		{vec3(0, 1, 0)}
//	};
//	vec2 pos = coord * 2.0 - 1; 
//	vec3 ray_origin = vec3(0, 0, -1);
//	vec3 ray_direction = normalize(vec3(pos.xy, 1));
//	Hit hit = RayMarch(ray_origin, ray_direction);
//	vec3 bg = vec3(0, 0, 0);
//	frag_color = vec4(hit.mat_id == -1 ? bg : Materials[hit.mat_id].color, 1);
};*/


#version 330
out vec4 frag_color;
in vec4 color;
in vec2 coord;
uniform sampler2D sampler;
uniform float iTime;
struct Material{
	vec3 color;
};
struct Hit{
	float dist;
	int mat_id;
};
float SphereSDF(vec4 sphere, vec3 pos){
	return length(sphere.xyz - pos) - sphere.w;
}
vec2 dirToSpherical(vec3 d) {
    d = normalize(d);
    float theta = atan(d.z, d.x); // azimuth
    float phi = asin(clamp(d.y, -1.0, 1.0)); // elevation
    return vec2(theta, phi);
}
vec3 checkeredSky(vec3 rd) {
    vec2 sph = dirToSpherical(rd);
    float u = sph.x * 8.0; 
    float v = sph.y * 8.0; 
    float check = mod(floor(u) + floor(v), 2.0);
    vec3 colorA = vec3(0.2, 0.3, 0.6);
    vec3 colorB = vec3(0.8, 0.9, 1.0);
    return mix(colorA, colorB, check);
}
vec3 getCamera(float t) {
    float radius = 6.0;      // distance from black hole
    float speed = 0.5;       // rotation speed
    float angle = t * speed;
    float camX = 0;
    float camZ = -1 * radius;
    float camY = sin(angle) * 2;        // fixed height
    vec3 ro = vec3(camX, camY, camZ);
    vec3 target = vec3(0.0, 0.0, 0.0);
    vec3 forward = normalize(target - ro);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(forward, up));
    vec3 camUp = cross(right, forward);

    return vec3(ro.x, ro.y, ro.z); 
}
vec3 getRayDir(vec3 ro, vec3 target, vec2 uv) {
    vec3 forward = normalize(target - ro);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(forward, up));
    vec3 camUp = cross(right, forward);

    float fov = 1.4; 

    vec3 rd = normalize(forward + uv.x * right * fov + uv.y * camUp * fov);
    return rd;
}

vec3 RayWalk(vec3 ray_origin, inout vec3 ray_direction){
	float step_size = 0.08;
	float trav = 0;
float rs = 0.35;
float BHMass = rs * 1.6;
	for (int i = 0; i < 400; i++){
		vec3 pos = ray_origin + ray_direction * trav;
        float r = length(pos) + 0.001;
        float grav = BHMass / (r * r);

        ray_direction -= normalize(pos) * grav * step_size;
        ray_direction = normalize(ray_direction);
        step_size = mix(0.12, 0.015, smoothstep(0.4, 2.0, r));
        trav += step_size;
        float horizon = smoothstep(rs, rs + 0.08, r);
if (horizon < 0.01) return vec3(0.0);
//		trav += step_size;
//		vec3 toCenter = -normalize(pos);
//		if (SphereSDF(vec4(0, 0, 0, rs), pos) <= 0){
//			return vec3(0.0);
//		}
	}

	return checkeredSky(ray_direction);
}
vec3 renderBlackHole(vec3 ro, vec3 rd) {
    float stepSize = 0.02;
    float rs = 0.6;

    vec3 p = ro;

    for(int i = 0; i < 1000; i++) {
        float r = length(p);

        if(r < rs) {
            return vec3(0.0);
        }
        if (abs(p.y) < 0.05 && length(p.xz) < 4 && length(p.xz) > rs + 0.9)return mix(vec3(215.0 / 255.0, 62.0 / 255.0, 42.0 / 255.0), vec3(1, 161.0 / 255.0, 70.0 / 255.0), r / 4.0);
        if (SphereSDF(vec4(0, 0, 8, 0.6), p) <= 0)return vec3(253 / 255.0, 28 / 255.0, 29 / 255.0);
        vec3 toCenter = -normalize(p);
        float bend = rs / (r * r + 1.0);

        rd = normalize(rd + toCenter * bend * stepSize);

        p += rd * stepSize;

        if(length(p) > 50.0)
            break;
    }

    return vec3(51 / 255.0, 52 / 255.0, 63 / 255.0);
}
void main() {
	Material Materials[] = {
		{vec3(1, 0, 0)},
		{vec3(0, 1, 0)}
	};
	vec2 pos = coord * 2.0 - 1; 
    float t = iTime;
    vec3 ro = getCamera(t);
    vec3 target = vec3(0.0, 0.0, 0.0);
    vec3 rd = getRayDir(ro, target, pos);
	vec3 ray_origin = ro;
	vec3 ray_direction = normalize(rd);
	vec3 hit = renderBlackHole(ray_origin, ray_direction);
	frag_color = vec4(hit, 1);
};