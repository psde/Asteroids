#include <iostream>

#include "FontRenderer.h"

namespace Game
{
	FontRenderer::FontRenderer()
		: m_shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
	{

		std::vector<Math::vec2> vertices;
		for (int i = 0; i < 9; i++)
		{
			Math::vec2 vec(0.1f + 0.4f * (i % 3), 0.1f + 0.4f * (i / 3));
			vertices.push_back(vec);
		}

		auto addCharToElement = [](std::vector<GLuint> &elements, std::vector<GLuint> charElements)
		{
			int offset = elements.size();
			int size = charElements.size();
			for (auto c : charElements)
			{
				elements.push_back(c);
			}
			return std::pair<int, int>(size, offset);
		};

		/* 0 1 2
		   3 4 5
		   6 7 8 */

		auto R = Graphics::GeometryRestartIndex;

		std::vector<GLuint> elements;
		m_characterLookup['0'] = addCharToElement(elements, {0, 2, 8, 6, 0 });
		m_characterLookup['1'] = addCharToElement(elements, {1, 7 });
		m_characterLookup['2'] = addCharToElement(elements, {0, 2, 5, 3, 6, 8 });
		m_characterLookup['3'] = addCharToElement(elements, {0, 2, 5, 3, R, 5, 8, 6 });
		m_characterLookup['4'] = addCharToElement(elements, {0, 3, 5, 2, 8 });
		m_characterLookup['5'] = addCharToElement(elements, {2, 0, 3, 5, 8, 6 });
		m_characterLookup['6'] = addCharToElement(elements, {2, 0, 6, 8, 5, 3 });
		m_characterLookup['7'] = addCharToElement(elements, {0, 2, 8 });
		m_characterLookup['8'] = addCharToElement(elements, {3, 0, 2, 5, 3, 6, 8, 5 });
		m_characterLookup['9'] = addCharToElement(elements, {5, 2, 0, 3, 5, 8 });

		m_characterLookup['.'] = addCharToElement(elements, {4, 7 });

		m_characterLookup['A'] = addCharToElement(elements, {6, 3, 1, 5, 8, R, 5, 3 });
		m_characterLookup['B'] = m_characterLookup['8'];
		m_characterLookup['C'] = addCharToElement(elements, {2, 0, 6, 8 });
		m_characterLookup['D'] = addCharToElement(elements, {1, 0, 6, 7, 5, 1 });
		m_characterLookup['E'] = addCharToElement(elements, {2, 0, 3, 5, R, 3, 6, 8 });
		m_characterLookup['F'] = addCharToElement(elements, {2, 0, 6, R, 5, 3, 6 });
		m_characterLookup['G'] = addCharToElement(elements, {2, 0, 6, 8, 5, 4 });
		m_characterLookup['H'] = addCharToElement(elements, {6, 0, R, 3, 5, R, 2, 8 });
		m_characterLookup['I'] = addCharToElement(elements, {0, 2, R, 1, 7, R, 6, 8 });
		m_characterLookup['J'] = addCharToElement(elements, {0, 2, R, 1, 7, 6 });
		m_characterLookup['K'] = addCharToElement(elements, {0, 6, R, 3, 2, R, 8, 3 });
		m_characterLookup['L'] = addCharToElement(elements, {0, 6, 8 });
		m_characterLookup['M'] = addCharToElement(elements, {6, 0, 4, 2, 8 });
		m_characterLookup['N'] = addCharToElement(elements, {6, 0, 8, 2 });
		m_characterLookup['O'] = m_characterLookup['0'];
		m_characterLookup['P'] = addCharToElement(elements, {6, 0, 2, 5, 3 });
		m_characterLookup['Q'] = addCharToElement(elements, {8, 6, 0, 2, 8, 4 });
		m_characterLookup['R'] = addCharToElement(elements, {6, 0, 2, 5, 3, 8 });
		m_characterLookup['S'] = m_characterLookup['5'];
		m_characterLookup['T'] = addCharToElement(elements, {0, 2, R, 1, 7 });
		m_characterLookup['U'] = addCharToElement(elements, {0, 6, 8, 2 });
		m_characterLookup['V'] = addCharToElement(elements, {0, 3, 7, 5, 2 });
		m_characterLookup['W'] = addCharToElement(elements, {0, 6, 4, 8, 2 });
		m_characterLookup['X'] = addCharToElement(elements, {0, 8, R, 6, 2 });
		m_characterLookup['Y'] = addCharToElement(elements, {0, 4, 2, R, 4, 7 });
		m_characterLookup['Z'] = addCharToElement(elements, {0, 2, 6, 8 });
		m_unknownCharacter = addCharToElement(elements, {0, 2, 8, 6, 0, 8, R, 2, 6});

		// Also allow lower case characters, but use upper case
		for (char c = 'A'; c <= 'Z'; c++)
		{
			m_characterLookup['a' + c - 'A'] = m_characterLookup[c];
		}

		m_mesh.reset(new Graphics::Mesh(vertices, elements));
	}

	FontRenderer &FontRenderer::instance()
	{
		static FontRenderer instance;
		return instance;
	}

	void FontRenderer::draw(Math::vec2 position, std::string text, float size)
	{
		m_shader->use();
		m_shader->uniform("size") = size;

		for (char &c : text)
		{
			m_shader->uniform("position") = position;

			if(c != ' ')
			{
				auto it = m_characterLookup.find(c);

				if (it != std::end(m_characterLookup))
				{
					m_mesh->draw(Graphics::DrawMode::LineStrip, it->second.first, it->second.second);
				}
				else
				{
					m_mesh->draw(Graphics::DrawMode::LineStrip, m_unknownCharacter.first, m_unknownCharacter.second);
				}
			}

			position.x += size;
		}
	}
}
