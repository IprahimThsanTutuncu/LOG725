uniform sampler2D texture;
uniform sampler2D texture_depth;

uniform vec4 top_bar_color;
uniform vec4 bottom_bar_color;
uniform float bar_screen_ratio_top;
uniform float bar_screen_ratio_bottom;

uniform float elapsedTime;

const float offset = 1.0 / 300.0;  

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	vec2 uv = gl_TexCoord[0].xy;

	vec4 color = mix(
		bottom_bar_color, pixel, step(bar_screen_ratio_bottom, uv.y)
	);
	
	color = mix(
		color,top_bar_color , step(1.0 - bar_screen_ratio_top, uv.y)
	);

    gl_FragColor = color ;
}
