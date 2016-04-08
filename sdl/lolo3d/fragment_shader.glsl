uniform sampler2D uSampler;
uniform bool uEnableTexture;
uniform vec4 uColor;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexcoord;
varying vec3 vLightDirection;

void main()
{
    vec4 color;
    float intensity = max(0.1, dot(vNormal, vLightDirection)) * 2.0;
    
    if (uEnableTexture) {
        color = texture2D(uSampler, vTexcoord);
    } else {
        color = uColor;
    }

    gl_FragColor = color * intensity;
}
