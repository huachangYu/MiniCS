/*
SpriteRenderer类：
功能：封装了绘图有关的功能，包括：1.初始化VAO和VBO；2.绘制图形
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
    // 绘制图形
    void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size, GLfloat rotate, glm::vec3 axis, glm::mat4 view, glm::mat4 proj, glm::vec3 color = glm::vec3(1.0f));
	void DrawSprite(Texture2D &texture, glm::vec3 position, glm::mat4 view, glm::mat4 proj,bool shooted=false);
	void DrawSprite(Texture2D &texture, std::vector<glm::vec3> positions, glm::mat4 view, glm::mat4 proj, std::vector<bool> shooteds);
	void DrawSprite(GLuint cubemapTexture,glm::mat4 view,glm::mat4 proj);
	void DrawWall(Texture2D &texture, std::vector<glm::vec3> postions, glm::mat4 view, glm::mat4 proj);
private:
    Shader shader;
    GLuint quadVAO;
    // 初始化和加载VAO和VBO
    void initRenderData(); //初始化2d数据
    void initRenderData3d(); //初始化3d数据
	void initRenderDataSkybox(); //初始化天空盒
	void initRenderDataEnemy(); //初始化敌人
	void initRenderDataWall(); //初始化墙壁
};

#endif
