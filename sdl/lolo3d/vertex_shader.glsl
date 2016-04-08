attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexcoord;
varying vec3 vLightDirection;

void main()
{
    vPosition = aPosition;
    vTexcoord = aTexcoord;

    mat3 normalMatrix;
    normalMatrix[0] = normalize((uViewMatrix * uModelMatrix)[0].xyz);
    normalMatrix[1] = normalize((uViewMatrix * uModelMatrix)[1].xyz);
    normalMatrix[2] = normalize((uViewMatrix * uModelMatrix)[2].xyz);
    vNormal = normalize(normalMatrix * aNormal);

    vec4 v1 = uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
    vec3 v2 = v1.xyz / v1.w;
    vLightDirection = normalize(vec3(0.0, 0.0, 1.0) - v2);

    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
}