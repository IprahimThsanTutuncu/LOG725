

void main()
{
    // transform the vertex position
	vec4 position = gl_Vertex;
	

    gl_Position = gl_ModelViewProjectionMatrix * position;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}