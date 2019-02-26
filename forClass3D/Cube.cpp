#include "Cube.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <math.h>
#include <cstdlib>
#include "GL/glew.h"



Cube::Cube()
{
	mesh = nullptr;
	shader = nullptr;
	global_rotation = glm::rotate(0.0f,glm::vec3(1.0f));
	local_rotation = glm::rotate(0.0f, glm::vec3(1.0f));
	local_translation = glm::translate(glm::vec3(0.0f));
	global_translation = glm::translate(glm::vec3(0.0f));
	euler_angles = glm::vec3(0);
	phi_local = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0));
	theta_local = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1));
	psi_local = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0));
	phi_global = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0));
	theta_global = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1));
	psi_global = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0));
	scale = glm::scale(glm::vec3(1));
}

Cube::Cube(Mesh* mesh1, Mesh* axis_mesh1, Shader* shader1, Shader* shaderPick, glm::vec3 color_pick)
{
	InitCube(mesh1, axis_mesh1, shader1, shaderPick, color_pick, 0);
}

Cube::~Cube()
{
}

void Cube::InitCube(Mesh * mesh1, Mesh* axis_mesh1, Shader * shader1, Shader * shaderPick, glm::vec3 color_pick, int index)
{

	mesh = mesh1;
	shader = shader1;
	axis_mesh = axis_mesh1;
	global_rotation = glm::rotate(90.0f, glm::vec3(1.0f,0.0f, 0.0f));
	local_rotation = glm::rotate(0.0f, glm::vec3(1.0f));
	local_translation = glm::translate(glm::vec3(0.0f,0.0f,-1.0f));
	global_translation = glm::translate(glm::vec3(0.0f));
	this->shader_pick = shaderPick;
	this->color_pick = color_pick;
	scale = glm::scale(glm::vec3(1));
	euler_angles = glm::vec3(0);
	phi_local = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0));
	theta_local = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1));
	psi_local = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0));
	phi_global = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0));
	theta_global = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1));
	psi_global = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0));
	chain_index = index;
	zoom = -10.0f;
	glm::vec3 pos = glm::vec3(0, 0, -30);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	perspective = glm::perspective(60.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f)* glm::lookAt(pos, pos + forward, up);
}


void Cube::RotateCube(float angle, glm::vec3 rotation_axis)
{
}



void Cube::Draw(int tex_index)
{
	shader->Update(perspective * glm::translate(glm::vec3(0.0f, 0.0f, zoom)) * mvp * scale, local_rotation, 3);
	axis_mesh->Draw_axis();
	shader->Update(perspective * glm::translate(glm::vec3(0.0f, 0.0f, zoom)) * mvp * scale, mvp, tex_index);
	mesh->Draw();
	
}

void Cube::DrawPick()
{
	shader_pick->Bind();
	shader_pick->Update(perspective * glm::translate(glm::vec3(0.0f,0.0f,zoom))* mvp * scale, color_pick);
	mesh->Draw();
}

void Cube::PrintEulerAngles()
{

	std::cout << "Phi Matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << phi_local[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Theta Matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << theta_local[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Psi Matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << psi_local[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

float Cube::getZoom()
{
	return zoom;
}

glm::mat4 Cube::getMVP()
{
	mvp = global_rotation * global_translation * local_rotation * local_translation;
	return  mvp;
	//return perspective * scale * local_translation * global_rotation * local_rotation * global_translation;

}

glm::mat4 Cube::getMVPArm()
{
	return  glm::translate(glm::vec3(0.0f, 0.0f, zoom)) * mvp * scale;
}

glm::mat4 Cube::getLocalRotation()
{
	return this->local_rotation;
}

glm::mat4 Cube::getGlobalRotation()
{
	return this->global_rotation;
}

glm::mat4 Cube::getLocalTranslation()
{
	return this->local_translation;
}

glm::mat4 Cube::getGlobalTranslation()
{
	return this->global_translation;
}

glm::mat4 Cube::getScale()
{
	return this->scale;
}

glm::vec3 Cube::getEulerAngles()
{
	return this->euler_angles;
}

int Cube::getChainIndex()
{
	return this->chain_index;
}

glm::mat4 Cube::getPerspective()
{
	return perspective;
}

glm::vec3 Cube::getColorPick()
{
	return color_pick;
}

glm::mat4 Cube::getRotateNoEular()
{
	return rotate_no_eular;
}

void Cube::setAngles(float angle, glm::vec3 v)
{
	local_rotation = local_rotation * glm::rotate(angle, v);
}

void Cube::setGlobalRotation(glm::mat4 GlobalRotation)
{
	global_rotation = global_rotation * GlobalRotation;
	//tip_position = glm::vec3(global_rotation * glm::vec4(tip_position,1));
}

void Cube::setLocalTranslation(glm::vec3 LocalTranslation)
{
	local_translation = local_translation*glm::translate(LocalTranslation);
	//tip_position = glm::vec3(glm::vec4(tip_position, 1) * local_translation);
}

void Cube::setGlobalTranslation(glm::vec3 GlobalTranslation)
{
	global_translation = glm::translate(GlobalTranslation) * global_translation;
	//tip_position = glm::vec3(glm::vec4(tip_position, 1) * global_translation);
}

void Cube::setZoom(float zoom)
{
	this->zoom += zoom;
}

void Cube::SetMVP(glm::mat4 mvp)
{
	this->mvp = mvp;
}

void Cube::setLocalEulerAngles(glm::vec3 angles)
{
	euler_angles = angles;
	phi_local = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0)) * phi_local;
	theta_local = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1)) * theta_local;
	psi_local = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0)) * psi_local;
	local_rotation = phi_local * theta_local * psi_local;
}

void Cube::setGlobalEulerAngles(glm::vec3 angles)
{
	euler_angles = angles;
	phi_global = glm::rotate(euler_angles[0], glm::vec3(1, 0, 0)) * phi_global;
	theta_global = glm::rotate(euler_angles[1], glm::vec3(0, 0, 1)) * theta_global;
	psi_global = glm::rotate(euler_angles[2], glm::vec3(1, 0, 0)) * psi_global;
	global_rotation = glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * phi_global * theta_global * psi_global;
}

void Cube::setScale(glm::vec3 Scale)
{
	scale = scale*glm::scale(Scale);
}

void Cube::setChainIndex(int index)
{
	this->chain_index = index;
}

void Cube::getRotateNoEular(glm::mat4 rotate_no_eular)
{
	this->rotate_no_eular = rotate_no_eular;
}

