#include "RoboticArm.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <math.h>
#include <cstdlib>
#include <time.h> 

RoboticArm::RoboticArm()
{

}

void RoboticArm::InitRoboticArm(Mesh * mesh1, Mesh* axis_mesh1, Shader * shader1, Shader * shaderPick)
{
	axis_mesh = axis_mesh1;
	shader = shader1;
	this->shaderPick = shaderPick;
	scale = 1.0f;
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		chain[i].InitCube(mesh1, axis_mesh1, shader1, shaderPick, glm::vec3(0.1*i + 0.1), i + 1);
		chain[i].setScale(glm::vec3(1, 1, 2));
		if (i != 0)
			chain[i].setLocalTranslation(glm::vec3(0,0,-3));
	}
}

RoboticArm::~RoboticArm()
{

}

// mode = 0 regular, mode = 1 picking
void RoboticArm::Draw(bool picking)
{
	//for (int i = 0; i < NUM_OF_LINKS - 1; i++) 
	//{
	//	shader->Update(chain[0].getPerspective() * chain[0].getGlobalRotation()*glm::translate(glm::vec3(0,0,-2))* glm::translate(axis_position[i])* glm::scale(glm::vec3(2 * scale)), glm::mat4(1),0);
	//	axis_mesh->Draw_axis();
	//}
	glm::vec3 angles;
	glm::mat4 mvp_new = glm::translate(glm::vec3(0));// unit matrix

	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		mvp_new =  mvp_new * chain[i].getLocalTranslation() * chain[i].getLocalRotation();
		chain[i].SetMVP(chain[0].getGlobalRotation()*chain[0].getGlobalTranslation() * mvp_new * glm::translate(glm::vec3(0, 0, -2)));
		if (picking)
		{
			chain[i].DrawPick();
		}
		else
		{
			chain[i].Draw(0);
		}
		// update the axis
		if (!picking && i != NUM_OF_LINKS - 1)
		{
			shader->Update(chain[0].getPerspective() *  glm::translate(glm::vec3(0.0f, 0.0f, chain[0].getZoom()))
				* chain[0].getGlobalRotation() * chain[0].getGlobalTranslation() *
				mvp_new * glm::translate(glm::vec3(0, 0, -4)) *
				glm::scale(glm::vec3(2 * scale)), glm::mat4(1), 0);
			axis_mesh->Draw_axis();
		}

	}
}

void RoboticArm::setLocalEulerAngles(glm::vec3 angles, int index)
{
	this->chain[index].setLocalEulerAngles(angles);
}
void RoboticArm::setGlobalEulerAngles(glm::vec3 angles)
{
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		this->chain[i].setGlobalEulerAngles(angles);
	}
}
void RoboticArm::translateArm(glm::vec3 Translation)
{
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		this->chain[i].setGlobalTranslation(Translation);
		if (i != NUM_OF_LINKS-1)
			this->axis_position[i] = glm::vec3(glm::translate(Translation)* glm::vec4(this->axis_position[i],1)) ;
	}
}
void RoboticArm::scaleArm(float Scale)
{
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		this->chain[i].setScale(glm::vec3(Scale));
		std::cout << i << std::endl;
	}
}
void RoboticArm::setZoom(float zoom)
{
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		this->chain[i].setZoom(zoom);
	}
}
void RoboticArm::setScale(float Scale)
{
	if (Scale == -1)
		scale = scale * 0.9;
	else if(Scale == 1)
		scale = scale * 1.1;
	for (int i = 0; i < NUM_OF_LINKS; i++)
	{
		if (Scale == -1)
		{
			chain[i].setScale(glm::vec3(0.9));
		}	
		else if (Scale == 1)
		{
			chain[i].setScale(glm::vec3(1.1));
		}

	}

}

Cube * RoboticArm::getChain()
{
	return chain;
}
