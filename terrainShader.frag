#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    //vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 myColor;
uniform float shadowC;



const int levels = 3;
const float scaleFactor = 1.0 / levels;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}

void main()
{

    vec3 normalcolor = vec3(fs_in.Normal.x * 0.5 + 0.5,fs_in.Normal.y* 0.5 + 0.5,fs_in.Normal.z* 0.5 + 0.5);
    //vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 color;
    
    if(FragPos.y < 0.0f)
    {
        color = vec3(0, 0.4, 1);
    }
    else if (FragPos.y < 2.5f)
    {
        color = vec3(1,1,0.6);
    }
    else
    {
        color = myColor;
    }
    
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.5);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 lightDir = normalize(vec3(0,1,0));
    
    vec3 L = lightDir;
    vec3 V = normalize( viewPos - fs_in.FragPos);
    vec3 H = normalize(L + V );
    
    // ambient
    vec3 ambient = 0.2 * color;
    
    // diffuse
    
    float diff = max(dot(lightDir, normal), 0.0);
    /*
    vec3 diffuse;
    if (diff > 0.75)
        diffuse = lightColor;
    else if (diff > 0.5)
        diffuse = lightColor * 0.3;
    else if (diff > 0.25)
        diffuse = lightColor * 0.3 * 0.3;
    else
        diffuse = lightColor * 0.3 * 0.3 * 0.3;
    */
    vec3 diffuse = diff * lightColor * floor(diff * levels) * scaleFactor;;
    
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;
    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    
    if(shadowC == -1.0f)
    {
        shadow = 0;
    }
    
    // toon shading
    float specMask = (pow(dot(H, normal), 32.0) > 0.4) ? 1 : 0;
    float edgeDetection = (dot(V, normal) > 0.01) ? 1 : 0;
    
    //vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    vec3 lighting = edgeDetection* (ambient + (1.0 - shadow) * (diffuse + specular * specMask)) * color;
    //vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
    
}
