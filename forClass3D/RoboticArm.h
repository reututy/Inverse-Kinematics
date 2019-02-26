#pragma once
#include "glm/glm.hpp"
#include "mesh.h"
#include "shader.h"
#include "Cube.h"

//#define RED glm::vec3(1,0,0)
////#define ORANGE glm::vec3(0.6,0.21,0)
//#define PINK glm::vec3(1.0,0.2,0.6) 
//#define BLUE glm::vec3(0,0,1)
//#define GREEN glm::vec3(0.4,0.8,0.2)
//#define YELLOW glm::vec3(1,0.8,0.1)
////#define WHITE glm::vec3(1,1,1)
//#define LIGHT_BLUE glm::vec3(0.6,0.86,0.9)
//

#define NUM_OF_LINKS 4
//bool is_eular=false;

class RoboticArm {
private:
	Cube chain[NUM_OF_LINKS];
	glm::vec3 axis_position[NUM_OF_LINKS];
	float scale;
public:
	Mesh * axis_mesh;
	Shader * shader;
	Shader * shaderPick;
	RoboticArm();
	~RoboticArm();

	void InitRoboticArm(Mesh * mesh1, Mesh* axis_mesh1, Shader * shader1, Shader * shaderPick);
	//void RotateCubeByScreen(glm::vec2 old_point, glm::vec2 new_point);
	void Draw(bool picking);
	//void DrawPick();
	void setLocalEulerAngles(glm::vec3 angles, int index);
	void setGlobalEulerAngles(glm::vec3 angles);
	void translateArm(glm::vec3 Translation);
	void scaleArm(float Scale);

	//Getters:
	Cube* getChain();


	//Setters:
	void setZoom(float zoom);
	void setScale(float Scale);

};

