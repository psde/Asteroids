#include <iostream>

#include "FontRenderer.h"

namespace Game
{
	FontRenderer::FontRenderer()
	{
		_shader.addUniversalShader("data/shader/font.glsl");

		std::vector<GLfloat> vertices = {
			0.10f, 0.10f, // 0 top left
			0.90f, 0.10f, // 1 top right
			0.10f, 0.90f, // 2 bottom left
			0.90f, 0.90f, // 3 bottom right
			0.50f, 0.10f, // 4 middle top
			0.50f, 0.90f, // 5 middle bottom
			0.10f, 0.50f, // 6 middle left
			0.90f, 0.50f //  7 middle right
		};

		std::vector<GLuint> elements = {
			0, 1, 3, 2, 0, // 0
			4, 5, // 1
			0, 1, 7, 6, 2, 3, // 2
			0, 1, 7, 6, 7, 3, 2, // 3
			0, 6, 7, 1, 3, // 4
			1, 0, 6, 7, 3, 2, // 5
			1, 0, 2, 3, 7, 6, // 6
			0, 1, 3, // 7
			0, 1, 7, 6, 7, 3, 2, 0, // 8
			7, 1, 0, 6, 7, 3 // 9
		};

		_mesh.reset(new Geometry::Mesh(vertices, elements));

		addCharacter('0', 5);
		addCharacter('1', 2);
		addCharacter('2', 6);
		addCharacter('3', 7);
		addCharacter('4', 5);
		addCharacter('5', 6);
		addCharacter('6', 6);
		addCharacter('7', 3);
		addCharacter('8', 8);
		addCharacter('9', 6);
	}

	void FontRenderer::addCharacter(char c, int size)
	{
		static int offset = 0;
		_characterLookup[c] = std::pair<int, int>(size, offset);
		offset += size;
	}

	void FontRenderer::draw(glm::vec2 position, std::string text, float size)
	{
		_shader.use();
		_shader["size"] = size;

		for (char &c : text)
		{
			_shader["position"] = position;

			auto it = _characterLookup.find(c);

			if (it != std::end(_characterLookup))
			{
				_mesh->draw(GL_LINE_STRIP, it->second.first, it->second.second);
			}

			position.x += size;
		}
	}
}