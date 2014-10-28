#include <iostream>

#include "Shader/Manager.h"

#include "FontRenderer.h"

namespace Game
{
	std::pair<int, int> addCharToElement(std::vector<GLuint> &elements, std::vector<GLuint> charElements)
	{
		int offset = elements.size();
		int size = charElements.size();
		for(auto c : charElements)
		{
			elements.push_back(c);
		}
		return std::pair<int, int>(size, offset);
	}

	FontRenderer::FontRenderer()
	: _shader(Shader::Manager::getProgram("data/shader/font.glsl"))
	{

		std::vector<glm::vec2> vertices;
		for(int i = 0; i < 9; i++)
		{
			glm::vec2 vec(0.1f + 0.4f * (i % 3), 0.1f + 0.4f * (i / 3));
			vertices.push_back(vec);
		}

		/* 0 1 2
		   3 4 5
		   6 7 8 */

		std::vector<GLuint> elements;
		_characterLookup['0'] = addCharToElement(elements, {0, 2, 8, 6, 0});
		_characterLookup['1'] = addCharToElement(elements, {1, 7});
		_characterLookup['2'] = addCharToElement(elements, {0, 2, 5, 3, 6, 8});
		_characterLookup['3'] = addCharToElement(elements, {0, 2, 5, 3, 5, 8, 6});
		_characterLookup['4'] = addCharToElement(elements, {0, 3, 5, 2, 8});
		_characterLookup['5'] = addCharToElement(elements, {2, 0, 3, 5, 8, 6});
		_characterLookup['6'] = addCharToElement(elements, {2, 0, 6, 8, 5, 3});
		_characterLookup['7'] = addCharToElement(elements, {0, 2, 8});
		_characterLookup['8'] = addCharToElement(elements, {3, 0, 2, 5, 3, 6, 8, 5});
		_characterLookup['9'] = addCharToElement(elements, {5, 2, 0, 3, 5, 8});

		_characterLookup['A'] = addCharToElement(elements, {6, 3, 1, 5, 8, 5, 3});
		_characterLookup['B'] = _characterLookup['8'];
		_characterLookup['C'] = addCharToElement(elements, {2, 0, 6, 8});
		_characterLookup['D'] = addCharToElement(elements, {1, 0, 6, 7, 5, 1});
		_characterLookup['E'] = addCharToElement(elements, {2, 0, 3, 5, 3, 6, 8});
		_characterLookup['F'] = addCharToElement(elements, {2, 0, 3, 5, 3, 6});
		_characterLookup['G'] = addCharToElement(elements, {2, 0, 6, 8, 5, 4});
		_characterLookup['H'] = addCharToElement(elements, {6, 0, 3, 5, 2, 8});
		_characterLookup['I'] = addCharToElement(elements, {0, 2, 1, 7, 6, 8});
		_characterLookup['J'] = addCharToElement(elements, {0, 2, 1, 7, 6});
		_characterLookup['K'] = addCharToElement(elements, {0, 3, 2, 3, 8, 3, 6});
		_characterLookup['L'] = addCharToElement(elements, {0, 6, 8});
		_characterLookup['M'] = addCharToElement(elements, {6, 0, 4, 2, 8});
		_characterLookup['N'] = addCharToElement(elements, {6, 0, 8, 2});
		_characterLookup['O'] = _characterLookup['0'];
		_characterLookup['P'] = addCharToElement(elements, {6, 0, 2, 5, 3});
		_characterLookup['Q'] = addCharToElement(elements, {8, 6, 0, 2, 8, 4});
		_characterLookup['R'] = addCharToElement(elements, {6, 0, 2, 5, 3, 8});
		_characterLookup['S'] = _characterLookup['5'];
		_characterLookup['T'] = addCharToElement(elements, {0, 2, 1, 7});
		_characterLookup['U'] = addCharToElement(elements, {0, 6, 8, 2});
		_characterLookup['V'] = addCharToElement(elements, {0, 3, 7, 5, 2});
		_characterLookup['W'] = addCharToElement(elements, {0, 6, 4, 8, 2});
		_characterLookup['X'] = addCharToElement(elements, {0, 8, 4, 6, 2});
		_characterLookup['Y'] = addCharToElement(elements, {0, 4, 2, 4, 7});
		_characterLookup['Z'] = addCharToElement(elements, {0, 2, 6, 8});

		_mesh.reset(new Geometry::Mesh(vertices, elements));
	}

	void FontRenderer::draw(glm::vec2 position, std::string text, float size)
	{
		_shader->use();
		_shader->uniform("size") = size;

		for (char &c : text)
		{
			_shader->uniform("position") = position;

			auto it = _characterLookup.find(c);

			if (it != std::end(_characterLookup))
			{
				_mesh->draw(GL_LINE_STRIP, it->second.first, it->second.second);
			}

			position.x += size;
		}
	}
}