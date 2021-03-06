// vertex shader
#version 330 core

layout (location = 0) in vec3 vertexData;
layout (location = 1) in vec3 normalData;
layout (location = 2) in vec3 colourData;

uniform mat4 m_projection;
uniform mat4 m_view;
uniform mat4 m_transform;

uniform vec3 m_light_pos;
out vec3 lightPosition;

out vec3 normals;
out vec3 fragPosition;
out vec3 colours;

void main() {
    gl_Position = m_projection * m_view * m_transform * vec4(vertexData, 1);

    fragPosition = vec3(m_transform * vec4(vertexData, 1.0));
    normals = normalData;

    lightPosition = m_light_pos;
}