#version 330 core
in vec2 oTexC;
in vec3 oNormal;
in vec3 oWorldPos;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 FragPosition;
layout (location = 2) out vec3 FragNormal;
uniform float iSmoothness;
uniform sampler2D iTexImage;
void main()
{
   FragColor.rgb = texture(iTexImage, oTexC).rgb;
   FragColor.a = iSmoothness;
   FragPosition = oWorldPos;
   FragNormal = oNormal;
}