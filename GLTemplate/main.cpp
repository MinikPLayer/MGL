#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Util.h"
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Cube.h"
#include "UserScript.h"
#include "Time.h"
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float texMix = 0.2f;
float window_width = 1280;
float window_height = 720;

float lastFrame = 0.0f;

Camera camera(vec3(0, 0, -3));

float vert_cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

const float vert_triangle[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

const float vert_triangle2[] = {
	// Second triangle
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f
};

const unsigned int indicies_triangle[] = {
	0, 1, 2,
	3, 4, 5
};

float vert_rect[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices_rect[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3,    // second triangle
};

float vertCol_triangle[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

float vertColUV_rectangle[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

void FBSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	camera.ProcessKeyboard(Camera::Directions::FORWARD, Time::deltaTime * glfwGetKey(window, GLFW_KEY_W));
	camera.ProcessKeyboard(Camera::Directions::BACKWARD, Time::deltaTime * glfwGetKey(window, GLFW_KEY_S));
	camera.ProcessKeyboard(Camera::Directions::LEFT, Time::deltaTime * glfwGetKey(window, GLFW_KEY_A));
	camera.ProcessKeyboard(Camera::Directions::RIGHT, Time::deltaTime * glfwGetKey(window, GLFW_KEY_D));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX;
	static float lastY;
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float x = xpos - lastX;
	float y = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(x, y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return 2;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1280, 720);
	
	// Setup input
	glfwSetFramebufferSizeCallback(window, FBSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	



	//LoadShaders("VertexShader.vert", "FragmentShader.frag");
	/*GLuint shader;
	if (!LoadShaders("VertexShader.vert", "FragmentShader.frag", shader))
		return 3;

	GLuint shader2;
	if (!LoadShaders("VertexShader.vert", "FragmentShader2.frag", shader2))
		return 4;*/

	Shader shader("VertexShader.vert", "FragmentShader.frag");

	// Generate and use VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Setup some verticies
	GLuint VBO;
	glGenBuffers(1, &VBO);	// Generate buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// Bind buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertCol_triangle), vertCol_triangle, GL_DYNAMIC_DRAW); // Copy triangle verticies to buffer

	// Specify program we're using and vertex attributes
	//glUseProgram(shader);
	shader.Use();
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	GLuint VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	
	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_triangle2), vert_triangle2, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttrib3f(1, 0.0f, 0.5f, 1.0f);


	GLuint VAO_rect;
	glGenVertexArrays(1, &VAO_rect);
	glBindVertexArray(VAO_rect);

	GLuint VBO_rect;
	glGenBuffers(1, &VBO_rect);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_cube), vert_cube, GL_STATIC_DRAW);

	/*GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_rect), indices_rect, GL_STATIC_DRAW);*/

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttrib3f(1, 1, 1, 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//GLint vertexColorLocation = glGetUniformLocation(shader2, "vertColor");

	// Load texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (!data)
	{
		cout << "Failed to load container texture!" << endl;
		return 3;
	}

	GLuint textures[2];
	glGenTextures(2, textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (!data)
	{
		cout << "Failed to load awesome face texture!" << endl;
		return 3;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	shader.Use();
	shader.SetInt("objTexture", 0);
	shader.SetInt("objTexture2", 1);
	shader.SetFloat("scale", 1);
	



	//shader.SetMat4("transform", glm::value_ptr(trans));
	//GLint transformLocation = shader.GetUniformLocation("transform");
	GLint modelLocation = shader.GetUniformLocation("model");
	GLint viewLocation = shader.GetUniformLocation("view");
	GLint projectionLocation = shader.GetUniformLocation("projection");

	cout << "Ready! " << endl << "Starting drawing loop..." << endl;

	//Cube** cubes = new Cube*[10];
	/*for (int i = 0; i < 10; i++)
	{
		//cubes[i] = new Cube(&shader);
		//cubes[i]->pos = cubePositions[i];
		Cube* cube = Instantiate<Cube>(&shader);
		cube->pos = cubePositions[i];
	}*/
	for (int i = 0; i < 1; i++)
	{
		//UserScript* script = Instantiate<UserScript>();
		//script->SetPosition(cubePositions[i]);
		UserScript* s = new UserScript();
		s->SetPosition(cubePositions[i]);
		Instantiate<UserScript>(s);
		
		vector<GameObject*> components = s->GetComponents();
		cout << "Comp size: " << components.size() << endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		Time::deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);

		shader.Use();
		//shader.SetFloat("scale", sin(glfwGetTime()));
		shader.SetVec2("uvScale", sin(glfwGetTime()), 1);
		shader.SetFloat("texMix", texMix);

		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(0.5, -0.5f, 0.0f));
		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() * 45), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		//shader.SetMat4(transformLocation, glm::value_ptr(trans));



		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/
		//glBindVertexArray(VAO_rect);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);




		glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)window_width / (float)window_height, 0.1f, 100.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix(); //glm::lookAt(camera.position, camera.position + camera.front, camera.worldUp); //glm::translate(view, glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z));

		shader.SetMat4(viewLocation, glm::value_ptr(view));
		shader.SetMat4(projectionLocation, glm::value_ptr(projection));

		/*for (int i = 0; i < 10; i++)
		{
			//glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, cubePositions[i]);
			//model = glm::rotate(model, glm::radians(20.0f) * (float)glfwGetTime() * (i - 5), glm::vec3(0.5f, 1.0f, 0.0f));

			//shader.SetMat4(modelLocation, glm::value_ptr(model));

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			cubes[i]->Draw();
		}*/

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == nullptr) 
				continue;

			objects[i]->Update();
			vector<GameObject*> components = objects[i]->GetComponents();
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->Update();
			}
		}

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == nullptr) 
				continue;

			objects[i]->Draw();
			vector<GameObject*> components = objects[i]->GetComponents();

			for (int i = 0; i < components.size(); i++)
			{
				components[i]->Draw();
			}
		}

		//trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(-0.5, 0.5f, 0.0f));
		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() * 45), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		//shader.SetMat4(transformLocation, glm::value_ptr(trans));

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}