#version 330 core

/* File: TextureColorFragmentShader.frag
   Use: To apply lighting to the primitive 
		General shader used in most meshes */
		
// No. of point lights and spotlights
#define NO_OF_POINTLIGHTS 0
#define NO_OF_SPOTLIGHTS 4

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
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
	float cosInner;
	float cosOuter;
};

// Definition of struct Material
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

layout (std140) uniform MatrixMV // 208
{
				// Offset, Size
	mat4 model; // 0, 64
	mat4 view; // 64, 64
	
	vec3 sunDir; // 128, 12
	//PointLight vPointLights[NO_OF_POINTLIGHTS]; // 144, 80 * NO_OF_POINTLIGHTS
};

layout (std140) uniform uSpotLight
{
	SpotLight vSpotLights[NO_OF_SPOTLIGHTS]; // 0, 112 * NO_OF_SPOTLIGHTS
};

// Input from geometry shader
in vec3 normal; // Normal of primitive
in vec3 pos; // Position of vertex in view space
in vec2 texCoord; // Texture coordinate of vertex
//in vec3 sunDir; // Sun direction
//in PointLight cPointLights[NO_OF_POINTLIGHTS]; // All the point lights
//in SpotLight cSpotLights[NO_OF_SPOTLIGHTS]; // All the spotlights

// Ouput data
out vec4 color;

// Texture data
uniform Material material;

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 fragPos); // Calculates color contributed by a point light
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 fragPos); // Calculates color contributed by a spotlight

void main()
{
	// Initialize to black
	color = vec4(0.0f, 0.0f, 0.0f, texture(material.diffuse, texCoord).w);

	// Directional light
	vec3 sundir = normalize(vec3(mat4(mat3(view)) * vec4(sunDir, 1.0f)));
	color += vec4(vec3(max(dot(-normal, sundir), 0.0f) * texture(material.diffuse, texCoord)), 0.0f);

	// Add color contributed by each point light
	/*for (int i = 0; i < NO_OF_POINTLIGHTS; ++i)
	{
		color += vec4(calculatePointLight(cPointLights[i], normal, pos), 0.0f);
	}*/
	
	// Add color contributed by each spotlight
	/*for (int i = 0; i < NO_OF_SPOTLIGHTS; ++i)
	{
		color += vec4(calculateSpotLight(cSpotLights[i], normal, pos), 0.0f);
	}*/
	for (int i = 0; i < 4; ++i)
	{
		SpotLight spotlight = vSpotLights[i];
		spotlight.position = vec3(view * vec4(spotlight.position, 1.0f));
		spotlight.direction = vec3(mat4(mat3(view)) * vec4(spotlight.direction, 1.0f));
		color += vec4(calculateSpotLight(spotlight, normal, pos), 0.0f);
	}
}

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 fragPos)
{
	// Direction vector to point light
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-pos), reflectDir), 0.0f), material.shininess);
	
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Calculate resultant color for each material property
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// Return final color with attenuation taken into account
	return attenuation * (ambient + diffuse + specular);
}

vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 fragPos)
{
	// Direction vector to spotlight
	vec3 lightDir = normalize(light.position - fragPos);
	
	// Diffuse shading
	float diff = max(dot(norm, lightDir), 0.0f);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-pos), reflectDir), 0.0f), material.shininess);
	
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cosInner - light.cosOuter;
	float intensity = clamp((theta - light.cosOuter) / epsilon, 0.0f, 1.0f);
	
	// Calculate resultant color for each material property
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
	
	return intensity * attenuation * (ambient + diffuse + specular);
}