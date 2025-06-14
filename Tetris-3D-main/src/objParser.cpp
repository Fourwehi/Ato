#include "objParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

std::shared_ptr<Model> ObjParser::fromFile(const std::string& filename) {
	
	std::string fullPath = "./numbers/" + filename + ".obj";
	std::ifstream file(fullPath);

	if (!file.is_open()) {
		std::cout << "Nie mozna otworzyc pliku: " << fullPath << "\n";
		return nullptr;
	}

	std::vector < std::shared_ptr<Model>> model_list;

	std::string name;
	std::vector< glm::vec4 > final_vertices;
	std::vector< glm::vec2 > final_texCoords;
	std::vector< glm::vec4 > final_normals;
	std::vector< glm::vec4 > colors;

	std::vector< glm::vec4 > raw_vertices;
	std::vector< glm::vec2 > raw_texCoords;
	std::vector< glm::vec4 > raw_normals;

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "o") {
			iss >> name;
		}
		else if (prefix == "v") {
			glm::vec4 v;
			// wczytywanie vertices z pliku
			iss >> v.x >> v.y >> v.z;
			v.w = 1.0f;
			raw_vertices.push_back(v);
		}
		else if (prefix == "vt") {
			glm::vec2 vt;
			// wczytywanie texCords z pliku
			iss >> vt.x >> vt.y;
			raw_texCoords.push_back(vt);
		}
		else if (prefix == "vn") {
			glm::vec4 vn;
			// wczytywanie normals z pliku
			iss >> vn.x >> vn.y >> vn.z;
			vn.w = 0.0f;
			raw_normals.push_back(vn);
		}
		else if (prefix == "f") {
			//wczytywanie faces z pliku
			std::string v1, v2, v3, v4;
			iss >> v1 >> v2 >> v3 >> v4;

			auto parseVertex = [](const std::string& s, int& vi, int& ti, int& ni) {
				std::stringstream ss(s);
				std::string v, t, n;
				std::getline(ss, v, '/');
				std::getline(ss, t, '/');
				std::getline(ss, n, '/');

				vi = std::stoi(v) - 1;
				ti = t.empty() ? -1 : std::stoi(t) - 1;
				ni = n.empty() ? -1 : std::stoi(n) - 1;
			};

			int vi[4], ti[4], ni[4];
			parseVertex(v1, vi[0], ti[0], ni[0]);
			parseVertex(v2, vi[1], ti[1], ni[1]);
			parseVertex(v3, vi[2], ti[2], ni[2]);
			parseVertex(v4, vi[3], ti[3], ni[3]);

			for (int i = 0; i < 3; ++i) {
				final_vertices.push_back(raw_vertices[vi[i]]); // zapisujemy indeks wierzcho³ka
				final_texCoords.push_back(raw_texCoords[ti[i]]);
				final_normals.push_back(raw_normals[ni[i]]);
				colors.push_back(glm::vec4(0.7f, 1.0f, 0.4f, 1.0f));
			}

			final_vertices.push_back(raw_vertices[vi[0]]); // zapisujemy indeks wierzcho³ka
			final_texCoords.push_back(raw_texCoords[ti[0]]);
			final_normals.push_back(raw_normals[ni[0]]);
			colors.push_back(glm::vec4(0.7f, 1.0f, 0.4f, 1.0f));

			final_vertices.push_back(raw_vertices[vi[2]]); // zapisujemy indeks wierzcho³ka
			final_texCoords.push_back(raw_texCoords[ti[2]]);
			final_normals.push_back(raw_normals[ni[2]]);
			colors.push_back(glm::vec4(0.7f, 1.0f, 0.4f, 1.0f));

			final_vertices.push_back(raw_vertices[vi[3]]); // zapisujemy indeks wierzcho³ka
			final_texCoords.push_back(raw_texCoords[ti[3]]);
			final_normals.push_back(raw_normals[ni[3]]);
			colors.push_back(glm::vec4(0.7f, 1.0f, 0.4f, 1.0f));
		}
	}

	auto model = std::make_shared<Model>(
		name,
		final_vertices,
		final_normals,
		final_texCoords,
		colors,
		static_cast<int>(final_vertices.size())
		);

	model_list.push_back(model);

	// Czyszczenie buforów
	raw_vertices.clear();
	raw_texCoords.clear();
	raw_normals.clear();
	final_vertices.clear();
	final_texCoords.clear();
	final_normals.clear();

	return model;
}
