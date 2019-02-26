#pragma once

#include "glm/glm.hpp"
#include "mesh.h"
#include "shader.h"


//#define RED glm::vec3(1,0,0)
////#define ORANGE glm::vec3(0.6,0.21,0)
//#define PINK glm::vec3(1.0,0.2,0.6) 
//#define BLUE glm::vec3(0,0,1)
//#define GREEN glm::vec3(0.4,0.8,0.2)
//#define YELLOW glm::vec3(1,0.8,0.1)
////#define WHITE glm::vec3(1,1,1)
//#define LIGHT_BLUE glm::vec3(0.6,0.86,0.9)

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 800

class Cube {
	private:
		glm::mat4 perspective;
		glm::mat4 scale;
		glm::vec3 euler_angles;
		glm::mat4 phi_local;
		glm::mat4 theta_local;
		glm::mat4 psi_local;
		glm::mat4 phi_global;
		glm::mat4 theta_global;
		glm::mat4 psi_global;
		glm::mat4 mvp;
		// determined by the scene rotation
		glm::mat4 global_rotation;
		glm::mat4 global_translation;
		// determined by the cube
		glm::mat4 local_translation;
		glm::mat4 local_rotation;
		glm::mat4 rotate_no_eular;
		// use for color picking
		glm::vec3 color_pick;
		// 0 if it is the moving 
		int chain_index;
		float zoom;

	public:
		Mesh* axis_mesh;
		Mesh * mesh;
		Shader* shader;
		Shader* shader_pick;
		Cube();
		Cube(Mesh* mesh1, Mesh* axis_mesh1, Shader* shader1, Shader* shaderPick, glm::vec3 color_pick);
		~Cube();
		void InitCube(Mesh* mesh1, Mesh* axis_mesh1, Shader* shader1, Shader* shaderPick, glm::vec3 color_pick, int index);
		void RotateCube(float angle, glm::vec3 rotation_axis);
		//void RotateCubeByScreen(glm::vec2 old_point, glm::vec2 new_point);
		//void TranslateCube(glm::vec2 translate_pos);
		void Draw(int tex_index);
		void DrawPick();
		void PrintEulerAngles();

		//Getters:
		float getZoom();
		glm::mat4 getMVP();
		glm::mat4 getMVPArm();
		glm::mat4 getLocalRotation();
		glm::mat4 getGlobalRotation();
		glm::mat4 getLocalTranslation();
		glm::mat4 getGlobalTranslation();
		glm::mat4 getScale();
		glm::vec3 getEulerAngles();
		int getChainIndex();
		glm::mat4 getPerspective();
		glm::vec3 getColorPick();
		glm::mat4 getRotateNoEular();


		//Setters:
		void setZoom(float zoom);
		void SetMVP(glm::mat4 mvp);
		void setLocalEulerAngles(glm::vec3 angles);
		void setGlobalEulerAngles(glm::vec3 angles);
		void setAngles(float angle, glm::vec3 v);
		void setGlobalRotation(glm::mat4 GlobalRotation);
		void setLocalTranslation(glm::vec3 LocalTranslation);
		void setGlobalTranslation(glm::vec3 GlobalTranslation);
		void setScale(glm::vec3 Scale);
		void setChainIndex(int index);
		void getRotateNoEular(glm::mat4 rotate_no_eular);
};