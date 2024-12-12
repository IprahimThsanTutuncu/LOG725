uniform float time;
uniform vec2 size;

uniform float radius_top_left;
uniform float radius_top_right;
uniform float radius_bottom_left;
uniform float radius_bottom_right;

uniform vec4 color_border;
uniform vec4 color_borders_border;

uniform float top_border_size;
uniform float bottom_border_size;
uniform float left_border_size;
uniform float right_border_size;

uniform sampler2D image;
uniform bool enable_image;

uniform float speed;
uniform vec3 direction;
uniform float density;
uniform int iteration;

uniform int theme;

// from http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float udRoundBox( vec2 p, vec2 b, float r )
{
    return abs(length(max(abs(p)-b+r,0.0)))-r;
}

float nearestSeq(float val, float seq){
	float multiple = val/seq;
	float fractMultipe = fract(multiple);
	multiple -= fractMultipe;
	fractMultipe = round(fractMultipe);
	multiple += fractMultipe;
	return multiple * seq;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy*2.0 - vec2(1.0);

    vec4 color_border_darker = color_border * 0.7;

    vec2 half_size = size * 0.5;
    float r = min(size.x, size.y);
    vec2 border_size = vec2(0.0);

	//position is pixelized
	vec2 position;
	position.x = size.x*nearestSeq(gl_TexCoord[0].x, 0.005) - half_size.x;
	position.y = size.y*nearestSeq(gl_TexCoord[0].y, 0.005) - half_size.y;
    
    if(uv.x < 0.0 && uv.y > 0.0)
        r *= radius_bottom_left;
    else if(uv.x > 0.0 && uv.y > 0.0)
        r*= radius_bottom_right;
    else if(uv.x < 0.0 && uv.y < 0.0)
        r*= radius_top_left;
    else if(uv.x > 0.0 && uv.y < 0.0)
        r*= radius_top_right;
    


    if(uv.x < 0.0)
        border_size.x = left_border_size;
    else if(uv.x > 0.0)
        border_size.x = right_border_size;
    if(uv.y > 0.0)
        border_size.y = bottom_border_size;
    else if(uv.y < 0.0)
        border_size.y = top_border_size;

        
    float intensity = udRoundBox(position, half_size, r);
	//float border0 = udRoundBox(position, half_size + vec2(0.0) * border_size, r);
    float border1 = udRoundBox(position, half_size + vec2(-0.0) * border_size, r);
    float border2 = udRoundBox(position, half_size + vec2(-6.0) * border_size, r);
    float border3 = udRoundBox(position, half_size + vec2(-17.0) * border_size, r);
    float border4 = udRoundBox(position, half_size + vec2(-25.0) * border_size, r);
    float border5 = udRoundBox(position, half_size + vec2(-30.0) * border_size, r);
    
    gl_FragColor  = color_border_darker;
    
    gl_FragColor  = mix(gl_FragColor , color_border_darker, smoothstep(1.0, 0.0, border1));
    gl_FragColor  = mix(gl_FragColor , color_border, smoothstep(1.0, 0.0, border2));
	gl_FragColor  = mix(gl_FragColor ,color_border_darker, smoothstep(1.0, 0.0, border3));
	gl_FragColor  = mix(gl_FragColor ,vec4(color_border_darker.rgb*0.2, color_border_darker.a), smoothstep(1.0, 0.0, border4));

    
    //add the bg
	vec4 color_background = vec4(0.0, 0.0, 0.0, 0.0); // NONE

	if(theme == 1) // FILL
		color_background = gl_Color;
	else if(theme == 2){
		//noise and shit
	}
	
	 if(enable_image){
        vec4 img_color = texture(image, gl_TexCoord[0].xy);
        color_background = mix(color_background, img_color, img_color.a);
    }

    gl_FragColor  = mix(gl_FragColor , color_background, smoothstep(1.0, 0.0, border5));
    
    if(gl_FragColor .a >= 0.7){
       gl_FragColor .a = mix(1.0, 0.0 , smoothstep(0.0, 0., intensity));
    }
      
}
