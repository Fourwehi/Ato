#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "model.h"
#include "objParser.h"
#include "Score.h"

Score::Score() {
	this->score = 0;
	loadTextures();
};
Score::~Score() {};

void Score::addScore(int value) {
	this->score += value;
}

void Score::loadTextures() {

	for (const auto& fileName : fileNames) {
		std::shared_ptr<Model> model = ObjParser::fromFile(fileName);
		if (!model) {
			std::cerr << "Failed to load model: " << fileName << ".obj" << std::endl;
			continue;
		}

		this->models.push_back(model);

		std::string texturePath = "numbers/" + fileName + ".png";

		GLuint texID = readTexture(texturePath.c_str());
		if (texID == 0) {
			std::cerr << "Failed to load texture: " << texturePath << std::endl;
			continue;
		}

		this->textures[fileName] = texID;

	}
}


void Score::displayScore(ShaderProgram* sp, glm::mat4* V, glm::mat4* P) {

	std::vector<int> digits = cutScore(this->score);
	//std::reverse(digits.begin(), digits.end());
	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(*P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(*V));
	for (size_t i = 0; i < digits.size(); ++i) {
		int digit = digits[i];
		//printf("%d", digit);
		// Create model matrix
		glm::mat4 M = glm::mat4(1.0f);
		float xOffset = i * 3.0f;
		M = glm::translate(M, glm::vec3(8.0f + xOffset, 10.0f, -5.0f));
		M = glm::scale(M, glm::vec3(6.0f, 6.0f, 6.0f));

		// Upload M matrix
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		// Bind correct vertex data and texture
		glEnableVertexAttribArray(sp->a("vertex"));
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0,
			models[digit]->vertices.data());

		glEnableVertexAttribArray(sp->a("normal"));
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0,
			models[digit]->normals.data());

		glEnableVertexAttribArray(sp->a("texCoord0"));
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0,
			models[digit]->texCoords.data());

		// Texture binding
		glUniform1i(sp->u("mode"), 1);
		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[models[digit]->name]);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, models[digit]->vertexCount);

		// Disable attributes (can skip if reusing same layout)
		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("normal"));
		glDisableVertexAttribArray(sp->a("texCoord0"));
	}
};

std::vector<int> Score::cutScore(int score) {
	std::vector<int> res;
	if (score == 0)
	{
		res.push_back(0);
		return res;
	}
	while (score > 0) {
		res.push_back(score % 10);
		score = score / 10;
	}
	return res;
}
