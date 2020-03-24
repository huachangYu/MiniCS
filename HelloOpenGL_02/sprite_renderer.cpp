#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader, int type)
{
    this->shader = shader;
	if (type == 2)
		this->initRenderData();
	else if (type == 3)
		this->initRenderData3d();
	else if (type == 4)
		this->initRenderDataSkybox();
	else if (type == 5)
		this->initRenderDataEnemy();
	else if (type == 6)
		this->initRenderDataWall();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec2 size, GLfloat rotate, glm::vec3 axis, glm::mat4 view, glm::mat4 proj, glm::vec3 color)
{
    this->shader.Use();
    glm::mat4 mvp = proj * view;
    this->shader.SetMatrix4("mvp", mvp);
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D & texture, glm::vec3 position, glm::mat4 view, glm::mat4 proj, bool shooted)
{
	glm::vec3 color;
	if (shooted) {
		color = glm::vec3(1.0f, 0.0f, 10.0f);
	}
	else {
		color = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	this->shader.Use();
	this->shader.SetMatrix4("view", view);
	this->shader.SetMatrix4("projection", proj);
	this->shader.SetVector3f("spriteColor", color);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	this->shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D & texture, std::vector<glm::vec3> positions, glm::mat4 view, glm::mat4 proj, std::vector<bool> shooteds)
{
	int cubeNums = positions.size();
	glm::vec3 position,color;
	bool shooted;
	this->shader.Use();
	this->shader.SetMatrix4("view", view);
	this->shader.SetMatrix4("projection", proj);
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glBindVertexArray(this->quadVAO);
	for (int i=0;i<cubeNums;i++) 
	{
		position = positions[i];
		shooted = shooteds[i];
		if (shooted) {
			color = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			color = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		this->shader.SetVector3f("spriteColor", color);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		this->shader.SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(GLuint cubemapTexture, glm::mat4 view, glm::mat4 proj)
{
	glDepthFunc(GL_LEQUAL);
	this->shader.Use();
	this->shader.SetMatrix4("view", view);
	this->shader.SetMatrix4("projection", proj);

	glBindVertexArray(this->quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void SpriteRenderer::DrawWall(Texture2D &texture, std::vector<glm::vec3> positions, glm::mat4 view, glm::mat4 proj)
{
	std::vector<bool> shoots(positions.size());
	this->DrawSprite(texture, positions, view, proj,shoots);
}


void SpriteRenderer::initRenderData()
{
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData3d()
{
    GLuint VBO;
    GLfloat vertices[] = {
        -200.5f, -0.5f, 200.5f, 0.0f, 200.5f,
        200.5f, -0.5f, -200.5f, 200.5f, 0.0f,
        -200.5f, -0.5f, -200.5f, 0.0f, 0.0f,

        -200.5f, -0.5f, 200.5f, 0.0f, 200.5f,
        200.5f, -0.5f, 200.5f, 200.5f, 200.5f,
        200.5f, -0.5f, -200.5f, 200.5f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderDataSkybox()
{
	std::cout << "initRenderDataSkybox..." << std::endl;
	GLuint VBO;
	GLfloat skyboxVertices[] = {
		// positions          
		-200.5f,  200.5f, -200.5f,
		-200.5f, -200.5f, -200.5f,
		200.5f, -200.5f, -200.5f,
		200.5f, -200.5f, -200.5f,
		200.5f,  200.5f, -200.5f,
		-200.5f,  200.5f, -200.5f,

		-200.5f, -200.5f,  200.5f,
		-200.5f, -200.5f, -200.5f,
		-200.5f,  200.5f, -200.5f,
		-200.5f,  200.5f, -200.5f,
		-200.5f,  200.5f,  200.5f,
		-200.5f, -200.5f,  200.5f,

		200.5f, -200.5f, -200.5f,
		200.5f, -200.5f,  200.5f,
		200.5f,  200.5f,  200.5f,
		200.5f,  200.5f,  200.5f,
		200.5f,  200.5f, -200.5f,
		200.5f, -200.5f, -200.5f,

		-200.5f, -200.5f,  200.5f,
		-200.5f,  200.5f,  200.5f,
		200.5f,  200.5f,  200.5f,
		200.5f,  200.5f,  200.5f,
		200.5f, -200.5f,  200.5f,
		-200.5f, -200.5f,  200.5f,

		-200.5f,  200.5f, -200.5f,
		200.5f,  200.5f, -200.5f,
		200.5f,  200.5f,  200.5f,
		200.5f,  200.5f,  200.5f,
		-200.5f,  200.5f,  200.5f,
		-200.5f,  200.5f, -200.5f,

		-200.5f, -200.5f, -200.5f,
		-200.5f, -200.5f,  200.5f,
		200.5f, -200.5f, -200.5f,
		200.5f, -200.5f, -200.5f,
		-200.5f, -200.5f,  200.5f,
		200.5f, -200.5f,  200.5f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderDataEnemy()
{
	GLuint VBO;
	GLfloat enemyVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(enemyVertices), &enemyVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void SpriteRenderer::initRenderDataWall()
{
	GLuint VBO;
	GLfloat wallVertices[] = {
		-0.5f, -10.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -10.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  10.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  10.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  10.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -10.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -10.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -10.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  10.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  10.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  10.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -10.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  10.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  10.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -10.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -10.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -10.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  10.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  10.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  10.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -10.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -10.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -10.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  10.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -10.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -10.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -10.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -10.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -10.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -10.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  10.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  10.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  10.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  10.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  10.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  10.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), &wallVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
