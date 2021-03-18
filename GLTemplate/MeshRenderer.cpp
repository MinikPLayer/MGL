#include "MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include <glm/glm.hpp>
using namespace glm;

void MeshRenderer::RenderMeshGroup(vector<Mesh*>& meshes)
{

}


// Dynamic batching
void MeshRenderer::RenderMeshes(vector<GameObject*>& objects)
{
	// Basically a dictionary
	static vector<Material*> materials;
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

			Mesh* mesh = (Mesh*)objects[i];
			counter++;
			bool found = false;
			for (int j = 0; j < materials.size(); j++)
			{
				if (materials[j] == mesh->GetMaterial())
				{
					meshes[j].push_back(mesh);
					found = true;
					break;
				}
			}
			if (!found)
			{
				// Add to dictionary
				materials.push_back(mesh->GetMaterial());
				vector<Mesh*> newVector;
				newVector.push_back(mesh);
				meshes.push_back(newVector);
			}
		}

		Mesh::updateRenderer = false;
	}

	for (int i = 0; i < materials.size(); i++)
	{
		if (materials[i] == nullptr || materials[i]->shader == nullptr)
			continue;

		GLuint VAO;
		glGenBuffers(1, &VAO);
		/*GLuint* VBOs = new GLuint[meshes[i].size()];
		glGenBuffers(meshes[i].size(), VBOs);

		GLuint* EBOs = new GLuint[meshes[i].size()];
		glGenBuffers(meshes[i].size(), EBOs);*/
		GLuint VBO;
		GLuint EBO;

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		materials[i]->shader->Use();
		materials[i]->__SendToShader();
		//materials[i]->Use();
		/*glBindVertexArray(VAO);
		for (int j = 0; j < meshes[i].size(); j++)
		{
			// Force re-initialization
			meshes[i][j]->initialized = false;

			glBindBuffer(GL_ARRAY_BUFFER, VBO); //VBOs[j]);
			glBufferData(GL_ARRAY_BUFFER, meshes[i][j]->vertexDataSize * sizeof(float), meshes[i][j]->vertexData, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//EBOs[j]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i][j]->indicesSize * sizeof(float), meshes[i][j]->indices, GL_DYNAMIC_DRAW);

			// Vertex pos
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Vertex UV
			//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			//glEnableVertexAttribArray(2);

			// Vertex color - white
			//glVertexAttrib3f(1, 1, 1, 1);
			//glVertexAttrib3f(1, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
			glVertexAttrib3f(1, meshes[i][j]->color.x, meshes[i][j]->color.y, meshes[i][j]->color.z);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, meshes[i][j]->GetPosition().GetGLVector());
			Vector3 pos = meshes[i][j]->GetPosition();
			shaders[i]->SetMat4(shaders[i]->modelLocation, glm::value_ptr(model));

			//glDrawArrays(GL_TRIANGLES, 0, meshes[i][j]->vertexDataSize / 5);
			glDrawElements(GL_TRIANGLES, meshes[i][j]->indicesSize, GL_UNSIGNED_INT, 0);
		}*/
		for (int j = 0; j < meshes[i].size(); j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, meshes[i][j]->GetPosition().GetGLVector());
			//Vector3 pos = meshes[i][j]->GetPosition();
			materials[i]->shader->SetMat4(materials[i]->shader->modelLocation, glm::value_ptr(model));

			meshes[i][j]->__Draw(false);
		}

		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		//delete[] VBOs;
		//delete[] EBOs;

		glDeleteVertexArrays(1, &VAO);


		
	}

	//LOG("Shaders count: ", shaders.size());

	//LOG("Mesh counter: ", counter, ", not meshes: ", notCounter);
}
