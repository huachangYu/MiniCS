/*
Camera��
���ܣ�
��װ�˹��������ϵͳ������йصĹ��ܣ���������������
1.���캯��
2.���view���󣨹۲����
3.��ʼ����Ϸ�еĵ�ͼ�����ڸ���ʵ��ǽ���赲���������ӵ��¼�
4.�����ж��Ƿ����Ŀ��
5.���ڴ�������¼�����Ҫ�ǿ���������ƶ�
6.���ڴ�������ƶ��¼�����Ҫ�ǿ����ӽǵı仯
7.���ڴ����������¼�����Ҫ�ǷŴ���С�ӽ�
8.��������ĸ�������
*/
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;

#define eps 0.0001

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	HIGH,
	LOW
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVTY = 0.05f;
const float JUMPSPEED = 4.5f;
const float JUMPSENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

extern const double WALL_HEIGHT;


class Camera
{
  public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
	glm::vec3 High;
    glm::vec3 WorldUp;
    // Eular Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
	float JumpSpeed;
	float JumpSensitivity;
    float Zoom;
	// gameMap
	vector<vector<double>> gameMap;
	int mapCenterX, mapCenterY;

    // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW, float pitch = PITCH) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), 
	  Zoom(ZOOM),JumpSensitivity(JUMPSENSITIVITY),JumpSpeed(JUMPSPEED),High(glm::vec3(0.0f,1.0f,0.0f))
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
  
  Camera(float posX, float posY, float posZ,
	  float upX, float upY, float upZ, float yaw, float pitch):
	  Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY),
	  Zoom(ZOOM), JumpSensitivity(JUMPSENSITIVITY), JumpSpeed(JUMPSPEED), High(glm::vec3(0.0f, 1.0f, 0.0f))
  {
	  Position = glm::vec3(posX, posY, posZ);
	  WorldUp = glm::vec3(upX, upY, upZ);
	  Yaw = yaw;
	  Pitch = pitch;
	  updateCameraVectors();
  }


    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		glm::vec3 prePosition = Position;
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			Position += Front * velocity;
		}
		if (direction == BACKWARD)
		{
			Position -= Front * velocity;
		}
		if (direction == LEFT)
		{
			Position -= Right * velocity;
		}
		if (direction == RIGHT)
		{
			Position += Right * velocity;
		}
		// ����Ƿ�����������
		/*int px = mapCenterX + round(Position[0]), py = mapCenterY + round(Position[2]);
		if (gameMap[px][py] > Position.y+eps) 
		{
			Position = prePosition;
		}*/

		if (direction == HIGH) 
		{
			Position += High * velocity;
		}
		if (direction == LOW)
		{
			Position -= High * velocity;
			int px = mapCenterX + round(Position[0]), py =mapCenterY + round(Position[2]);
			Position[1] = Position[1] > gameMap[px][py] ? Position[1] : gameMap[px][py];
		}


		//���ܴ�ǽ
		int startX = round(mapCenterX + prePosition[0]), startY = round(mapCenterY + prePosition[2]);
		int endX = round(mapCenterX + Position[0]), endY = round(mapCenterY + Position[2]);
		int mx = endX > startX ? 1 : -1, my = endY > startY ? 1 : -1;
		int dx = abs(endX - startX), dy = abs(endY - startY);
		for (int i = -1; i <= dx+1; i++) {
			for (int j = -1; j <= dy+1; j++) {
				if (gameMap[startX + mx*i][startY + my*j] == WALL_HEIGHT) {
					Position = prePosition;
					break;
				}
			}
		}
		//������������
		Position[0] = Position[0] < 200 ? Position[0] : 200;
		Position[2] = Position[2] < 200 ? Position[2] : 200;
		Position[0] = Position[0] > -200 ? Position[0] : -200;
		Position[2] = Position[2] > -200 ? Position[2] : -200;
	}

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

	bool isShooted(glm::vec3 targetPosion,double tol) 
	{
		glm::vec3 cro = glm::cross(glm::normalize(Front), targetPosion-Position);
		double distance = glm::length(cro);
		//std::cout << distance << std::endl;
		if (distance < tol&&glm::dot(glm::normalize(Front), targetPosion - Position)>0)
			return true;
		else
			return false;
	}

	void initMap(vector<vector<double>> _gameMap, int _mapCenterX,int _mapCenterY) 
	{
		gameMap = _gameMap;
		mapCenterX = _mapCenterX;
		mapCenterY = _mapCenterY;
	}

  private:
    void updateCameraVectors()
    {
        glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
    }
};
