uniform sampler2D texture;


//world coordinate
uniform vec3 camera_position;

uniform float far;
uniform float terrain_size;
uniform float angle;

#define PI 3.1415926538

vec3 floatToRgb(float v, float scale) {
    float r = v;
    float g = mod(v*scale,1.0);
    r-= g/scale;
    float b = mod(v*scale*scale,1.0);
    g-=b/scale;
    return vec3(r,g,b);
}


void main()
{   
    float max_distance = far;

    vec2 p = camera_position.xz + vec2(terrain_size/2.0) - gl_TexCoord[0].xy*terrain_size;
    float n_angle = -angle + PI/2;
    vec2 n = vec2(cos(n_angle), sin(n_angle));
    vec2 q = dot(n,p)*n;
    float z = length(q);
	z /= far;

    gl_FragColor = vec4(floatToRgb(z, 256.0), 1.0);
}
