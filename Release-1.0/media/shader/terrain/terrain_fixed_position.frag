uniform sampler2D texture;
uniform sampler2D texture_height;

uniform bool isXaxis;
uniform bool isYaxis;
uniform bool isZaxis;

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
    if(isXaxis){
        gl_FragColor =  vec4(floatToRgb(gl_TexCoord[0].x, 256.0), 1.0);
    }
    else if(isYaxis){
        vec3 fragPosition = vec3(gl_TexCoord[0].xy, texture2D(texture_height, gl_TexCoord[0].xy)).xzy;
        float y = fragPosition.y;
        gl_FragColor =  vec4(floatToRgb(y, 256.0), 1.0);
    }
    else if(isZaxis){
        gl_FragColor =  vec4(floatToRgb(gl_TexCoord[0].y, 256.0), 1.0);
    }
}
