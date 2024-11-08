#version 130

uniform sampler2D texture;
uniform sampler2D texture_depth;

uniform vec3 directionalLight_direction;
uniform vec4 directionalLight_ambient;
uniform vec4 directionalLight_diffuse;
uniform vec4 directionalLight_specular;
uniform float time;
uniform float width;
uniform float height;
uniform float num_layer;

float offset = 1.0 / length(vec2(width, height));

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec3(.1031, .1030, .0973, .1099)

#define MAX_EFFECT 10
uniform int effects[MAX_EFFECT];
uniform float opacity[MAX_EFFECT];

// snow param
uniform float size_rate;
uniform float fg_layers;
uniform float bg_layers;
uniform float layer_gap;
uniform vec2 wind_direction;
uniform vec2 speed;

// letterbox param
uniform int type;
uniform int letterbox_blend; // value: none:0, additive:1, subtract:2, multiply:3

uniform int priority_letterbox[4]; // value -> top:0, right:1, bottom:2, left:3

uniform vec4 top_bar_color;
uniform vec4 bottom_bar_color;
uniform vec4 left_bar_color;
uniform vec4 right_bar_color;

uniform float bar_screen_ratio_top;
uniform float bar_screen_ratio_bottom;
uniform float bar_screen_ratio_left;
uniform float bar_screen_ratio_right;

uniform float fading_range_top;
uniform float fading_range_bottom;
uniform float fading_range_left;
uniform float fading_range_right;


// mix of these
// https://blog.csdn.net/tjw02241035621611/article/details/80047566
// https://www.shadertoy.com/view/3ld3zX

mat2 Rot(float a){
    float s = sin(a), c=cos(a);
    return mat2(c, -s, s, c);
}

//https://www.shadertoy.com/view/XtGfzw
float sdCross( in vec2 p, in vec2 b, float r ) 
{
    p = abs(p); p = (p.y>p.x) ? p.yx : p.xy;
    
	vec2  q = p - b;
    float k = max(q.y,q.x);
    vec2  w = (k>0.0) ? q : vec2(b.y-p.x,-k);
    float d = length(max(w,0.0));
    return ((k>0.0)?d:-d) + r;
}

float Hash11(float p)
{
	vec3 p3  = fract(vec3(p) * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z); 
}

float Hash21(vec2 p)
{
    p = fract(p*vec2(123.34, 456.21));
    p += dot(p, p+45.32);
    return fract(p.x*p.y);
}

vec2 Rand22(vec2 co)
{
    float x = fract(sin(dot(co.xy ,vec2(122.9898,783.233))) * 43758.5453);
    float y = fract(sin(dot(co.xy ,vec2(457.6537,537.2793))) * 37573.5913);
    return vec2(x,y);
}


float star(vec2 uv, float flare)
{
    float d = length(uv);
    float m = .05/d;// smoothstep(.1, .05, d);
  
    float rays = max(0., 1.-abs(uv.x*uv.y*1000.));
    m += rays*flare;
    
    uv *= Rot(3.1415/4.0);
    rays = max(0., 1.-abs(uv.x*uv.y*1000.));
    m += rays*0.3*flare;
    
    m *= smoothstep(1., .2, d);
    return m;
}

vec3 starLayer(vec2 uv, float scale)
{
    vec3 col = vec3(0.);
    vec2 gv = fract(uv)-0.5;
    vec2 id = floor(uv);

    //col.rg = gv;
    for(float y=-1.0;y<=1.0;y++){
         for(float x=-1.0;x<=1.0;x++){
             vec2 offset = vec2(x,y);
             float n = Hash21(id+offset);
              float size = fract(n*345.32)*scale;
              vec2 harmonic = vec2(cos(time)*0.4*fract(n*512.4), sin(time)*0.4*fract(n*398.4));
             float curr_star = star((gv-offset-vec2(n, fract(n*34.))+harmonic+.5), smoothstep(0.95, 1.0, size));
             vec3 color = vec3(1.0); //= sin(vec3(.5, .0, .9)*fract(n*34.)*123.)*0.5 + .5;
             //color = color*vec3(1.0,0.5,1.+size);
             col += curr_star*size*color;
         }
         
    }
//if(gv.x > 0.48 || gv.y > 0.48) col.r = 1.0;
// Output to screen

//col.rg += id* .04;
     return col;
}

float snow_flake(vec2 uv, float flare)
{
    float d = length(uv);
    float m = smoothstep(.3, .05, d); //.05/d;// 
  
    float rays = max(0., 1.-abs(uv.x*uv.y*1000.));
    //m += rays*flare;
    
    uv *= Rot(3.1415/4.0);
   // rays = max(0., 1.-abs(uv.x*uv.y*1000.));
   // m += rays*0.3*flare;
    
    m *= smoothstep(.5, .1, d);
    return m;
}

vec3 snow_farLayer(vec2 uv, float scale)
{
    vec3 col = vec3(0.);
    vec2 gv = fract(uv)-0.5;
    vec2 id = floor(uv);

    //col.rg = gv;
    for(float y=-1.0;y<=1.0;y++){
         for(float x=-1.0;x<=1.0;x++){
             vec2 offset = vec2(x,y);
             float n = Hash21(id+offset);
              float size = fract(n*345.32)*scale;

             float curr_flake = snow_flake((gv-offset-vec2(n, fract(n*332.))+.5), smoothstep(0.2, 1.0, size));
             vec3 color = sin(vec3(0.8,0.8,0.8)*fract(n*34.)*123.)*0.5 + .5;
             col += curr_flake*size*color;
         }
         
    }
//if(gv.x > 0.48 || gv.y > 0.48) col.r = 1.0;
// Output to screen

//col.rg += id* .04;
     return col;
}

vec3 snow_singleLayer(vec2 uv,float layer)
{
    
    vec3 acc = vec3(0.0,0.0,0.0);
    uv = uv * (2.0+layer);
    float xOffset = uv.y * (((Hash11(layer)*2.-1.)*0.5+1.)*speed.x);
    float yOffset = (speed.y*time);
    uv += vec2(xOffset,yOffset);
    vec2 rgrid = Rand22(floor(uv)+(31.1759*layer));
    uv = fract(uv);
    uv -= (rgrid*2.-1.0) * 0.35;
    uv -=0.5;
    float r = length(uv);

    float circleSize = 0.05*(1.0+0.3*sin(time*size_rate));
    float val = smoothstep(circleSize,-circleSize,r);
    vec3 col = vec3(val,val,val)* rgrid.x ;
    return col;
}

vec3 snow_background(vec3 fogColor)
{
 // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_TexCoord[0].xy;

    vec2 gv = gl_TexCoord[0].xy * 4.0;
    vec2 id = floor(gv);

    uv += wind_direction*time;

    float t = time *.1;
    //uv *= Rot(t);
    vec3 skyColor = directionalLight_ambient.rgb;
    float d = texture2D(texture_depth, gl_TexCoord[0].xy).r;

    for(float i = fg_layers + layer_gap; i<fg_layers + layer_gap + bg_layers; i+=1.){
        float depth = fract(i);
        float scale = mix(1.0, 0.1, depth);
        float fade = depth*smoothstep(1.0, 0.9, depth);
        fogColor += snow_singleLayer(uv, i); //skyColor += starLayer(uv*scale+i*5435.1)*fade;
        fogColor += starLayer(gv*scale+(1/(fg_layers+bg_layers+layer_gap-i+1.0))*431.1, 0.01); //skyColor += starLayer(uv*scale+i*5435.1)*fade;
    }
    float upperLimite = 0.7;
    float lowerLimite = 0.1;

    float factor = (upperLimite - gl_TexCoord[0].y) / (upperLimite - lowerLimite);
    factor = clamp(factor, 0.0, 1.0);
    //skyColor = vec3(fract(uv), 0.0);
    return fogColor.xyz*(1.0 - factor);
}

vec4 snow_foreGround(vec3 pixel)
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_TexCoord[0].xy;

    vec2 gv = gl_TexCoord[0].xy;
    vec2 id = floor(gv);
    gv += wind_direction * time;

    uv += wind_direction*time;

    float t = time *.1;
    //uv *= Rot(t);
    vec3 snow = vec3(0);
    float d = texture2D(texture_depth, gl_TexCoord[0].xy).r;

    for(float i = 0.; i<fg_layers; i+=1.){
        float depth = fract(i);
        float scale = mix(1.0, 0.1, depth);
        float fade = depth*smoothstep(1.0, 0.9, depth);
        snow += snow_singleLayer(uv, i); //skyColor += starLayer(uv*scale+i*5435.1)*fade;
        snow += starLayer(gv*scale+(1/(fg_layers-i+1.0))*431.1, 0.01); //skyColor += starLayer(uv*scale+i*5435.1)*fade;
    }

    //skyColor = vec3(fract(uv), 0.0);
    return vec4(snow, 1.0);
}

vec3 snowfall(bool isBorder)
{
    vec3 color = vec3(0.0); 
    if(isBorder)
        color = snow_background(color);
    color += snow_foreGround(color).rgb;
    return color;
}

vec3 letterbox_blend_func(vec3 first, vec3 second){
    vec3 color = first;
    if(letterbox_blend == 0);
    else if(letterbox_blend == 1)
        color += second;
    else if(letterbox_blend == 2)
        color -= second;
    else if(letterbox_blend == 3)
        color *= second;
    else if(letterbox_blend == 4){
        if(second.r != 0.0)
            color.r /= second.r;
        else
            color.r = 1.0;
        if(second.g != 0.0)
            color.g /= second.g;
        else
            color.g = 1.0;
        if(second.b != 0.0)
            color.b /= second.b;
        else
            color.b = 1.0;
    }
    else if(letterbox_blend == 5)
        color = min(color, second);
    else if(letterbox_blend == 6)
        color = min(color, second);
    return color;
}

vec3 letterbox(vec3 pixel)
{
    // lookup the pixel in the texture
    vec3 color = pixel;
	vec2 uv = gl_TexCoord[0].xy;

    if(type == 0){
        for(int i = 0; i < 4; i++){
            if(priority_letterbox[i] == 0)
                color = mix(letterbox_blend_func(color, top_bar_color.rgb), color, step(bar_screen_ratio_top, uv.y));
            else if(priority_letterbox[i] == 1)
                color = mix(letterbox_blend_func(color, right_bar_color.rgb), color, step(bar_screen_ratio_right, 1.0-uv.x));
            else if(priority_letterbox[i] == 2)
                color = mix(letterbox_blend_func(color, bottom_bar_color.rgb), color, step(bar_screen_ratio_bottom, 1.0-uv.y));
            else if(priority_letterbox[i] == 3)
                color = mix(letterbox_blend_func(color, left_bar_color.rgb), color, step(bar_screen_ratio_left, uv.x));
        }
    }

    else if(type == 1){
        for(int i = 0; i < 4; i++){
            if(priority_letterbox[i] == 0)
                color = mix(letterbox_blend_func(color, top_bar_color.rgb), color, smoothstep(bar_screen_ratio_top, bar_screen_ratio_top+fading_range_top, uv.y));
            else if(priority_letterbox[i] == 1)
                color = mix(letterbox_blend_func(color, right_bar_color.rgb), color, smoothstep(1.0-bar_screen_ratio_right,1.0-(bar_screen_ratio_right+fading_range_right), uv.x));
            else if(priority_letterbox[i] == 2)
                color = mix(letterbox_blend_func(color, bottom_bar_color.rgb), color, smoothstep(1.0-bar_screen_ratio_bottom,1.0-(bar_screen_ratio_bottom+fading_range_bottom), uv.y));
            else if(priority_letterbox[i] == 3)
                color = mix(letterbox_blend_func(color, left_bar_color.rgb), color, smoothstep(bar_screen_ratio_left, bar_screen_ratio_left+fading_range_left, uv.x));
        }
    }

    return color;
}

void main()
{
    // lookup the pixel in the texture
    vec3 pixel = texture2D(texture, gl_TexCoord[0].xy).rgb;

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float d = texture2D(texture_depth, gl_TexCoord[0].xy).r;

    float kernel_border[9] = float[](
        -1 , -1, -1,
        -1 ,  8, -1 ,
        -1 , -1 , -1 
    );

    float kernel_inside[9] = float[](
        0, -1, 0,
        -1,  5, -1,
        0, -1, 0
    );
    


    vec3 sampleTex[9];
    vec3 sampleTexDepth[9];

    bool isBorder = false;
    for(int i = 0; i < 9; i++){
        float offset_depth = texture2D(texture_depth, gl_TexCoord[0].xy + offsets[i]).r;
        vec4 offset_color = texture2D(texture, gl_TexCoord[0].xy + offsets[i]);

        if(d <= offset_depth && directionalLight_ambient.rgb == offset_color.rgb){
            isBorder = true;
        }
        sampleTex[i] = offset_color.rgb;
        if(offset_depth > 0.0)
            sampleTexDepth[i] = vec3(offset_depth);
        else
            sampleTexDepth[i] = vec3(1.0);
    }

    
    vec3 col = vec3(0.0);
    if(isBorder){
        for(int i = 0; i < 9; i++){
            col += sampleTex[i] * (1.0 - clamp(sampleTexDepth[i].r, 0.0, 1.0)) * kernel_border[i];
        }
    }


    float average_pix = ( pixel.r + pixel.g + pixel.b ) * 0.333;
    float average_col = ( col.r + col.g + col.b ) * 0.333;
    //pixel *= average_pix;
    //col *= average_col;
    for(int i = 0; i < MAX_EFFECT; i++){

        if(effects[i] == -1); // -1 means no effect at current slot
        else if(effects[i] == 0)
            pixel += opacity[effects[i]] * snowfall(isBorder).rgb;
        else if(effects[i] == 1){
            pixel = (pixel * (1.0 - opacity[effects[i]])) + (opacity[effects[i]] * letterbox(pixel));
        }
    }

    gl_FragColor = vec4(pixel.rgb, 1.0);

}