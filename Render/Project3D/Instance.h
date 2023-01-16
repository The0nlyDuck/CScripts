#pragma once
#include "OBJMesh.h"
#include "Shader.h"
#include "tiny_obj_loader.h"

using glm::vec3;

class Scene;

class Instance
{
public:
	Instance();
	Instance(glm::mat4 transform, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	~Instance();

	void draw(Scene* scene);
	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles = vec3(1), glm::vec3 scale = vec3(1));

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;	
};

