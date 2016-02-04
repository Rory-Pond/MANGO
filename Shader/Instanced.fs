#version 130
//---------------------------
//----- Fracmant Shader -----
//---------------------------

// in data
in vec3 Color;
in vec3 vertexPos;
in vec3 Normal;
in mat4 M;
in vec3 LightPos2;

out vec3 outColor;

uniform vec3 LightPos;
uniform mat4 V;

void main()
{
//    float distance = length(LightPos - vertexPos);
//
//    vec3 cameraPos= ( V * vec4(vertexPos,1)).xyz;
//    vec3 cameraLightPos = ( V * vec4(LightPos,1)).xyz;
//    vec3 lightVector = normalize(cameraLightPos - cameraPos);
//
//    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
//    // pointing in the same direction then it will get max illumination.
//    float diffuse = max(dot(cameraNormal, lightVector), 0.1);
//
//    // Add attenuation.
//    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
//
//    // color the color by the diffuse illumination level to get final output color.
//    color = Color * diffuse;
    vec3 LightColor = vec3(1,1,1);
    float LightPower = 1.0;

    // Material properties
    vec3 MaterialDiffuseColor = Color;
    vec3 MaterialAmbientColor = vec3(0.6,0.6,0.6) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(0.1,0.1,0.1);

    vec3 WorldPos = (M * vec4(vertexPos,1)).xyz;

    // Distance to the light
    float distance = length(WorldPos - LightPos);

        // Normal of the the vertex, in camera space
    vec3 Normal_cameraspace = (-1)*( V * M * vec4(Normal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPos,1)).xyz;
    vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 LightPosition_cameraspace = ( V * vec4(LightPos,1)).xyz;
    vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( LightDirection_cameraspace );
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    outColor =
        // Ambient : simulates indirect lighting
        MaterialAmbientColor +
        // Diffuse : "color" of the object
        MaterialDiffuseColor * LightColor * LightPower * cosTheta;// / (distance*distance);// +
        // Specular : reflective highlight, like a mirror
        //MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}