#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Model {
public:
	Model(
		const std::string& name,
		const std::vector <glm::vec4>& vertices,
		const std::vector <glm::vec4>& normals,
		const std::vector <glm::vec2>& texCoords,
		const std::vector <glm::vec4>& colors,
		int vertexCount
	) :
		name(name),
		vertices(vertices),
		normals(normals),
		texCoords(texCoords),
		colors(colors),
		vertexCount(vertexCount)
	{}

	std::string name;
	std::vector <glm::vec4> vertices;
	std::vector <glm::vec4> normals;
	std::vector <glm::vec2> texCoords;
	std::vector <glm::vec4> colors;
	int vertexCount;
};

