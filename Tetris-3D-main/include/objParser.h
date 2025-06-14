#pragma once

#include <memory>

#include "model.h"

class ObjParser {
public:
	ObjParser() = delete; // abstrakcyjna klasa
	
	static std::shared_ptr<Model> fromFile(const std::string& filename);
};
