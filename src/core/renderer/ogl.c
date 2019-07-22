#include "ogl.h"

void ogl_setup(void) {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void ogl_clear(float r, float g, float b, float a) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(r, g, b, a);
}

GLuint ogl_vertex_buffer_generate(void) {
    GLuint vaoID;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glGenBuffers(1, &vaoID);

    return vaoID;
}

GLuint ogl_buffer_generate_float(unsigned int dataSize, GLfloat* data, GLenum target, GLenum usage) {
    GLuint buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, dataSize, data, usage);

    return buffer;
}

GLuint ogl_buffer_generate_uint(unsigned int dataSize, GLuint* data, GLenum target, GLenum usage) {
    GLuint buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, dataSize, data, usage);

    return buffer;
}

void ogl_buffer_delete(unsigned int count, GLuint* buffer) {
    glDeleteBuffers(count, buffer);
}

mesh_t ogl_quad_generate(void) {
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    mesh_t mesh;

    mesh.vaoID = ogl_vertex_buffer_generate();

    GLuint vboID = ogl_buffer_generate_float(sizeof(vertices), vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint iboID = ogl_buffer_generate_uint(sizeof(indices), indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    mesh.iCount = (sizeof(indices) / sizeof(GLuint));
    mat4_identity(&mesh.tMatrix);

    glBindVertexArray(0);

    return mesh;
}

char* util_file_read(const char* filePath, const char* mode) {
    FILE* fp = fopen(filePath, mode);

    if (!fp) {
        printf("failed to read file: %s\nerror: %d %s", filePath, errno, stderr);
        exit(0);
    }

    fseek(fp, 0, SEEK_END);
    unsigned int len = ftell(fp);
    rewind(fp);
    char *data = calloc(len + 1, sizeof(char));
    fread(data, 1, len, fp);
    fclose(fp);

    return data;
}

GLuint shader_compile(const char* filePath, int type) {
    unsigned int shader;
    unsigned int length, result;

    char* source = util_file_read(filePath, "r");
    if(!source) return 0;

    shader = glCreateShader(type);
    length = strlen(source);
    glShaderSource(shader, 1, (const char**)&source, &length);
    glCompileShader(shader);
    free(source);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        char* log;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        log = malloc(length);
        glGetShaderInfoLog(shader, length, &result, log);

        fprintf(stderr, "shaderCompileFromFile(): Unable to compile %s: %s\n", filePath, log);
        free(log);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint shader_load(const char *vertexFilePath, const char *fragmentFilePath) {
    unsigned int vertexShaderID   = shader_compile(vertexFilePath, GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = shader_compile(fragmentFilePath, GL_FRAGMENT_SHADER);

    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

// 475 -250