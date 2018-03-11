#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct DirLight
{
    // vec3 position; // 现在不在需要光源位置了，因为它是无限远的
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

in vec3 world_pos;
in vec3 world_normal;

out vec4 color;


//const vec3 ambinetColor = vec3(0.90,0.0,0.20);

//number of levels
//for diffuse color
const int levels = 5;
const float scaleFactor = 1.0 / levels;
//const vec3 diffuseColor = vec3(0.30,0.80,0.10);


void main()
{
    vec3 L = normalize( lightPos - world_pos);
    vec3 V = normalize( viewPos - world_pos);
    vec3 H = normalize(L + V );
    
    //vec3 diffuse = material.diffuse * max(0, dot(L,world_normal));
    float diffuse = max(0, dot(L,world_normal));
    
    
    
    vec3 diffuseColor = material.diffuse * floor(diffuse * levels) * scaleFactor;
    
    vec3 specular = vec3(0,0,0);
    
    if(dot(L,world_normal) > 0.0)
    {
        specular = material.specular * pow( max(0, dot( H, world_normal)), material.shininess);
    }
    
    //Black color if dot product is smaller than 0.3
    //else keep the same colors
    float specMask = (pow(dot(H, world_normal), material.shininess) > 0.4) ? 1 : 0;
    float edgeDetection = (dot(V, world_normal) > 0.2) ? 1 : 0;
    
    vec3 light = edgeDetection * (material.ambient + diffuseColor + specular* specMask);
    color = vec4(light,1.0f);

    
}


/*
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
//in vec3 normalColor;
in vec3 Normal;
in vec3 FragPos;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
*/

/*
void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
   
    //color = vec4(vec3(normalColor.x*0.5+0.5,normalColor.y*0.5+0.5,normalColor.z*0.5+0.5), sampleExtraOutput);
    
    
    //vec3 viewDir = normalize(viewPos - FragPos);
    
    //vec3 result = CalcDirLight(dirLight, Normal, viewDir);
    
    //color = vec4(result, 1.0);
    
    
    vec3 lightDir = normalize(dirLight.direction);
    float intensity = max(dot(Normal, lightDir), 0.0);
    
    vec3 ambient = dirLight.ambient * material.ambient;
    vec3 diffuse = dirLight.diffuse * material.diffuse;
    vec3 specular = dirLight.specular * material.specular;

    
    if (intensity > 0.95)
        
        color = vec4(ambient + diffuse + specular,1.0);
        //color = vec4(1.0,0.5,0.5,1.0);
    else if (intensity > 0.5)
        color = vec4((ambient + diffuse + specular)*0.6,1.0);
        //color = vec4(0.6,0.3,0.3,1.0);
    else if (intensity > 0.25)
        color = vec4((ambient + diffuse + specular)*0.6*0.6,1.0);
        //color = vec4(0.4,0.2,0.2,1.0);
    else if (intensity > 0.01)
        color = vec4((ambient + diffuse + specular)*0.6*0.6*0.6,1.0);
    else
        color = vec4(0.0,0.0,0.0,1.0);
    
}
*/

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}
