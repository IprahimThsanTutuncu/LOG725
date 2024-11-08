uniform sampler2D texture;
uniform vec3 directionalLight_direction;
uniform vec4 directionalLight_ambient;
uniform vec4 directionalLight_diffuse;
uniform vec4 directionalLight_specular;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if(pixel.a == 0.0)
        discard;

    vec3 L = directionalLight_direction;
    float diffuse = max(0.0, dot(vec3(0.0, 1.0, 0.0), L));
    pixel += directionalLight_specular*vec4(0.0); //absolute retarded. faut impl. un blinn-phong legit bro. ou juste phong
    pixel *= directionalLight_diffuse * diffuse;
    pixel += directionalLight_ambient * texture2D(texture, gl_TexCoord[0].xy);


    // multiply it by the color
    gl_FragColor = gl_Color * pixel;


}
