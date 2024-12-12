uniform sampler2D texture;
uniform vec3 camera_position;
uniform vec4 fogColor;
uniform bool isSprite;
uniform bool isFixed;
void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    if(isSprite || isFixed)
        gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    else
        gl_TexCoord[0] = gl_MultiTexCoord0;


    gl_FrontColor =  gl_Color;
}
