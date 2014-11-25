#pragma once

#include <map>
#include <string>

#include "Math/Math.h"

#include "Graphics/Shader/Program.h"
#include "Graphics/Geometry/Mesh.h"

namespace Game
{
	// Implements a vector font renderer, which can draw a string at any size.
	// Currently only the characters 'A' to 'Z' and '0' to '9' are supported.
	// As each FontRenderer creates its own unique mesh, most applications only
	// need one instance.
	class FontRenderer
	{
	private:

		std::unique_ptr<Graphics::Mesh> _mesh;

		std::map<char, std::pair<int, int>> _characterLookup;

		std::shared_ptr<Graphics::Program> _shader;
	public:
		// Constructor
		FontRenderer();

		// Renders a given string at a given position with a given size
		void draw(Math::vec2 position, std::string text, float size);
	};

}