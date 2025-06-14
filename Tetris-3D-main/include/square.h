#pragma once

float squareVertices[] = {
	 1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f,
};

float squareColors[] = {
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

unsigned int squareIndexes[] = {
	0, 2,
	0, 3,
	1, 2,
	1, 3
};

int squareVertexCount = 4;
int squareIndexCount = 8;
