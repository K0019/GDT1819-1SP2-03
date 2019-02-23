#version 330 core

/* File: SingleColorFragmentShader.frag
   Use: To apply lighting to the primitive 
		Shader applicable to the bear object only */
		
// No. of point lights and spotlights
#define NO_OF_POINTLIGHTS 0
#define NO_OF_SPOTLIGHTS 2

// Definition of struct PointLight
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Definition of struct SpotLight
struct SpotLight {
				   // Offset, Size
	vec3 position; // 0, 12
	
	float constant; // 12, 4
	float linear; // 16, 4
	float quadratic; // 20, 4
	
	vec3 ambient; // 32, 12
	vec3 diffuse; // 48, 12
	vec3 specular; // 64, 12
	
	vec3 direction; // 80, 12
	float cosInner; // 92, 4
	float cosOuter; // 96, 4
};

// Input from geometry shader
in vec3 normal; // Normal of primitive
in vec3 pos; // Position of vertex in view space
in vec3 sunDir; // Directional light
//in PointLight cPointLights[NO_OF_POINTLIGHTS]; // All the point lights
in SpotLight cSpotLights[NO_OF_SPOTLIGHTS]; // All the spotlights

// Ouput data
out vec4 color;

// Color data input
layout (std140) uniform ColorData
{
	vec3 inColor;
	vec3 inDiffuse;
	vec3 inSpecular;
	float inShininess;
};

vec4 calculatePointLight(const PointLight light, const vec3 norm, const vec3 fragPos); // Calculates color contributed by a point light
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 fragPos); // Calculates color contributed by a spotlight

void main()
{
	// Initialize to black
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Directional light
	color += max(dot(-normal, sunDir), 0.0f) * vec4(inDiffuse, 0.0f);

	// Add color contributed by each point light
	/*for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		color += calculatePointLight(cPointLights[i], normal, pos);
	}*/

	// Add color contributed by each spotlight
	for (int i = 0; i < NO_OF_SPOTLIGHTS; ++i)
	{
		color += vec4(calculateSpotLight(cSpotLights[i], normal, pos), 0.0f);
	}
}

vec4 calculatePointLight(const PointLight light, const vec3 norm, const vec3 fragPos)
{
	// Direction vector to point light
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-pos), reflectDir), 0.0f), inShininess);
	
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Calculate resultant color for each material property
	vec3 ambient = light.ambient * inColor;
	vec3 diffuse = light.diffuse * diff * inDiffuse;
	vec3 specular = light.specular * spec * inSpecular;

	// Return final color with attenuation taken into account
	return attenuation * vec4(ambient + diffuse + specular, 0.0f);
}

vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 fragPos)
{
	// Direction vector to spotlight
	vec3 lightDir = normalize(light.position - fragPos);
	
	// Diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-pos), reflectDir), 0.0f), inShininess);
	
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Calculate intensity of color to be applied
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cosInner - light.cosOuter;
	float intensity = clamp((theta - light.cosOuter) / epsilon, 0.0f, 1.0f);
	
	// Calculate resultant color for each material property
	vec3 ambient = light.ambient * inColor;
	vec3 diffuse = light.diffuse * diff * inDiffuse;
	vec3 specular = light.specular * spec * inSpecular;
	
	// Return final color with attenuation and intensity taken into account
	return intensity * attenuation * (ambient + diffuse + specular);
}