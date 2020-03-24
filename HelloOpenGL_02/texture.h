/*
Texture2D类，封装了纹理有关的方法
功能：
1.生成纹理
2.删除纹理
3.绑定纹理
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
public:
    GLuint ID;
    GLuint Width, Height;
    GLuint Internal_Format;
    GLuint Image_Format;
    // T纹理配置
    GLuint Wrap_S;
    GLuint Wrap_T;
    GLuint Filter_Min;
    GLuint Filter_Max;

public:
    Texture2D();
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void Delete();
    void Bind() const;
};

#endif
