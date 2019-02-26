#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include "RoboticArm.h"
#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GLFW/glfw3.h"
#include "texture.h"


using namespace glm;

#define RED glm::vec3(1,0,0)
//#define ORANGE glm::vec3(0.6,0.21,0)
#define PINK glm::vec3(1.0,0.2,0.6) 
#define BLUE glm::vec3(0,0,1)
#define GREEN glm::vec3(0.4,0.8,0.2)
#define YELLOW glm::vec3(1,0.8,0.1)
//#define WHITE glm::vec3(1,1,1)
#define LIGHT_BLUE glm::vec3(0.6,0.86,0.9)
#define BLACK glm::vec3(0,0,0)
#define COLOR_MAIN_CUBE 0
glm::vec3 axis = glm::vec3(0, 0, 0);

extern RoboticArm robotic_arm;
extern Cube cube;
void DrawPicking(double x, double y);


int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "sneaky snac");

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),RED),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),RED),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),RED),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),RED),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),PINK),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),PINK),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),PINK),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),PINK),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),GREEN),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),GREEN),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),GREEN),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),GREEN),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),LIGHT_BLUE),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),LIGHT_BLUE),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),LIGHT_BLUE),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),LIGHT_BLUE),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),YELLOW),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),YELLOW),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),YELLOW),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),YELLOW),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),BLUE)
	};

	unsigned int indices[] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};

	Vertex axis_vertices[] =
	{
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, -1),RED),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, -1),RED),

		Vertex(glm::vec3(0, 1, 0), glm::vec2(0, 1), glm::vec3(0, 0, -1), GREEN),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(0, 0, -1), GREEN),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),BLUE),
		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, -1),BLUE),
	};

	unsigned int axis_indices[] = {
		0, 1,
		2, 3,
		4, 5,
	};

	Mesh axis_mesh(axis_vertices, sizeof(axis_vertices) / sizeof(axis_vertices[0]), axis_indices, sizeof(axis_indices) / sizeof(axis_indices[0]));
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Shader shader("./res/shaders/basicShader");
	Shader shaderPick("./res/shaders/pickingShader");
	Texture tex_snake("./res/textures/snake.bmp");
	Texture tex_brick("./res/textures/bricks.jpg");
	Texture tex_plane("./res/textures/bricks.jpg");
	
	cube.InitCube(&mesh,&axis_mesh, &shader, &shaderPick, glm::vec3(COLOR_MAIN_CUBE), 0);
	cube.setGlobalTranslation(glm::vec3(-5,0,0));
	robotic_arm.InitRoboticArm(&mesh, &axis_mesh, &shader, &shaderPick);

	glfwSetCursorPosCallback(display.m_window, cursor_position_callback);
	glfwSetScrollCallback(display.m_window, scroll_callback);
	glfwSetKeyCallback(display.m_window, key_callback);

	/*glm::mat4 tmp_rotate = glm::rotate(80.0f, glm::vec3(0.8, 0.2 , 0.1));
	glm::vec3 euler_vec = mat2euler(tmp_rotate);
	glm::mat4 new_rotate = glm::rotate(euler_vec[2], glm::vec3(1, 0, 0)) *
						   glm::rotate(euler_vec[1], glm::vec3(0, 0, 1)) * 
						   glm::rotate(euler_vec[0], glm::vec3(1, 0, 0));*/
	//glm::mat4 tmp_rotate = glm::rotate(90.0f, glm::vec3(1, 0, 0));
	//glm::vec3 euler_vec = mat2euler(tmp_rotate);
	//glm::mat4 new_rotate = glm::rotate(euler_vec[0], glm::vec3(1, 0, 0)) *
	//						glm::rotate(euler_vec[1], glm::vec3(0, 0, 1)) *
	//						glm::rotate(euler_vec[2], glm::vec3(1, 0, 0));
	//std::cout << "the matrix: " << glm::to_string(tmp_rotate) << std::endl;
	//std::cout << "the matrix x0: " << glm::to_string(glm::rotate(euler_vec[0], glm::vec3(1, 0, 0))) << std::endl;
	//std::cout << "the matrix: z" << glm::to_string(glm::rotate(euler_vec[1], glm::vec3(0, 0, 1))) << std::endl;
	//std::cout << "the matrix: x1: " << glm::to_string(glm::rotate(euler_vec[2], glm::vec3(1, 0, 0))) << std::endl;
	//std::cout << "the vector: " << glm::to_string(euler_vec) << std::endl;
	//std::cout << "new_rotate: " << glm::to_string(new_rotate) << std::endl;

	float t = 0.0f;
	while (!glfwWindowShouldClose(display.m_window))
	{
		shader.Bind();
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		t += 0.1;
		Sleep(3);

		if (solve)
		{
			IK_Solver();
		}
		
		tex_plane.Bind(0);
		shader.Update(cube.getPerspective()*glm::rotate(90.0f,glm::vec3(1,0,0))*glm::scale(glm::vec3(10)),glm::mat4(1),0);
		axis_mesh.Draw_axis();
		tex_brick.Bind(0);
		cube.getMVP();
		cube.Draw(1);
		tex_snake.Bind(0);
		robotic_arm.Draw(false);

		display.SwapBuffers();
		glfwPollEvents();
	}
	return 0;
}

