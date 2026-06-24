#version 430 core

out vec4 frag_color;

in vec2 coord;

uniform sampler2D sampler;
void main(){
	frag_color.rgb = texture2D(sampler, vec2(coord.x ,1 - coord.y) ).rgb * 0.9;frag_color.a = 1.0;
}
//#version 430 core
//uniform sampler2D sampler;
//uniform float time;
//
//in vec2 coord;
//out vec4 frag_color;
//
//struct Ripple
//{
//    vec2 pos;
//    float startTime;
//};
//
//Ripple ripples[8];
//uniform int rippleCount;
//
//void main()
//{
//    vec2 uv = coord;
//
//    vec2 offset = vec2(0.0);
//    ripples[0].pos = vec2(0.5, 1);
//    ripples[1].pos = vec2(0.5, 0.1);
//    ripples[0].startTime = 0.5;
//    ripples[1].startTime = 1;
//    for(int i = 0; i < 2; i++)
//    {
//        float age = time - ripples[i].startTime;
//
//        if(age < 0.0)
//            continue;
//
//        vec2 delta = uv - ripples[i].pos;
//
//        float dist = length(delta);
//
//        float radius = age * 0.25;
//
//        float ringWidth = 0.03;
//
//        float ring =
//            1.0 -
//            smoothstep(
//                ringWidth,
//                ringWidth * 2.0,
//                abs(dist - radius)
//            );
//
//        float fade =
//            max(0.0, 1.0 - age * 0.25);
//
//        float wave =
//            sin((dist - radius) * 120.0);
//
//        if(dist > 0.0001)
//        {
//            offset +=
//                normalize(delta)
//                * wave
//                * ring
//                * fade
//                * 0.01;
//        }
//    }
//
//    //
//    // Pixelation
//    //
//
//    float pixelSize = 1.0 / 320.0;
//
//    offset =
//        floor(offset / pixelSize)
//        * pixelSize;
//
//    uv += offset;
//
//    frag_color = texture(sampler, uv);
//}