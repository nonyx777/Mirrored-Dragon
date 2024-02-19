#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/stb_image.h"
#include "include/Model.hpp"
#include "include/VAO.hpp"
#include "include/VBO.hpp"
#include <random>

// pp_quad vertices (post-processing quad)
float quad_vertices[] = {
	// position //texcoords
	-1.f, 1.f, 0.f, 0.f, 1.f,
	-1.f, -1.f, 0.f, 0.f, 0.f,
	1.f, -1.f, 0.f, 1.f, 0.f,

	-1.f, 1.f, 0.f, 0.f, 1.f,
	1.f, -1.f, 0.f, 1.f, 0.f,
	1.f, 1.f, 0.f, 1.f, 1.f};

float mirror_quad_vertices[] = {
	// position //texcoords
	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	-0.5f, -0.5f, 0.f, 0.f, 0.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,

	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,
	0.5f, 0.5f, 0.f, 1.f, 1.f};

float grass_vertices[] = {
	// position //texcoords
	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	-0.5f, -0.5f, 0.f, 0.f, 0.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,

	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,
	0.5f, 0.5f, 0.f, 1.f, 1.f};

float cloud_vertices[] = {
	// position //texcoords
	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	-0.5f, -0.5f, 0.f, 0.f, 0.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,

	-0.5f, 0.5f, 0.f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f, 0.f,
	0.5f, 0.5f, 0.f, 1.f, 1.f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
float clamp(double min, double max, double value);
float convertAngle(float offset);
unsigned int loadTexture(char const *path);
float getRandomFloat(float min, float max);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
double lastX, lastY;
bool firstMouse = true;
float angle_x{0.f};
float angle_y{0.f};
const float sensetivity_x = 7.f;
const float sensetivity_y = 5.f;
bool can_rotate = false;

// grass rotation variables
const float min_rotation_angle = 0.f;
const float max_rotation_angle = 360.f;

struct ForShader
{
	glm::vec3 view_position = glm::vec3(0.f, 0.f, -30.f);
	glm::vec3 light_position = glm::vec3(0.f, 0.f, 2.f);
	glm::vec3 light_direction = glm::vec3(-1.f, -1.f, -1.f);
};

const float radius = 10.f;

int main()
{
	//....
	ForShader forShader;

	// initialize glfw
	glfwInit();
	if (glfwInit() == GL_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// registering callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// handling mouse movement variables
	lastX = SCR_WIDTH / 2.0;
	lastY = SCR_HEIGHT / 2.0;

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
	stbi_set_flip_vertically_on_load(true);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// grass variables
	unsigned int amount = 50000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	float *grassHeight;
	grassHeight = new float[amount];
	srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		//model.......
		float angle = getRandomFloat(0.f, 360.f);
		float x = getRandomFloat(-40.f, 40.f);
		float z = getRandomFloat(-40.f, 40.f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, -1.f, z));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
		modelMatrices[i] = model;
		//height......
		float height = getRandomFloat(0.f, 3.f);
		grassHeight[i] = height;
	}

	glEnable(GL_DEPTH_TEST);

	//...
	Shader modelShader = Shader("./resource/object.vert", "./resource/object.frag");
	Shader ppquadShader = Shader("./resource/pp_quad.vert", "./resource/pp_quad.frag");
	Shader mirrorShader = Shader("./resource/mirror_quad.vert", "./resource/mirror_quad.frag");
	Shader grassShader = Shader("./resource/grass.vert", "./resource/grass.frag");
	Shader cloudShader = Shader("./resource/cloud.vert", "./resource/cloud.frag");

	// load model
	Model backpack("./resource/objects/backpack/backpack.obj");
	unsigned int grassTexture = loadTexture("./resource/objects/grass/diffuse.png");
	unsigned int cloudTexture = loadTexture("./resource/objects/cloud/cloud1.png");

	ppquadShader.use();
	glUniform1i(glGetUniformLocation(ppquadShader.id, "screenTexture"), 0);
	mirrorShader.use();
	glUniform1i(glGetUniformLocation(mirrorShader.id, "mirrorTexture"), 0);
	grassShader.use();
	glUniform1i(glGetUniformLocation(grassShader.id, "grassTexture"), 0);
	cloudShader.use();
	glUniform1i(glGetUniformLocation(cloudShader.id, "cloudTexture"), 0);

	// vertex buffers and attributes
	// post processing quad
	VAO ppquadVAO = VAO();
	VBO quadVBO = VBO(quad_vertices, sizeof(quad_vertices));
	ppquadVAO.bind();
	ppquadVAO.linkVBO(quadVBO, 0, 1, true);
	ppquadVAO.unbind();
	// mirror quad
	VAO mquadVAO = VAO();
	VBO mquadVBO = VBO(mirror_quad_vertices, sizeof(mirror_quad_vertices));
	mquadVAO.bind();
	mquadVAO.linkVBO(mquadVBO, 0, 1, true);
	mquadVAO.unbind();

	// grass texture
	VAO grassVAO = VAO();
	grassVAO.bind();
	VBO grassVBO1 = VBO(grass_vertices, sizeof(grass_vertices));
	VBO grassVBO2 = VBO(&modelMatrices[0], amount * sizeof(glm::mat4));
	VBO grassVBO3 = VBO(grassHeight, amount * sizeof(float));
	grassVAO.linkVBO(grassVBO1, grassVBO2, grassVBO3, 0, 1, 2, 3, 4, 5, 6);
	grassVAO.unbind();

	// cloud texture
	VAO cloudVAO = VAO();
	cloudVAO.bind();
	VBO cloudVBO = VBO(cloud_vertices, sizeof(cloud_vertices));
	cloudVAO.linkVBO(cloudVBO, 0, 1, true);
	cloudVAO.unbind();

	// framebuffer for post processing
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// generate and attach a color texture buffer to framebuffer
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// generate and attach a renderbuffer fo the framebuffer
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// framebuffer for mirror
	unsigned int mirrorframebuffer;
	glGenFramebuffers(1, &mirrorframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrorframebuffer);
	// generate and attach a color texture buffer to framebuffer
	unsigned int mirrorTextureColorBuffer;
	glGenTextures(1, &mirrorTextureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, mirrorTextureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureColorBuffer, 0);
	unsigned int mirror_rbo;
	glGenRenderbuffers(1, &mirror_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, mirror_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mirror_rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not coplete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//......................................................................................

		// activate quad framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, mirrorframebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.f, 0.5f, 0.6f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene
		modelShader.use();
		forShader.light_position = glm::vec3(-1.f, 1.f, 5.f);

		glm::mat4 model = glm::mat4(1.f);
		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 projection = glm::mat4(1.f);

		model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f));
		view = glm::translate(view, glm::vec3(0.f, 0.f, -8.f));
		view = glm::rotate(view, glm::radians(angle_y) * sensetivity_y, glm::vec3(1.f, 0.f, 0.f));
		view = glm::rotate(view, glm::radians(angle_x) * sensetivity_x, glm::vec3(0.f, 1.f, 0.f));

		projection = glm::perspective(glm::radians(45.f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.f);

		glm::mat4 transform = projection * view * model;

		// Passing uniforms
		// view position(camera position)
		glUniform3fv(glGetUniformLocation(modelShader.id, "view_pos"), 1, glm::value_ptr(forShader.light_position));
		// point light parameters
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.direction"), 1, glm::value_ptr(forShader.light_direction));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.position"), 1, glm::value_ptr(forShader.light_position));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.specular"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
		glUniform1f(glGetUniformLocation(modelShader.id, "material.shininess"), 6.f);

		glUniform1f(glGetUniformLocation(modelShader.id, "light.kc"), 1.f);
		glUniform1f(glGetUniformLocation(modelShader.id, "light.kl"), 0.09f);
		glUniform1f(glGetUniformLocation(modelShader.id, "light.kq"), 0.032f);
		// view matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// model matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// projection matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// projection * view * model
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

		backpack.draw(modelShader);

		grassShader.use();
		grassVAO.bind();
		model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, amount);

		// post processing framebuffer takeover

		//.....................................................................................

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.f, 0.7f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// drawing mirror
		mirrorShader.use();
		mquadVAO.bind();

		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-4.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(70.f), glm::vec3(0.f, 1.f, 0.f));
		model = glm::scale(model, glm::vec3(4.f, 6.f, 1.f));
		view = glm::mat4(1.f);
		view = glm::translate(view, glm::vec3(0.f, 0.f, -30.f));
		view = glm::rotate(view, glm::radians(angle_y) * sensetivity_y, glm::vec3(1.f, 0.f, 0.f));
		view = glm::rotate(view, glm::radians(angle_x) * sensetivity_x, glm::vec3(0.f, 1.f, 0.f));

		glUniformMatrix4fv(glGetUniformLocation(mirrorShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(mirrorShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(mirrorShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, mirrorTextureColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw backpack
		modelShader.use();
		forShader.light_position = glm::vec3(-1.f, 1.f, 5.f);

		model = glm::mat4(1.f);
		view = glm::mat4(1.f);

		model = glm::translate(model, glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, 0.5f, glm::vec3(0.f, 0.7f, 0.f));
		view = glm::translate(view, forShader.view_position);
		view = glm::rotate(view, glm::radians(angle_y) * sensetivity_y, glm::vec3(1.f, 0.f, 0.f));
		view = glm::rotate(view, glm::radians(angle_x) * sensetivity_x, glm::vec3(0.f, 1.f, 0.f));
		projection = glm::perspective(glm::radians(45.f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.f);

		transform = projection * view * model;

		// Passing uniforms
		// view position(camera position)
		glUniform3fv(glGetUniformLocation(modelShader.id, "view_pos"), 1, glm::value_ptr(forShader.light_position));
		// point light parameters
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.direction"), 1, glm::value_ptr(forShader.light_direction));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.position"), 1, glm::value_ptr(forShader.light_position));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
		glUniform3fv(glGetUniformLocation(modelShader.id, "light.specular"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
		glUniform1f(glGetUniformLocation(modelShader.id, "material.shininess"), 6.f);

		glUniform1f(glGetUniformLocation(modelShader.id, "light.kc"), 1.f);
		glUniform1f(glGetUniformLocation(modelShader.id, "light.kl"), 0.09f);
		glUniform1f(glGetUniformLocation(modelShader.id, "light.kq"), 0.032f);
		// view matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// model matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// projection matrix
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// projection * view * model
		glUniformMatrix4fv(glGetUniformLocation(modelShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

		backpack.draw(modelShader);

		// draw grass

		grassShader.use();
		grassVAO.bind();
		model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, grassTexture);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, amount);

		// draw cloud
		cloudShader.use();
		cloudVAO.bind();
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-5.f, 20.f, -50.f));
		glUniformMatrix4fv(glGetUniformLocation(cloudShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(cloudShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cloudShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, cloudTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//........................................................................................................
		// back to the default framebuffer
		// drawing the screen quad for post processing

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		ppquadShader.use();
		ppquadVAO.bind();
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	modelShader.Delete();
	ppquadShader.Delete();
	mirrorShader.Delete();
	grassShader.Delete();
	ppquadVAO.Delete();
	mquadVAO.Delete();
	grassVAO.Delete();
	quadVBO.Delete();
	mquadVBO.Delete();
	grassVBO1.Delete();

	delete[] modelMatrices;
	delete[] grassHeight;

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	lastY = SCR_HEIGHT / 2.0;
	lastX = SCR_WIDTH / 2.0;

	if (!can_rotate)
		return;

	if (firstMouse)
	{
		firstMouse = false;
		return;
	}

	angle_x = convertAngle(xpos - lastX);
	angle_y = convertAngle(lastY - ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		can_rotate = true;
	else
		can_rotate = false;
}

float clamp(double min, double max, double value)
{
	if (value < min)
		return min;
	if (value > max)
		return max;

	return value;
}

float convertAngle(float offset)
{
	// SCR_WIDTH = 360 degrees
	float angle_deg = 360.f * offset / 800.f;
	return glm::radians(angle_deg);
}

unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

float getRandomFloat(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}