#include "glad.h"
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>

#include "shader.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "fps_camera.h"

#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "lib/irrKlang.lib")
using namespace std;

irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

bool isShooted = false;
vector<glm::vec3> enemyPositions;
vector<bool> enemyShooteds;

const unsigned int width = 800;
const unsigned int height = 600;
const float pi = 3.14;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = width / 2;
float lastY = height / 2;
bool  firstMouse = true;
Camera camera(glm::vec3(5.0f, 0.0f, 0.0f));

// game map
vector<vector<double>> gameMap(400, vector<double>(400));
int mapCenterX = 200;
int mapCenterY = 200;

// box
vector<glm::vec3> boxPositions;
vector<bool> boxShooted; //alwayls be false

// wall
extern const double WALL_HEIGHT = 999;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void shakeEnermy(vector<glm::vec3>& enemy);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



    GLFWwindow* window = glfwCreateWindow(width, height, "Mini CS", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    ResourceManager::LoadShader("shaders/2d.vert", "shaders/common.frag", nullptr, "sprite");
	

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::LoadTexture("textures/crosshair.png", GL_TRUE, "crosshair");
	
    Shader shader = ResourceManager::GetShader("sprite");
    SpriteRenderer* Renderer = new SpriteRenderer(shader, 2);

	//skybox
	ResourceManager::LoadShader("shaders/skybox_shader.vert", "shaders/skybox_shader.frag", nullptr, "skybox");
	Shader skyboxShader = ResourceManager::GetShader("skybox");
	SpriteRenderer* RenderSkybox = new SpriteRenderer(skyboxShader, 4);
	vector<string> faces 
	{
		"textures/arrakisday_ft.jpg",
		"textures/arrakisday_bk.jpg",
		"textures/arrakisday_up.jpg",
		"textures/arrakisday_dn.jpg",
		"textures/arrakisday_rt.jpg",
		"textures/arrakisday_lf.jpg"
	};
	unsigned int skyboxTexture = ResourceManager::loadCubemap(faces);

	//enemy
	ResourceManager::LoadShader("shaders/enemy_shader.vert", "shaders/enemy_shader.frag", nullptr, "enemy");
	Shader enemyShader = ResourceManager::GetShader("enemy");
	SpriteRenderer* RenderEnemy = new SpriteRenderer(enemyShader, 5);
	enemyPositions.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(5.0f, 1.0f, 1.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(-15.0f, 5.0f, 0.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(-15.0f, 5.0f, 10.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(15.0f, 3.0f, -10.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(-18.0f, 3.0f, -12.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(-18.0f, 5.0f, -12.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(-13.0f, 0.0f, -10.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(17.0f, 3.0f, -13.0f));
	enemyShooteds.push_back(false);
	enemyPositions.push_back(glm::vec3(19.0f, 3.0f, -13.0f));
	enemyShooteds.push_back(false);
	ResourceManager::LoadTexture("textures/funny.jpg", GL_FALSE, "funny");

	//box
	ResourceManager::LoadShader("shaders/enemy_shader.vert", "shaders/enemy_shader.frag", nullptr, "enemy");
	Shader boxShader = ResourceManager::GetShader("enemy");
	SpriteRenderer* RenderBox = new SpriteRenderer(enemyShader, 5);
	boxPositions = {
		glm::vec3(19.0f,0.0f,1.0f),
		glm::vec3(18.0f,0.0f,1.0f),
		glm::vec3(17.0f,0.0f,1.0f),
		glm::vec3(16.0f,0.0f,1.0f),
		glm::vec3(19.0f,0.0f,2.0f),
		glm::vec3(18.0f,0.0f,2.0f),
		glm::vec3(17.0f,0.0f,2.0f),
		glm::vec3(16.0f,0.0f,2.0f),
		glm::vec3(19.0f,0.0f,3.0f),
		glm::vec3(18.0f,0.0f,3.0f),
		glm::vec3(17.0f,0.0f,3.0f),
		glm::vec3(16.0f,0.0f,3.0f),
		glm::vec3(19.0f,0.0f,4.0f),
		glm::vec3(18.0f,0.0f,4.0f),
		glm::vec3(17.0f,0.0f,4.0f),
		glm::vec3(16.0f,0.0f,4.0f),
		glm::vec3(18.0f,1.0f,3.0f),
		glm::vec3(17.0f,1.0f,3.0f),
		glm::vec3(18.0f,1.0f,2.0f),
		glm::vec3(17.0f,1.0f,2.0f),
	};
	int px = 0, py = 0;
	for (auto position : boxPositions) {
		px = mapCenterX + round(position[0]);
		py = mapCenterY + round(position[2]);
		gameMap[px][py] =gameMap[px][py] + 1.0;
		boxShooted.push_back(false);
	}

	//wall
	ResourceManager::LoadShader("shaders/enemy_shader.vert", "shaders/enemy_shader.frag", nullptr, "wall");
	Shader wallShader = ResourceManager::GetShader("wall");
	SpriteRenderer* RenderWall = new SpriteRenderer(wallShader, 6);
	vector<glm::vec3> wallPositions = {
		glm::vec3(-10.0f,0.0f,-10.0f),
		glm::vec3(-10.0f,0.0f,-9.0f),
		glm::vec3(-10.0f,0.0f,-8.0f),
		glm::vec3(-10.0f,0.0f,-7.0f),
		glm::vec3(-10.0f,0.0f,-6.0f),
		glm::vec3(-10.0f,0.0f,-5.0f),
		glm::vec3(-10.0f,0.0f,-4.0f),
		glm::vec3(-10.0f,0.0f,-3.0f),
		glm::vec3(-10.0f,0.0f,-2.0f),
		glm::vec3(-10.0f,0.0f,-1.0f),
		glm::vec3(-10.0f,0.0f,0.0f),
		glm::vec3(-10.0f,0.0f,1.0f),
		glm::vec3(-10.0f,0.0f,2.0f),
		glm::vec3(-10.0f,0.0f,3.0f),
		glm::vec3(-10.0f,0.0f,4.0f),
		glm::vec3(-10.0f,0.0f,5.0f),
		glm::vec3(-10.0f,0.0f,6.0f),
		glm::vec3(-10.0f,0.0f,7.0f),
		glm::vec3(-10.0f,0.0f,8.0f),
		glm::vec3(-10.0f,0.0f,9.0f),
		glm::vec3(-10.0f,0.0f,10.0f),
		glm::vec3(-9.0f,0.0f,10.0f),
		glm::vec3(-8.0f,0.0f,10.0f),
		glm::vec3(-7.0f,0.0f,10.0f),
		glm::vec3(-6.0f,0.0f,10.0f),
		glm::vec3(-5.0f,0.0f,10.0f),
		glm::vec3(-4.0f,0.0f,10.0f),
		glm::vec3(-3.0f,0.0f,10.0f),
		glm::vec3(-2.0f,0.0f,10.0f),
		glm::vec3(-1.0f,0.0f,10.0f),
		glm::vec3(0.0f,0.0f,10.0f),
		glm::vec3(1.0f,0.0f,10.0f),
		glm::vec3(2.0f,0.0f,10.0f),
		glm::vec3(3.0f,0.0f,10.0f),
		glm::vec3(4.0f,0.0f,10.0f),
		glm::vec3(5.0f,0.0f,10.0f),
		glm::vec3(6.0f,0.0f,10.0f),
		glm::vec3(7.0f,0.0f,10.0f),
		glm::vec3(8.0f,0.0f,10.0f),
		glm::vec3(9.0f,0.0f,10.0f),
		glm::vec3(10.0f,0.0f,10.0f),
		glm::vec3(10.0f,0.0f,9.0f),
		glm::vec3(10.0f,0.0f,8.0f),
		glm::vec3(10.0f,0.0f,7.0f),
		glm::vec3(10.0f,0.0f,6.0f),
		glm::vec3(10.0f,0.0f,5.0f),
		glm::vec3(10.0f,0.0f,4.0f),
		glm::vec3(10.0f,0.0f,3.0f),
		glm::vec3(10.0f,0.0f,2.0f),
		glm::vec3(10.0f,0.0f,1.0f),
		glm::vec3(10.0f,0.0f,0.0f),
		glm::vec3(10.0f,0.0f,-1.0f),
		glm::vec3(10.0f,0.0f,-2.0f),
		glm::vec3(10.0f,0.0f,-3.0f),
		glm::vec3(10.0f,0.0f,-4.0f),
		glm::vec3(10.0f,0.0f,-5.0f),
		glm::vec3(10.0f,0.0f,-6.0f),
		glm::vec3(10.0f,0.0f,-7.0f),
		glm::vec3(10.0f,0.0f,-8.0f),
		glm::vec3(10.0f,0.0f,-9.0f),
		glm::vec3(10.0f,0.0f,-10.0f),
		glm::vec3(9.0f,0.0f,-10.0f),
		glm::vec3(8.0f,0.0f,-10.0f),
		glm::vec3(7.0f,0.0f,-10.0f),
		glm::vec3(6.0f,0.0f,-10.0f),
		glm::vec3(5.0f,0.0f,-10.0f),
		glm::vec3(4.0f,0.0f,-10.0f),
		glm::vec3(3.0f,0.0f,-10.0f),
		glm::vec3(-3.0f,0.0f,-10.0f),
		glm::vec3(-4.0f,0.0f,-10.0f),
		glm::vec3(-5.0f,0.0f,-10.0f),
		glm::vec3(-6.0f,0.0f,-10.0f),
		glm::vec3(-7.0f,0.0f,-10.0f),
		glm::vec3(-8.0f,0.0f,-10.0f),
		glm::vec3(-9.0f,0.0f,-10.0f),
		glm::vec3(-10.0f,0.0f,-10.0f),
	};
	int x, y;
	for (auto position : wallPositions) {
		x = round(position[0]);
		y = round(position[2]);
		gameMap[mapCenterX + x][mapCenterY + y] = WALL_HEIGHT;
	}
	ResourceManager::LoadTexture("textures/woodbox.jpg", GL_FALSE, "woolenbox");

	//3d
    ResourceManager::LoadShader("shaders/3d.vert", "shaders/common.frag", nullptr, "scene");
    ResourceManager::GetShader("scene").Use().SetInteger("image", 0);
    ResourceManager::LoadTexture("textures/arrakisday_dn.jpg", GL_FALSE, "ground");
    Shader sceneShader = ResourceManager::GetShader("scene");
    SpriteRenderer* Renderer3d = new SpriteRenderer(sceneShader, 3);

	// texture
    Texture2D crosshair = ResourceManager::GetTexture("crosshair");
    Texture2D ground = ResourceManager::GetTexture("ground");
	Texture2D sky = ResourceManager::GetTexture("sky");
	Texture2D woolenbox = ResourceManager::GetTexture("woolenbox");
	Texture2D funny = ResourceManager::GetTexture("funny");

	camera.initMap(gameMap, mapCenterX, mapCenterY);
	float startFrame = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.49f, 0.75f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)width/height, 0.1f, 100.0f);
		
		RenderSkybox->DrawSprite(skyboxTexture, view, proj);

		int len = enemyPositions.size();
		double movex = 5 * sin(2*(glfwGetTime() -startFrame));
		enemyPositions[len - 1][0] = 19.0f+movex;
		enemyPositions[len - 2][0] = 17.0f+movex;
		RenderEnemy->DrawSprite(funny, enemyPositions, view, proj, enemyShooteds);

		RenderBox->DrawSprite(woolenbox, boxPositions, view, proj, boxShooted);
		RenderWall->DrawWall(woolenbox,wallPositions, view,proj);
		Renderer3d->DrawSprite(ground, glm::vec3(0, 0, 0), glm::vec2(width*10, height*10), pi/6, glm::vec3(1.0f, 0.0f, 0.0f), view, proj);
		Renderer->DrawSprite(crosshair, glm::vec3(0.0f, 0.0f,0.0f), glm::vec2(width, height));
		
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void shakeEnermy(vector<glm::vec3>& enemy)
{
	int len = enemy.size();
	double movex = 5*sin(glfwGetTime()/1000);
	glm::vec3 movev(movex, 0.0f, 0.0f);
	enemy[len - 1] += movev;
	enemy[len - 2] += movev;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(HIGH, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		camera.ProcessKeyboard(LOW, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, -yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button==GLFW_MOUSE_BUTTON_LEFT&& action == GLFW_PRESS)
	{
		bool hasShooted = false;
		int enemyNum = enemyPositions.size();
		for (int i = 0; i < enemyNum; i++) 
		{
			if (camera.isShooted(enemyPositions[i],1.0f))
			{
				hasShooted = true;
				enemyShooteds[i] = true;
			}
		}
		if (hasShooted)
		{
			SoundEngine->play2D("sounds/shooted.wav", GL_FALSE);
		}
		else {
			SoundEngine->play2D("sounds/shoot.wav", GL_FALSE);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)window;
    (void)xoffset;
    camera.ProcessMouseScroll(yoffset);
}
