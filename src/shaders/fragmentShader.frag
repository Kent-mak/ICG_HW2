#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 

uniform sampler2D ourTexture;
uniform vec3 rainbowColor;
uniform bool useRainbow;

void main()
{
    // TODO: Implement Rainbow Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. Set FragColor to be the dot product of the color and rainbowColor
	//   Note: Ensure FragColor is appropriately set for both rainbow and normal cases.
	
	vec4 texColor = texture(ourTexture, TexCoord);

	if (useRainbow) {
		float brightness = dot(texColor.rgb, rainbowColor);
    	FragColor = vec4(brightness * rainbowColor, texColor.a);
	} else {
		FragColor = texColor;
	}
	
} 