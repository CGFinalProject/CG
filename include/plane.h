#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"

class Plane
{
public:
	int width;
	int height;
	unsigned int planeVAO, planeVBO;
	unsigned int planeTexture;
	Plane(int width, int height) {
		float planeVertices[] = {
			// positions            // normals         // texcoords
			25.0f,  0.0f,  25.0f,  0.0f, 1.0f, 0.0f,	50.0f,  0.0f,
			-25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,

			25.0f,  0.0f,  25.0f,  0.0f, 1.0f, 0.0f, 50.0f,  0.0f,
			-25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
			25.0f,  0.0f, -25.0f,  0.0f, 1.0f, 0.0f,  50.0f, 50.0f
		};
		unsigned int planeVBO;
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
		planeTexture = loadTexture("resources/textures/down.jpg");

	}
		
	unsigned int loadTexture(char const * path)
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

	void show_plane(float Zoom, glm::mat4 viewmatrix, Shader planeshader) {
		planeshader.use();
		planeshader.setInt("texture1", 0);
		//planeshader.setInt("shadowMap", 1);
		glm::mat4 projection = glm::perspective(Zoom, (float)width/ (float)height, 0.1f, 100.0f);
		planeshader.setMat4("projection", projection);
		//glm::mat4 view = glm::mat4(glm::mat3(viewmatrix));
		glm::mat4 view = viewmatrix;
		view = glm::translate(view, glm::vec3(0.0f, -10.0f, 0.0f));
		view = glm::scale(view, glm::vec3(0.8f));
		planeshader.setMat4("view", view);
		// 将光影参数传进着色器
		//planeshader.setVec3("viewPos", camera.Position);
		//planeshader.setVec3("lightPos", lightPos);
		//planeshader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
	}

};

#endif
