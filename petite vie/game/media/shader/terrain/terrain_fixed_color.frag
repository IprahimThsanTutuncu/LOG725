uniform sampler2D texture;

void main()
{   
    vec4 pixel = gl_Color;
    if(pixel.a == 0.0)
        discard;
    gl_FragColor = vec4(pixel.rgb, 1.0);
}
