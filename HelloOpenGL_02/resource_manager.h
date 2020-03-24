/*
ResourceManager��Ϊ��������Ա����ȫΪ��̬����
�������ܣ�
1.������ɫ��
2.�����ɫ��
3.��������
4.��ȡ����
5.��ȡ��պ�����
*/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <vector>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader GetShader(std::string name);
    static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
    static Texture2D LoadTexture(unsigned char *image, int width, int height, std::string name);
    static Texture2D GetTexture(std::string name);
	static unsigned int loadCubemap(std::vector<std::string> faces);
    static void      Clear();
private:
    ResourceManager() { }
    static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif
