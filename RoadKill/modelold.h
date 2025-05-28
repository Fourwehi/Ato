#pragma once

// modelold.h content
#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <iostream>

#include "vec.h"
#include "resource.h"
#include "shader.h"
#include "utility.h"

using namespace std;

struct vertex {
	vec3 pos;
	vec2 tex;
	vec3 norm;

	vertex() {}

	vertex(const vec3& pos, const vec2& tex, const vec3& norm)
		: pos(pos), tex(tex), norm(norm) {
	}
};

class Model {
private:
	GLuint buffer;
	GLuint size;
	vector<vertex> vertices;

	inline static GLuint fpCanonical;

public:
	inline static void init() {
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		vector<vec2> data;
		data.push_back(vec2(-1.0, -1.0));
		data.push_back(vec2(0.0, 0.0));
		data.push_back(vec2(1.0, -1.0));
		data.push_back(vec2(1.0, 0.0));
		data.push_back(vec2(-1.0, 1.0));
		data.push_back(vec2(0.0, 1.0));
		data.push_back(vec2(1.0, 1.0));
		data.push_back(vec2(1.0, 1.0));

		glGenBuffers(1, &fpCanonical);
		glBindBuffer(GL_ARRAY_BUFFER, fpCanonical);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * data.size(), &data[0], GL_STATIC_DRAW);
		data.clear();
	}

	inline static void drawFramePassCanonical() {
		glBindBuffer(GL_ARRAY_BUFFER, fpCanonical);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2) * 2, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2) * 2, (void*)sizeof(vec2));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	inline Model() {}

	inline void addBox(vec3 pos1, vec3 pos2) {
		vec3 pos111 = pos1;
		vec3 pos112 = vec3(pos1.x, pos1.y, pos2.z);
		vec3 pos121 = vec3(pos1.x, pos2.y, pos1.z);
		vec3 pos122 = vec3(pos1.x, pos2.y, pos2.z);
		vec3 pos211 = vec3(pos2.x, pos1.y, pos1.z);
		vec3 pos212 = vec3(pos2.x, pos1.y, pos2.z);
		vec3 pos221 = vec3(pos2.x, pos2.y, pos1.z);
		vec3 pos222 = pos2;

		vec3 nu = vec3(0.0, 0.0, 1.0);
		vec3 nd = vec3(0.0, 0.0, -1.0);
		vec3 nr = vec3(1.0, 0.0, 0.0);
		vec3 nl = vec3(-1.0, 0.0, 0.0);
		vec3 nf = vec3(0.0, 1.0, 0.0);
		vec3 nb = vec3(0.0, -1.0, 0.0);

		vec2 lt = vec2(0.0, 0.0);
		vec2 rt = vec2(1.0, 0.0);
		vec2 lb = vec2(0.0, 1.0);
		vec2 rb = vec2(1.0, 1.0);

		addPoint(pos111, lt, nl); addPoint(pos112, rt, nl); addPoint(pos121, lb, nl);
		addPoint(pos121, lb, nl); addPoint(pos112, rt, nl); addPoint(pos122, rb, nl);

		addPoint(pos112, lt, nu); addPoint(pos212, rt, nu); addPoint(pos122, lb, nu);
		addPoint(pos122, lb, nu); addPoint(pos212, rt, nu); addPoint(pos222, rb, nu);

		addPoint(pos212, lt, nr); addPoint(pos211, rt, nr); addPoint(pos222, lb, nr);
		addPoint(pos222, lb, nr); addPoint(pos211, rt, nr); addPoint(pos221, rb, nr);

		addPoint(pos211, lt, nd); addPoint(pos111, rt, nd); addPoint(pos221, lb, nd);
		addPoint(pos221, lb, nd); addPoint(pos111, rt, nd); addPoint(pos121, rb, nd);

		addPoint(pos211, lt, nb); addPoint(pos212, rt, nb); addPoint(pos111, lb, nb);
		addPoint(pos111, lb, nb); addPoint(pos212, rt, nb); addPoint(pos112, rb, nb);

		addPoint(pos221, lt, nf); addPoint(pos121, rt, nf); addPoint(pos222, lb, nf);
		addPoint(pos222, lb, nf); addPoint(pos121, rt, nf); addPoint(pos122, rb, nf);
	}

	inline void addPoint(vec3 pos, vec2 tex, vec3 norm) {
		vertices.push_back(vertex(pos, tex, norm));
	}

	inline void load(const char* fn) { load(fn, NULL); }

	inline void load(const char* fn, const char* group) {
		ifstream f(fn);
		vector<vec3> v(1, vec3(0.0, 0.0, 0.0));
		vector<vec2> vt(1, vec2(0.0, 0.0));
		vector<vec3> vn(1, vec3(0.0, 0.0, 0.0));
		bool skip = true;

		while (!f.eof()) {
			string buf;
			getline(f, buf);
			if (buf.empty()) continue;

			vector<string> t = tokenize(buf, " ");
			string flag = t[0];

			if (flag == "#") continue;
			else if (flag == "v") v.push_back(vec3(stof(t[1]), stof(t[2]), stof(t[3])));
			else if (flag == "vt") vt.push_back(vec2(stof(t[1]), stof(t[2])));
			else if (flag == "vn") vn.push_back(vec3(stof(t[1]), stof(t[2]), stof(t[3])));
			else if (flag == "g") {
				if (group) {
					if (skip) {
						if (string(group) == t[1]) skip = false;
						else continue;
					}
					else break;
				}
			}
			else if (flag == "f") {
				if (skip && group) continue;

				bool quad = ( t.size() == 5 );
				auto parse = [](const string& s) {
					auto p = tokenize(s, "/");
					return vertex(vec3(), vec2(), vec3());
					};
				auto p1 = tokenize(t[1], "/"), p2 = tokenize(t[2], "/"), p3 = tokenize(t[3], "/");
				vertex v1(v[stoi(p1[0])], vt[stoi(p1[1])], vn[stoi(p1[2])]);
				vertex v2(v[stoi(p2[0])], vt[stoi(p2[1])], vn[stoi(p2[2])]);
				vertex v3(v[stoi(p3[0])], vt[stoi(p3[1])], vn[stoi(p3[2])]);
				vertices.push_back(v1); vertices.push_back(v2); vertices.push_back(v3);
				if (quad) {
					auto p4 = tokenize(t[4], "/");
					vertex v4(v[stoi(p4[0])], vt[stoi(p4[1])], vn[stoi(p4[2])]);
					vertices.push_back(v3); vertices.push_back(v4); vertices.push_back(v1);
				}
			}
		}

		f.close();
		cout << "Model: " << fn << " loaded with " << vertices.size() << " vertices" << endl;
		generate();
	}

	inline void generate() {
		size = vertices.size();
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
		vertices.clear();
	}

	inline void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)( sizeof(vec3) ));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)( sizeof(vec3) + sizeof(vec2) ));
		glDrawArrays(GL_TRIANGLES, 0, size);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
};
