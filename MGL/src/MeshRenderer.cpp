#include "MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include <glm/glm.hpp>
using namespace glm;

void MeshRenderer::RenderMeshGroup(vector<Mesh*>& meshes)
{

}

// Dynamic batching
void MeshRenderer::RenderMeshes(vector<shared_ptr<GameObject>>& objects)
{
	// Basically a dictionary
	static vector<Material*> materials; // It does NOT need to be a shared_ptr, and this way it's way faster
	static vector<vector<Mesh*>> meshes;


	if (Mesh::updateRenderer)
	{
		materials.clear();
		meshes.clear();

		int counter = 0;
		int notCounter = 0;
		for (int i = 0; i < objects.size(); i++)
		{
			if (!objects[i]->IsType<Mesh>())
			{
				notCounter++;
				continue;
			}

			Mesh* mesh = (Mesh*)objects[i].get();
			counter++;
			bool found = false;
			for (int j = 0; j < materials.size(); j++)
			{
				if (materials[j] == mesh->GetMaterial().get())
				{
					meshes[j].push_back(mesh);
					found = true;
					break;
				}
			}
			if (!found)
			{
				// Add to dictionary
				materials.push_back(mesh->GetMaterial().get());
				vector<Mesh*> newVector;
				newVector.push_back(mesh);
				meshes.push_back(newVector);
			}
		}

		Mesh::updateRenderer = false;
	}

	for (int i = 0; i < materials.size(); i++)
	{
		if (materials[i] == nullptr || materials[i]->GetShader() == nullptr)
			continue;

		GLuint VAO;
		glGenBuffers(1, &VAO);
		GLuint VBO;
		GLuint EBO;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		materials[i]->GetShader()->Use();
		materials[i]->__SendToShader();
		for (int j = 0; j < meshes[i].size(); j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, meshes[i][j]->GetPosition().GetGLVector());
			materials[i]->GetShader()->SetMat4(materials[i]->GetShader()->modelLocation, glm::value_ptr(model));

			meshes[i][j]->__Draw(shared_ptr<Material>(materials[i]));
		}

		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		glDeleteVertexArrays(1, &VAO);
	}
}
