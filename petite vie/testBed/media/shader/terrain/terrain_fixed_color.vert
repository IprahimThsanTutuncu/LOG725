uniform sampler2D texture;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_FrontColor =  gl_Color;
}
