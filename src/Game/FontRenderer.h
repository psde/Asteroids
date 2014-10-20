#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "Shader/Program.h"
#include "Geometry/Mesh.h"

namespace Game
{
	class FontRenderer
	{
	private:

		std::unique_ptr<Geometry::Mesh> _mesh;

		std::map<char, std::pair<int, int>> _characterLookup;

		std::shared_ptr<Shader::Program> _shader;

		void addCharacter(char c, int size);
	public:
		FontRenderer();

		void draw(glm::vec2 position, std::string text, float size);
	};

}