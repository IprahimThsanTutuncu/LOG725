#version 130

uniform sampler2D texture;
uniform sampler2D texture_depth;

uniform float time;
uniform float width;
uniform float height;

float offset = 1.0 / length(vec2(width, height));

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec3(.1031, .1030, .0973, .1099)

/*
type:
    0 - fade_to_color
    1 - left_to_right
    2 - right_to_left
    3 - left_right_to_center
    4 - top_to_bottom
    5 - bottom_to_top
    6 - top_bottom_to_center
    7 - zoom_blur_to_color
*/
uniform int transition_type;

//t as in t in interpolation, f(t) = p*t + (1.0 - t)*q
uniform float transition_t;

uniform vec4 transition_color1;
uniform vec4 transition_color2;

uniform int fading_type; // 0: step, 1:smoothstep

vec4 interpolate_color(vec4 color1, vec4 color2, float t){
    return color1 * (1.0 - t) + color2 * t;
}

vec4 fade(vec4 source, vec4 destination, float t){
    return interpolate_color(source, destination, t);
}

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = vec4(texture2D(texture, gl_TexCoord[0].xy).rgb, 1.0);
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    if(transition_type == 0)
        color = fade(pixel, transition_color1, transition_t);
    
    
    gl_FragColor = vec4(pixel.rgb*(1.0-transition_t) + transition_color1.rgb*transition_t, 1.0);

}