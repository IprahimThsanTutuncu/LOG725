uniform sampler2D texture;
uniform sampler2D texture_height;
uniform sampler2D texture_depth;
uniform sampler2D texture_depthTerrain;
uniform sampler2D texture_sprites;
uniform sampler2D texture_normalmap;
uniform sampler2D texture_Xaxis;
uniform sampler2D texture_Yaxis;
uniform sampler2D texture_Zaxis;

uniform sampler2D sprite_emissionTexture;
uniform sampler2D sprite_depthTexture;
uniform bool has_sprite_depthTexture;
uniform bool has_sprite_emissionTexture;

uniform vec3 directionalLight_direction;
uniform vec4 directionalLight_ambient;
uniform vec4 directionalLight_diffuse;
uniform vec4 directionalLight_specular;

//world coordinate
uniform vec3 camera_position;
uniform vec3 sprite_position;


//data for sprite inside the spritesheet
uniform vec2 sprite_texCoord_size;
uniform vec2 sprite_scale;
uniform vec2 sprite_texCoord_position;
uniform vec2 spriteSheet_size;
uniform vec4 sprite_key_color;
uniform float sprite_rotation;
uniform float sprite_depth_factor;

uniform vec2 NDC_sprite_position;
uniform vec4 fogColor;
uniform float time;
uniform float far;
uniform float terrain_size;
uniform float terrainMaxElevation;
uniform float angle;

//uniform float sprite_z;
uniform bool isSprite;
uniform bool isDepth;
uniform bool isFixedPreRender;
uniform bool isFixed;
uniform bool isXaxis;
uniform bool isYaxis;
uniform bool isZaxis;

uniform float width;
uniform float height;
uniform float scale_height;

uniform float normal_blur_width_factor;
uniform float normal_blur_height_factor;
uniform float normal_blur_iteration;
uniform float shininess;
uniform bool isFlipX;
uniform bool isFlipY;

uniform bool is_debug_normal;
uniform bool is_debug_position;
uniform bool is_debug_normalized_scale;
uniform bool is_debug_position_normalized_scale;
uniform bool is_debug_depth;
uniform bool is_debug_none;

uniform int blur_type;

//pointlight
struct PointLight{
    vec3 position;  
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	
    float constant;
    float linear;
    float quadratic;
};

#define NB_POINT_LIGHTS 50
uniform PointLight pointLights[NB_POINT_LIGHTS];
uniform int pointLightCount;

// à cosnidérer
struct AreaLight{
    vec3 position;  
    mat3 basis;
    vec2 size;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	
    float constant;
    float linear;
    float quadratic;
};

#define NB_AREA_LIGHTS 1
uniform AreaLight areaLights[NB_AREA_LIGHTS];
uniform int areaLightCount;

//from stackoverflow by Makers_F
//https://stackoverflow.com/questions/6893302/decode-rgb-value-to-single-float-without-bit-shift-in-glsl

const vec4 bitEnc = vec4(1.,255.,65025.,16581375.);
const vec4 bitDec = 1./bitEnc;

float w = terrain_size;
float h = terrain_size;

#define PI 3.1415926538

vec4 EncodeFloatRGBA (float v) {
    vec4 enc = bitEnc * v;
    enc = fract(enc);
    enc -= enc.yzww * vec2(1./255., 0.).xxxy;
    return enc;
}
float DecodeFloatRGBA (vec4 v) {
    return dot(v, bitDec);
}

float packColor(vec3 color) {
    return color.r + color.g * 256.0 + color.b * 256.0 * 256.0;
}

vec3 unpackColor(float f) {
    vec3 color;
    color.b = floor(f / 256.0 / 256.0);
    color.g = floor((f - color.b * 256.0 * 256.0) / 256.0);
    color.r = floor(f - color.b * 256.0 * 256.0 - color.g * 256.0);
    // now we have a vec3 with the 3 components in range [0..255]. Let's normalize it!
    return color / 255.0;
}

float rgbToFloat(vec3 rgb, float scale){
    return rgb.r +
        (rgb.g/scale)+
        (rgb.b/(scale*scale));
}

vec3 floatToRgb(float v, float scale) {
    float r = v;
    float g = mod(v*scale,1.0);
    r-= g/scale;
    float b = mod(v*scale*scale,1.0);
    g-=b/scale;
    return vec3(r,g,b);
}

vec4 calcDirectionalLight(vec3 normal, vec3 position){
    vec4 result;
    vec3 L = -directionalLight_direction;
    float diffuse = max(0.0, dot(normal, L));


    vec3 fragPosition = position;
    fragPosition.x *= terrain_size;
    fragPosition.z *= terrain_size;
    fragPosition.y *= scale_height;

	fragPosition.x -= terrain_size/2.0;
	fragPosition.z -= terrain_size/2.0;

    fragPosition.x /= terrain_size;
	fragPosition.y /= scale_height;
	fragPosition.z /= terrain_size;

    vec3 cam_pos = camera_position;
    cam_pos.x /= terrain_size;
	cam_pos.y /= scale_height;
	cam_pos.z /= terrain_size;

    //speculars
    vec3 viewDir    = normalize(camera_position - position);
    vec3 halfwayDir = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec4 diffuse_color = directionalLight_diffuse * diffuse * texture2D(texture, gl_TexCoord[0].xy);
    vec4 ambient_color = directionalLight_ambient * texture2D(texture, gl_TexCoord[0].xy);
    vec4 specular_color = directionalLight_specular * spec * texture2D(texture, gl_TexCoord[0].xy);
    result = ambient_color + diffuse_color + specular_color;
    return result;

}


vec4 calcDirectionalLight_dynamique(vec3 normal, vec3 position){
    vec4 result;
    vec3 L = -directionalLight_direction;
    float diffuse = max(0.0, dot(normal, L));


    vec3 fragPosition = position;
    fragPosition.x *= terrain_size;
    fragPosition.z *= terrain_size;
    fragPosition.y *= scale_height;

	fragPosition.x -= terrain_size/2.0;
	fragPosition.z -= terrain_size/2.0;

    fragPosition.x /= terrain_size;
	fragPosition.y /= scale_height;
	fragPosition.z /= terrain_size;

    vec3 cam_pos = camera_position;
    cam_pos.x /= terrain_size;
	cam_pos.y /= scale_height;
	cam_pos.z /= terrain_size;

    //speculars
    vec3 viewDir    = normalize(camera_position - position);
    vec3 halfwayDir = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec4 diffuse_color = directionalLight_diffuse * diffuse * gl_Color;
    vec4 ambient_color = directionalLight_ambient * gl_Color;
    vec4 specular_color = directionalLight_specular * spec * gl_Color;
    result = ambient_color + diffuse_color + specular_color;
    return result;

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, float shininess)
{
	vec3 position = light.position;
	vec3 fragPosition = fragPos;
    fragPosition.x *= terrain_size;
    fragPosition.z *= terrain_size;
    fragPosition.y *= scale_height;

	fragPosition.x -= terrain_size/2.0;
	fragPosition.z -= terrain_size/2.0;

	position.x /= terrain_size;
	position.y /= scale_height;
	position.z /= terrain_size;

	fragPosition.x /= terrain_size;
	fragPosition.y /= scale_height;
	fragPosition.z /= terrain_size;

    vec3 cam_pos = camera_position;
    cam_pos.x /= terrain_size;
	cam_pos.y /= scale_height;
	cam_pos.z /= terrain_size;

    vec3 lightDir = normalize(position - fragPosition);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    //speculars
    vec3 viewDir    = normalize(cam_pos - fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // attenuation
    float distance    = length(position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec4 ambient  = light.ambient;
    vec4 diffuse  = light.diffuse  * diff;
    vec4 specular = light.specular * spec;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //return vec3(distance(fragPosition.z/terrain_size, position.z/terrain_size));
    return (ambient.rgb + diffuse.rgb + specular.rgb);
}

mat2 rotationMatrix(float angle)
{
	angle *= 3.14159 / 180.0;
    float s=sin(angle), c=cos(angle);
    return mat2( c, -s, s, c );
}

vec2 rotate(vec2 uv, float th) {
    return mat2(cos(th), sin(th), -sin(th), cos(th)) * uv;
}

/*
            vec2 offset = sprite_texCoord_position_normalize;
            float x = gl_TexCoord[0].x - offset.x;
            float y = gl_TexCoord[0].y - offset.y;
			vec2 normalized_coord = rotate(vec2(x,y), -sprite_rotation*3.1459/180.0) / sprite_texCoord_size_normalize;
*/

vec3 calculate_normal(vec2 texel){
    float tx = 1.0 / w;
    float ty = 1.0 / h;
    float hL = texture2D(texture_height, texel - vec2(-tx, 0.0)).r;
    float hR = texture2D(texture_height, texel - vec2(tx, 0.0)).r;
    float hD = texture2D(texture_height, texel - vec2(0.0, ty)).r;
    float hU = texture2D(texture_height, texel - vec2(0.0, -ty)).r;

    vec3 normal;
    normal.x = hL - hR;
    normal.y = (1.0 / scale_height);
    normal.z = hD - hU;
    normal = normalize(normal);

    return normal;
}

vec3 retrieve_normal(vec2 texel){
    vec3 normal = texture2D(texture_normalmap, texel).xyz;
    normal -= vec3(0.5,0.0,0.5);
    normal = normalize(normal);
    return normal;
}

vec3 blur_normal(vec3 normal, vec3 fragPosition){
    
    if(blur_type == 1){
            for(float i = 1.0; i < normal_blur_iteration+1.0; i++){                       
                float tx = (1.0/(w*normal_blur_width_factor)) * i; 
                float ty = (1.0/(h*normal_blur_height_factor)) * i;
                vec3 normalU = retrieve_normal(fragPosition.xz - vec2(-tx, 0.0)); // *2 c'est le scalaire du blur de la normal en W. en faire un param.
                vec3 normalD = retrieve_normal(fragPosition.xz - vec2(tx, 0.0));
                vec3 normalL = retrieve_normal(fragPosition.xz - vec2(0.0, ty));
                vec3 normalR = retrieve_normal(fragPosition.xz - vec2(0.0, ty));
                vec3 temp_normal = vec3(normalU + normalD + normalL + normalR) * 0.25;
                normal += normalize(temp_normal);
            }

            normal *= 1.0/(normal_blur_iteration+1.0);
            normal = normalize(normal);


    }else if(blur_type == 2){
        for(float i = 1.0; i < normal_blur_iteration+1.0; i++){                       
            float tx = (1.0/(width*normal_blur_width_factor)) * i; 
            float ty = (1.0/(h*normal_blur_height_factor)) * i;
            vec3 normalL = texture2D(texture_normalmap, fragPosition.xz - vec2(-tx, 0.0)).xyz;
            vec3 normalLU = texture2D(texture_normalmap, fragPosition.xz - vec2(-tx, ty)).xyz;
            vec3 normalLD = texture2D(texture_normalmap, fragPosition.xz - vec2(-tx, -ty)).xyz;
            vec3 normalR = texture2D(texture_normalmap, fragPosition.xz - vec2( tx, 0.0)).xyz;
            vec3 normalRU = texture2D(texture_normalmap, fragPosition.xz - vec2( tx, ty)).xyz;
            vec3 normalRD = texture2D(texture_normalmap, fragPosition.xz - vec2( tx, -ty)).xyz;
            vec3 normalU = texture2D(texture_normalmap, fragPosition.xz - vec2( 0.0, ty)).xyz;
            vec3 normalD = texture2D(texture_normalmap, fragPosition.xz - vec2( 0.0,-ty)).xyz;
            vec3 temp_normal = vec3(normalU + normalD + normalL + normalLU + normalLD + normalR + normalRU + normalRD) * 0.125;
            normal += normalize(temp_normal);
        }

        normal *= 1.0/(normal_blur_iteration+1.0);
        normal = normalize(normal);
    }else if(blur_type == 3){
        for(float i = 1.0; i < normal_blur_iteration+1.0; i++){                       
            float tx = (1.0/(width*normal_blur_width_factor)) * i; 
            float ty = (1.0/(h*normal_blur_height_factor)) * i;
            vec3 normalLD = texture2D(texture_normalmap, fragPosition.xz - vec2(-tx, -ty)).xyz;
            vec3 normalRD = texture2D(texture_normalmap, fragPosition.xz - vec2( tx, -ty)).xyz;
            vec3 normalU = texture2D(texture_normalmap, fragPosition.xz - vec2( 0.0, ty)).xyz;
            vec3 temp_normal = vec3(normalLD + normalRD + normalU) * 0.3333;
            normal += normalize(temp_normal);
        }

        normal *= 1.0/(normal_blur_iteration+1.0);
        normal = normalize(normal);
    }

    return normal;
}

void main()
{   
    if(isSprite){
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
        vec4 depth = texture2D(texture_depth, gl_FragCoord.xy/vec2(width,height));
        
        if(pixel == sprite_key_color)
            discard;

        float max_distance = far;

        vec2 p = camera_position.xz + vec2(terrain_size/2.0) - sprite_position.xz;
        float n_angle = -angle + PI/2;
        vec2 n = vec2(cos(n_angle), sin(n_angle));
        vec2 q = dot(n,p)*n;
        float z = length(q);
		z /= far;

        if(!isDepth){

			float upperLimite = 0.9;
            float lowerLimite = 0.5;

            float factor = (upperLimite - z) / (upperLimite - lowerLimite);
            factor = clamp(factor, 0.0, 1.0);
			

			vec2 sprite_texCoord_position_normalize = sprite_texCoord_position / spriteSheet_size;
			vec2 sprite_texCoord_size_normalize = sprite_texCoord_size / spriteSheet_size;

			vec3 position = sprite_position;
			
			//position.z += (texture2D(sprite_depthTexture, gl_TexCoord[0].xy).r-0.5)*sprite_texCoord_size.x; // reconsdier depth z length to be sprite_texCoord_size
            
			position.x /= terrain_size;
			position.z /= terrain_size;
            position.y = (sprite_position.y * scale_height ) / (256.0 * terrain_size); // same as: (sprite_position.y / 255) * scale_height ) / terrain_size, simplifying

			vec2 normalized_coord = ( gl_TexCoord[0].xy - sprite_texCoord_position_normalize) / sprite_texCoord_size_normalize;
			
			normalized_coord.y = 1.0 - normalized_coord.y;
			normalized_coord.x = normalized_coord.x - 0.5;
                               


            //reverse the rotation to make sure the real world position
            //doesn't move with the rotation.
            vec2 offset = vec2(0.0, 0.5);
            float x = normalized_coord.x - offset.x;
            float y = normalized_coord.y - offset.y;
            normalized_coord = rotate(vec2(x,y), -sprite_rotation*3.1459/180.0);
            normalized_coord += offset;

            if(isFlipX)
                normalized_coord.x = normalized_coord.x * -1.0;
            if(isFlipY)
                normalized_coord.y = 1.0 -  normalized_coord.y;
            
            if(is_debug_normalized_scale)
                gl_FragColor = vec4(normalized_coord.xy,0.0, 1.0); 	

            //normalize to terrain size
			normalized_coord.x *= (sprite_texCoord_size.x * sprite_scale.x) / terrain_size;
            normalized_coord.y *= (sprite_texCoord_size.y * sprite_scale.y);
            normalized_coord.y  = (2.0*normalized_coord.y) / terrain_size;
            
           if(is_debug_normalized_scale)
                gl_FragColor = vec4(normalized_coord.xy,0, 1.0); 	

            if(is_debug_position)
                gl_FragColor = vec4(position, 1.0);

			position.x += normalized_coord.x;
			position.y += normalized_coord.y;

            if(is_debug_position_normalized_scale)
                gl_FragColor = vec4(position, 1.0);


			//normalized_coord.y *= sprite_texCoord_size.y/ sprite_z / texture2D(texture_height, position.xz).r;
            
            
			vec3 normal = vec3(0.0, 0.0, 1.0);
			 // lookup the pixel in the texture
             if(has_sprite_depthTexture){
                vec2 coord_tex = gl_TexCoord[0].xy;
                float hL = texture2D(sprite_depthTexture, coord_tex - vec2(-1.0 / spriteSheet_size.x, 0.0)).r * sprite_depth_factor;
                float hR = texture2D(sprite_depthTexture, coord_tex - vec2( 1.0 / spriteSheet_size.x, 0.0)).r * sprite_depth_factor;
                float hU = texture2D(sprite_depthTexture, coord_tex - vec2( 0.0, 1.0 / spriteSheet_size.y)).r * sprite_depth_factor;
                float hD = texture2D(sprite_depthTexture, coord_tex - vec2( 0.0,-1.0 / spriteSheet_size.y)).r * sprite_depth_factor;

                //float normal = vec3(normalU + normalD + normalL + normalR) * 0.25;    
                if(isFlipX)
			        normal.x = hL - hR;
                else
                    normal.x = hR - hL;

			    normal.y = hD - hU;
			    normal.z = 1.0/length(sprite_texCoord_size);
                normal = normalize(normal);
            }


            /*

                Calcule des lumières directionelle et pointlight (potentiellement spot)

            */

            pixel = calcDirectionalLight(normal, position);
            
			for(int i = 0; i < NB_POINT_LIGHTS && i < pointLightCount; i++)
			    pixel.rgb += texture2D(texture, gl_TexCoord[0].xy).rgb * vec4(CalcPointLight(pointLights[i], normal, position, shininess), 1.0).rgb;

            if(is_debug_none){
			    gl_FragColor = vec4( fogColor.xyz*(1.0 - factor) + pixel.xyz * factor, 1.0);
                if(has_sprite_emissionTexture){
                    vec4 color = texture2D(sprite_emissionTexture, gl_TexCoord[0].xy) * gl_Color;
                    if(color != sprite_key_color)
                        gl_FragColor = vec4(color.rgb, 1.0);
                }
            }
			//gl_FragColor = vec4(texture2D(sprite_depthTexture, gl_TexCoord[0].xy).xyz, 1.0);

           if(is_debug_normal)
			    gl_FragColor = vec4(normal, 1.0);
           if(is_debug_depth)
                gl_FragColor = vec4(vec3(rgbToFloat(depth.rgb, 256.0)), 1.0);

        }else{
            gl_FragColor = vec4(floatToRgb(z, 256.0), 1.0);
        }
    }
    else{
        float max_distance = far;

        vec2 p = camera_position.xz + vec2(terrain_size/2.0) - gl_TexCoord[0].xy*terrain_size;
        float n_angle = -angle + PI/2;
        vec2 n = vec2(cos(n_angle), sin(n_angle));
        vec2 q = dot(n,p)*n;
        float z = length(q);
		z /= far;

        if(isFixedPreRender){
            vec4 pixel = gl_Color;
            if(pixel.a == 0.0)
                discard;
           gl_FragColor = vec4(pixel.rgb, 1.0);
        }

        else if(isFixed){
           vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
           vec4 depth = texture2D(texture_depth, gl_TexCoord[0].xy);
           float xAxis = rgbToFloat(texture2D(texture_Xaxis, gl_TexCoord[0].xy).rgb, 256.0);
           float yAxis = (rgbToFloat(texture2D(texture_Yaxis, gl_TexCoord[0].xy).rgb, 256.0) * scale_height) / terrain_size;
           float zAxis = rgbToFloat(texture2D(texture_Zaxis, gl_TexCoord[0].xy).rgb, 256.0);

           z = texture2D(texture_depthTerrain, gl_TexCoord[0].xy).r;

           vec3 fragPosition = vec3(xAxis, yAxis, zAxis);

           if(rgbToFloat(depth.rgb, 256.0) < z && packColor(depth.rgb) != 0.0){
                 discard;
           }
           
            if(pixel.a == 0.0)
               discard;

            // lookup the pixel in the texture
            vec3 normal = retrieve_normal(fragPosition.xz);
            normal = blur_normal(normal, fragPosition);

            pixel = calcDirectionalLight(normal, fragPosition);

			for(int i = 0; i < NB_POINT_LIGHTS && i < pointLightCount; i++)
				pixel.rgb += CalcPointLight(pointLights[i], normal, fragPosition, shininess);

            float upperLimite = 0.9;
            float lowerLimite = 0.5;

            float factor = (upperLimite - z) / (upperLimite - lowerLimite);
            factor = clamp(factor, 0.0, 1.0);
            
           if(is_debug_normal)
                gl_FragColor = vec4(normal, 1.0);
           if(is_debug_none)
                gl_FragColor = vec4(pixel.rgb, 1.0); //gl_FragColor =  vec4( fogColor.xyz*(1.0 - factor) + pixel.xyz * factor, 1.0);
           if(is_debug_position_normalized_scale || is_debug_position || is_debug_normalized_scale)
                gl_FragColor = vec4(fragPosition, 1.0);
           if(is_debug_depth)
                gl_FragColor = vec4(vec3(z), 1.0);
        }
        else if(isXaxis){
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
      
        else if(isDepth){
          gl_FragColor = vec4(floatToRgb(z, 256.0), 1.0);
        }
          else{
            vec4 depth = texture2D(texture_depth, gl_FragCoord.xy/vec2(width,height));

            if(rgbToFloat(depth.rgb, 256.0) < z && packColor(depth.rgb) != 0.0){
                    discard;
            }
                
			vec3 fragPosition = vec3(gl_TexCoord[0].xy, texture2D(texture_height, gl_TexCoord[0].xy)).xzy;
            fragPosition.y = (fragPosition.y  * scale_height) / terrain_size;

            // lookup the pixel in the texture
            vec3 normal = texture2D(texture_normalmap, fragPosition.xz).rgb;
            normal = normalize(normal);

            normal = blur_normal(normal, fragPosition);

            vec4 pixel = calcDirectionalLight_dynamique(normal, fragPosition);

            float upperLimite = 0.9;
            float lowerLimite = 0.5;

            float factor = (upperLimite - z) / (upperLimite - lowerLimite);
            factor = clamp(factor, 0.0, 1.0);

			
			float distance    = length(pointLights[0].position - fragPosition);
			float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * distance + pointLights[0].quadratic * (distance * distance));
			
			for(int i = 0; i < NB_POINT_LIGHTS && i < pointLightCount; i++)
				pixel.rgb += CalcPointLight(pointLights[i], normal, fragPosition, 32).rgb;

           gl_FragColor =  vec4( fogColor.xyz*(1.0 - factor) + pixel.xyz * factor, 1.0);
           //gl_FragColor = vec4(floatToRgb(z, 256.0), 1.0);
		   //gl_FragColor = vec4(normal, 1.0);
           //gl_FragColor = vec4(pixel.rgb, 1.0);
           
           //gl_FragColor = vec4(z,z,z, 1.0);
           //gl_FragColor = vec4(fragPosition, 1.0);

            if(is_debug_normal)
                gl_FragColor = vec4(normal.rgb, 1.0);
           //gl_FragColor = vec4(pixel.rgb, 1.0);
           if(is_debug_none)
             gl_FragColor =  vec4( fogColor.xyz*(1.0 - factor) + pixel.xyz * factor, 1.0);
           //gl_FragColor = vec4(xAxis,yAxis,zAxis, 1.0);
            if(is_debug_position_normalized_scale || is_debug_position || is_debug_normalized_scale)
                gl_FragColor = vec4(fragPosition, 1.0);
            if(is_debug_depth)
                gl_FragColor = vec4(vec3(z), 1.0);
        }



    }

}
