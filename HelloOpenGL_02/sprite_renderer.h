/*
SpriteRenderer�ࣺ
���ܣ���װ�˻�ͼ�йصĹ��ܣ�������1.��ʼ��VAO��VBO��2.����ͼ��
*/

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
public:
    SpriteRenderer(Shader &shader, int type);
    ~SpriteRenderer();
    // ����ͼ��
    void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size, GLfloat rotate, glm::vec3 axis, glm::mat4 view, glm::mat4 proj, glm::vec3 color = glm::vec3(1.0f));
	void DrawSprite(Texture2D &texture, glm::vec3 position, glm::mat4 view, glm::mat4 proj,bool shooted=false);
	void DrawSprite(Texture2D &texture, std::vector<glm::vec3> positions, glm::mat4 view, glm::mat4 proj, std::vector<bool> shooteds);
	void DrawSprite(GLuint cubemapTexture,glm::mat4 view,glm::mat4 proj);
	void DrawWall(Texture2D &texture, std::vector<glm::vec3> postions, glm::mat4 view, glm::mat4 proj);
private:
    Shader shader;
    GLuint quadVAO;
    // ��ʼ���ͼ���VAO��VBO
    void initRenderData(); //��ʼ��2d����
    void initRenderData3d(); //��ʼ��3d����
	void initRenderDataSkybox(); //��ʼ����պ�
	void initRenderDataEnemy(); //��ʼ������
	void initRenderDataWall(); //��ʼ��ǽ��
};

#endif
