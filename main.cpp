#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png++/png.hpp>

#include "math_utils.hpp"
#include "model.h"

unsigned char pixels[512 * 512 * 4];

int main(int argc, char **argv) {
    int xRot = argc >= 2 ? atoi(argv[1]) % 90 : 0;
    int yRot = argc >= 3 ? atoi(argv[2]) % 180 : 0;
    int zRot = argc >= 4 ? atoi(argv[3]) % 180 : 0;
    printf("%d %d %d\n", xRot, yRot, zRot);

    // get the display.
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("failed to get the EGL display.\n");
        return -1;
    }
    printf("success to get the EGL display.\n");

    // initialize the EGL.
    EGLint magor, minor;
    EGLBoolean status;
    status = eglInitialize(display, &magor, &minor);
    if (status == EGL_FALSE) {
        printf("failed to initialize EGL.\n");
        return -1;
    }
    printf("success to initialize EGL.\n");

    // select a configuration.
    EGLint configAttribs[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
    };
    EGLint numConfigs;
    EGLConfig config;
    status = eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
    if (status == EGL_FALSE) {
        printf("failed to take a configuration.\n");
        return -1;
    }
    printf("success to take a configuration.\n");

    // create a surface/
    EGLint pbufferAttribs[] = {
            EGL_WIDTH, 512,
            EGL_HEIGHT, 512,
            EGL_NONE
    };
    EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE) {
        printf("failed to create a surface.\n");
        return -1;
    }
    printf("success to create a surface.\n");

    // create a context.
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    if (context == EGL_NO_CONTEXT) {
        printf("failed to create a context.\n");
        return -1;
    }
    printf("success to create a context.\n");

    // bind the OpenGL ES 2.0.
    status = eglBindAPI(EGL_OPENGL_ES_API);
    if (status == EGL_FALSE) {
        printf("failed to bind the OpenGL ES 2.0.\n");
        return -1;
    }
    printf("success to bind the OpenGL ES 2.0.\n");

    // bind a context.
    status = eglMakeCurrent(display, surface, surface, context);
    if (status == EGL_FALSE) {
        printf("failed to bind the context.\n");
        return -1;
    }
    printf("success to bind the context.\n");

    // draw.

    // compile a vertex shader.
    std::ifstream vsFs("./shader.vs");
    std::string vsStr((std::istreambuf_iterator<char>(vsFs)), std::istreambuf_iterator<char>());
    std::cout << "vertex shader:\n" << vsStr << std::endl;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vsCstr = vsStr.c_str();
    glShaderSource(vertexShader, 1, &vsCstr, NULL);
    glCompileShader(vertexShader);
    GLint shaderCompileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus == GL_FALSE) {
        printf("failed to compile a vertex shader.\n");
    }
    printf("success to compile a vertex shader.\n");

    // compile a fragment shader.
    std::ifstream fsFs("./shader.fs");
    std::string fsStr((std::istreambuf_iterator<char>(fsFs)), std::istreambuf_iterator<char>());
    std::cout << "fragment shader:\n" << fsStr << std::endl;
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsCstr = fsStr.c_str();
    glShaderSource(fragmentShader, 1, &fsCstr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus == GL_FALSE) {
        printf("failed to compile a fragment shader.\n");
    }
    printf("success to compile a fragment shader.\n");

    // link a shader program.
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint programLinkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinkStatus);
    if (programLinkStatus == GL_FALSE) {
        printf("failed to link the shader to program.\n");
    }
    printf("success to link the shader to program.\n");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // get the shader's attributes.
    GLuint attrPosition = 0;
    GLuint attrTexCoord = 1;
    glBindAttribLocation(program, attrPosition, "aPosition");
    glBindAttribLocation(program, attrTexCoord, "aTexCoord");
    GLuint uniModelTransform = glGetUniformLocation(program, "uModelTransform");
    GLuint uniTexture = glGetUniformLocation(program, "uTexture");

    // initialize the OpenGL status.
    glViewport(0, 0, 512, 512);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    // crate a texture.
    png::image <png::rgb_pixel> texImage("PC01_Kohaku.png");
    size_t texImageWidth = texImage.get_width();
    size_t texImageHeight = texImage.get_height();
    GLfloat *texPixels = new GLfloat[4 * texImageWidth * texImageHeight];
    for (int i = 0; i < texImageHeight; ++i) {
        for (int j = 0; j < texImageWidth; ++j) {
            int index = 4 * texImageWidth * i + 4 * j;
            png::rgb_pixel pixel = texImage[i][j];
            texPixels[index + 0] = (float) pixel.red / 255.0f;
            texPixels[index + 1] = (float) pixel.green / 255.0f;
            texPixels[index + 2] = (float) pixel.blue / 255.0f;
            texPixels[index + 4] = 1.0f;
        }
    }
    GLuint texBuffer;
    glGenTextures(1, &texBuffer);
    glBindTexture(GL_TEXTURE_2D, texBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageWidth, texImageHeight, 0, GL_RGBA, GL_FLOAT, texPixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    delete[] texPixels;

    // create buffers.
    GLuint modelVerticesBuffer;
    glGenBuffers(1, &modelVerticesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, modelVerticesBuffer);
    glBufferData(GL_ARRAY_BUFFER, modelVerticesSize, modelVertices, GL_STATIC_DRAW);

    GLuint modelIndicesBuffer;
    glGenBuffers(1, &modelIndicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelIndicesSize, modelIndices, GL_STATIC_DRAW);

    // draw the model.
    GLfloat matrix[16];
    setIdentityMatrix(matrix);
    mulRotationMatrix(matrix, 0.0f, 0.0f, 1.0f, (float) zRot * (M_PI / 180.0f));
    mulRotationMatrix(matrix, 1.0f, 0.0f, 0.0f, (float) xRot * (M_PI / 180.0f));
    mulRotationMatrix(matrix, 0.0f, -1.0f, 0.0f, (float) yRot * (M_PI / 180.0f));
    mulTranslateMatrix(matrix, 0.0f, 0.8f, 0.0f);
    mulRotationMatrix(matrix, 1.0f, 0.0f, 0.0f, M_PI / 2.0f);
    glUniformMatrix4fv(uniModelTransform, 1, GL_FALSE, matrix);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uniTexture, 0);

    glEnableVertexAttribArray(attrPosition);
    glVertexAttribPointer(attrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid *) 0);
    glEnableVertexAttribArray(attrTexCoord);
    glVertexAttribPointer(attrTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5,
                          (const GLvoid *) (sizeof(GLfloat) * 3));

    glDrawElements(GL_TRIANGLES, modelIndicesCount, GL_UNSIGNED_SHORT, (const GLvoid *) 0);

    // swap the draw buffer.
    glFinish();

    // out put a png with rendering result.
    glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    png::image <png::rgb_pixel> image(512, 512);
    for (int i = 0; i < 512; ++i) {
        for (int j = 0; j < 512; ++j) {
            int index = 512 * 4 * i + 4 * j;
            image[i][j] = png::rgb_pixel(pixels[index], pixels[index + 1], pixels[index + 2]);
        }
    }
    char fileName[256];
    sprintf(fileName, "image/rendering-%d-%d-%d.png", xRot, yRot, zRot);
    image.write(fileName);

    return 0;
}
