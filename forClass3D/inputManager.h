#pragma once
#include "RoboticArm.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <math.h>
#include <algorithm>
#define COLOR_MAIN_CUBE 0
#define PI 3.14159265359

glm::vec2 old_pos;
glm::vec2 curr_pos = glm::vec2(-10.0, -10.0);
glm::vec2 pixel_difference;
int wall_mode = 0;  // control which wall to use if rubik_size>3
int ik_cube_index = NUM_OF_LINKS-1; // determines what cube to work on in the ik solver

RoboticArm robotic_arm;
Cube cube;
Cube* current_object = &cube;
bool is_euler = true; // if true will use ik solver with euler angles
bool solve = false;
bool first_click = false; // it true, this is the first mouse click (used for picking)

void DrawPicking(double x, double y);
void IK_Solver();
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void updatePosition(double xpos, double ypos);
void mouseProccessing(int state);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 mat2euler(glm::mat4 mat);
bool is_eular = false;
struct { GLubyte red, green, blue; } pixel;

float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

void DrawPicking(double x, double y)
{
	float depth;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw all things with the proper picking colors

	cube.DrawPick();
	robotic_arm.Draw(true);

	GLint viewport[4];
	unsigned char data[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	std::cout << "depth : " << depth << std::endl;
	std::cout << "object color: : " << (int)data[0] << std::endl;
	switch ((int)data[0])
	{
	case COLOR_MAIN_CUBE:
		current_object = &cube;
		break;
	case 25:
		current_object = &robotic_arm.getChain()[0];
		break;
	case 51:
		current_object = &robotic_arm.getChain()[1];
		break;
	case 76:
		current_object = &robotic_arm.getChain()[2];
		break;
	case 102:
		current_object = &robotic_arm.getChain()[3];
		break;
	default:
		current_object = nullptr;
		break;
	}
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube.Draw(1);
	robotic_arm.Draw(false);
}

void IK_Solver()
{
	Cube* tip_cube = &robotic_arm.getChain()[NUM_OF_LINKS - 1];
	Cube* curr_cube = &robotic_arm.getChain()[ik_cube_index];
	Cube* first_cube = &robotic_arm.getChain()[0];

	// get the tip of the robotic arm
	glm::mat4 tmp_m = tip_cube->getMVPArm();
	glm::mat4 tmp_m_new = glm::translate(tmp_m , glm::vec3(0, 0, -1.0f));
	glm::vec3 E = glm::vec3(tmp_m_new[3][0], tmp_m_new[3][1], tmp_m_new[3][2]);

	// to get the axis point of the first cube (the start point of the robotic arm)
	tmp_m = first_cube->getMVPArm();
	tmp_m_new = glm::translate(tmp_m, glm::vec3(0, 0, -1.0f));
	glm::vec3 S = glm::vec3(tmp_m_new[3][0], tmp_m_new[3][1], tmp_m_new[3][2]);

	// get the axis point of the current working arm
	tmp_m = curr_cube->getMVPArm();
	tmp_m_new = glm::translate(tmp_m, glm::vec3(0, 0, 1.0f));
	glm::vec3 R = glm::vec3(tmp_m_new[3][0], tmp_m_new[3][1], tmp_m_new[3][2]);

	// get the cube position
	tmp_m = glm::translate(glm::vec3(0.0f, 0.0f, cube.getZoom()))*cube.getMVP();
	glm::vec3 D = glm::vec3(tmp_m[3][0], tmp_m[3][1], tmp_m[3][2]);

	if (is_eular)
	{
		// if too far
		D = glm::vec3(D.x, D.z, -D.y);	//destination
		E = glm::vec3(E.x, E.z, -E.y);	//tip
		R = glm::vec3(R.x, R.z, -R.y);	//axis (last cube tip)
		S = glm::vec3(S.x, S.z, -S.y);


		float dist = glm::distance(S, D);
		glm::vec3 ER_v = glm::normalize(E - R);
		glm::vec3 DR_v = glm::normalize(D - R);
		float t = 0.01;
		float angle = glm::degrees(glm::acos(glm::clamp(glm::dot(ER_v, DR_v), -1.0f, 1.0f)));
		glm::vec3 per = glm::normalize(glm::cross(ER_v, DR_v));
		if (dist > 13)
		{
			std::cout << "Too far " << dist << std::endl;
			return;
		}
		// if too reached
		if (glm::distance(E, D) < 0.3)
		{
			std::cout << "Destination Achieved! " << glm::to_string(glm::distance(E, D)) << std::endl;
			return;
		}
		//if (DR_v.z > 0)
			//per.z = -per.z;
		//per.x = per.x;
		//per.y = per.y;
		//per.z =-per.z;
		glm::mat4 tmp_rotate = glm::rotate(t*angle, per);
		std::cout << "matrix: " << glm::to_string(tmp_rotate) << std::endl;
		glm::vec3 tem_vec = mat2euler(tmp_rotate);
		tem_vec.x = tem_vec.x;
		tem_vec.y = tem_vec.y;
		tem_vec.z = tem_vec.z;
		curr_cube->setLocalEulerAngles(tem_vec);
	}
	else
	{
		// if too far
		D = glm::vec3(D.x, D.z, -D.y);	//destination
		E = glm::vec3(E.x, E.z, -E.y);	//tip
		R = glm::vec3(R.x, R.z, -R.y);	//axis (last cube tip)
		S = glm::vec3(S.x, S.z, -S.y);

		float dist = glm::distance(S, D);
		glm::vec3 ER_v = glm::normalize(E - R);
		glm::vec3 DR_v = glm::normalize(D - R);
		float t = 0.01 * abs(dist);
		float angle = glm::degrees(glm::acos(glm::clamp(glm::dot(ER_v, DR_v), -1.0f, 1.0f)));
		glm::vec3 per = glm::normalize(glm::cross(ER_v, DR_v));
		if (dist > 13)
		{
			std::cout << "Too far " << dist << std::endl;
			return;
		}
		// if too reached
		if (glm::distance(E, D) < 0.3)
		{
			std::cout << "Destination Achieved! " << glm::to_string(glm::distance(E, D)) << std::endl;
			return;
		}
		if (DR_v.z > 0)
			per.x = -per.x;
		curr_cube->setAngles(t*angle, per);
	}

	std::cout << "Distance: " << glm::distance(E, D) << std::endl;
	std::cout << "Distance SD: " << glm::distance(S, D) << std::endl;
	std::cout << "Distance SR: " << glm::distance(S, R) << std::endl;
	std::cout << "Distance RE: " << glm::distance(R, E) << std::endl;
	std::cout << "Distance ED: " << glm::distance(E, D) << std::endl;
	if (ik_cube_index == 0)
	{
		ik_cube_index = NUM_OF_LINKS - 1;
	}
	else
	{
		ik_cube_index--;
	}

	/*std::cout << "   ----   "<< std::endl;
	std::cout << "   ----   " << std::endl;
	std::cout << "   ----   " << std::endl;
	std::cout << "tip " << glm::to_string(E) << std::endl;
	std::cout << "destination : " << glm::to_string(D) << std::endl;
	std::cout << "axis: " << glm::to_string(R) << std::endl;
	std::cout << "   ----   " << std::endl;
	std::cout <<"tip vector: "<<glm::to_string(ER_v) << std::endl;
	std::cout << "destination vector: "<< glm::to_string(DR_v) << std::endl;
	std::cout << "perpendicolar vector: " << glm::to_string(per) << std::endl;
	std::cout <<"angle: "<< angle << std::endl;*/
}

glm::vec3 mat2euler(glm::mat4 mat)
{
	//glm::vec3 angles_vector = glm::vec3(0);
	//float phi = 0;
	//phi = acos(mat[2][2]);

	//// if it's close to zero, make it zero
	//if (abs(phi) < 0.02)
	//{
	//	phi = 0;
	//}
	//angles_vector[1] = glm::degrees(phi);

	//if (phi == 0)
	//{
	//	angles_vector[0] = glm::degrees(acos(clip(mat[1][2], -1, 1)));
	//	angles_vector[2] = 0;
	//}
	//else
	//{
	//	angles_vector[2] = glm::degrees(asin(clip(mat[0][2] / sin(phi), -1, 1)));
	//	angles_vector[0] = glm::degrees(asin(clip(mat[2][0] / sin(phi), -1, 1)));
	//}
	//return angles_vector;


	glm::vec3 angles_vector = glm::vec3(0);
	float phi = 0;
	phi = acos(mat[0][0]);

	// if it's close to zero, make it zero
	if (abs(phi) < 0.2)
	{
		phi = 0;
	}
	angles_vector[1] = glm::degrees(phi);

	if (phi == 0)
	{
		angles_vector[0] = glm::degrees(acos(clip(mat[2][2],-1,1)));
		angles_vector[2] = 0;
	}
	else
	{
		angles_vector[2] = glm::degrees(asin(clip(mat[0][2] / sin(phi), -1, 1)));
		angles_vector[0] = glm::degrees(asin(clip(mat[2][0] / sin(phi), -1, 1)));
	}
	return angles_vector;
} 

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (!first_click)
		{
			first_click = true;
			DrawPicking(xpos, ypos);
		}

		//glReadPixels(xpos, ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
		updatePosition(xpos, ypos);
		mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!first_click)
		{
			first_click = true;
			DrawPicking(xpos, ypos);
		}

		
		//glReadPixels(xpos, ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
		updatePosition(xpos, ypos);
		mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		//std::cout << "LEFT: " << "x: " << xpos << " y: " << ypos << std::endl;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (first_click)
		{
			first_click = false;
		}
		curr_pos = glm::vec2(-10.0, -10.0);

		//std::cout << "RIGHT:  RELEASEEEEEE"  << std::endl;
	}

}

void updatePosition(double xpos, double ypos)
{
	if (curr_pos == glm::vec2(-10.0, -10.0))
	{
		//TODO handle mouse release to set curr_pos back to -10.0
		old_pos = glm::vec2(xpos, ypos);
		curr_pos = glm::vec2(xpos, ypos);
	}
	else
	{
		old_pos = curr_pos;
		curr_pos = glm::vec2(xpos, ypos);
	}
	pixel_difference = curr_pos - old_pos;

}

void mouseProccessing(int state)
{
	float x = 0.5f*pixel_difference[1];
	float z = 0.2f*pixel_difference[0];
	glm::vec3 rotation_vector = glm::vec3(0, z, x);
	// rotate
	if (state == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (old_pos != curr_pos)
		{
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
				{
					current_object->setLocalEulerAngles(rotation_vector);
				}
				else
				{
					robotic_arm.setLocalEulerAngles(rotation_vector, current_object->getChainIndex()-1);
				}
			}
			else
			{
				cube.setGlobalEulerAngles(rotation_vector);
				robotic_arm.setGlobalEulerAngles(rotation_vector);


			}

		}			
	}
	// translate
	else if (state == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (old_pos != curr_pos)
		{
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
					current_object->setGlobalTranslation(glm::vec3(-0.2f*z, 0, 0.1f*x));
				else
					robotic_arm.translateArm(glm::vec3(-0.2f*z, 0, 0.1f*x));
			}
			else
			{
				cube.setGlobalTranslation(glm::vec3(-0.2f*z, 0, 0.1f*x));
				robotic_arm.translateArm(glm::vec3(-0.2f*z, 0, 0.1f*x));

			}
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (current_object != nullptr)
	{
		if (current_object->getChainIndex() == 0)
			current_object->setZoom(yoffset);
		else
			robotic_arm.setZoom(yoffset);
	}
	else
	{
		cube.setZoom(yoffset);
		robotic_arm.setZoom(yoffset);
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::vec3 rotation_vector;
	switch (key)
	{
		case GLFW_KEY_RIGHT:
			rotation_vector = glm::vec3(0, 5, 0);
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
				{
					current_object->setLocalEulerAngles(rotation_vector);
				}
				else
				{
					robotic_arm.setLocalEulerAngles(rotation_vector, current_object->getChainIndex() - 1);
				}
			}
			else
			{
				cube.setGlobalEulerAngles(rotation_vector);
				robotic_arm.setGlobalEulerAngles(rotation_vector);

			}
			break;
		case GLFW_KEY_LEFT:
			rotation_vector = glm::vec3(0, -5, 0);
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
				{
					current_object->setLocalEulerAngles(rotation_vector);
				}
				else
				{
					robotic_arm.setLocalEulerAngles(rotation_vector, current_object->getChainIndex() - 1);
				}
			}
			else
			{
				cube.setGlobalEulerAngles(rotation_vector);
				robotic_arm.setGlobalEulerAngles(rotation_vector);

			}
			break;
		case GLFW_KEY_UP:
			rotation_vector = glm::vec3(0, 0, -5);
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
				{
					current_object->setLocalEulerAngles(rotation_vector);
				}
				else
				{
					robotic_arm.setLocalEulerAngles(rotation_vector, current_object->getChainIndex() - 1);
				}
			}
			else
			{
				cube.setGlobalEulerAngles(rotation_vector);
				robotic_arm.setGlobalEulerAngles(rotation_vector);

			}
			break;
		case GLFW_KEY_DOWN:
			rotation_vector = glm::vec3(0, 0, 5);
			if (current_object != nullptr)
			{
				if (current_object->getChainIndex() == 0)
				{
					current_object->setLocalEulerAngles(rotation_vector);
				}
				else
				{
					robotic_arm.setLocalEulerAngles(rotation_vector, current_object->getChainIndex() - 1);
				}
			}
			else
			{
				cube.setGlobalEulerAngles(rotation_vector);
				robotic_arm.setGlobalEulerAngles(rotation_vector);

			}
			break;
		case GLFW_KEY_E:
			if (action == GLFW_PRESS)
			{
				if (!is_euler)
				{
					std::cout << "Euler angles IK solver" << std::endl;
				}
				else
				{
					std::cout << "Vector rotation IK solver" << std::endl;
				}
				is_euler = !is_euler;
			}

			break;
		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
			{
				current_object->PrintEulerAngles();
			}
			break;
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS)
			{
				solve = !solve;
				//IK_Solver(is_euler);
			}
			break;
		default:
			break;
	}

}
