#include "MapComponent.h"
#include <algorithm>
#include <SDL.h>
#include "Renderer.h"

LW2D::MapComponent::MapComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	std::vector<std::string> map = {
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   0   #.#    ",
		"#####.# ##=## #.#####",
		"     .  #123#  .     ",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "
	};

	ReadMap(map);
}

void LW2D::MapComponent::Render() const
{
	auto pRenderer{ Renderer::GetInstance().GetSDLRenderer() };
	SDL_SetRenderDrawColor(pRenderer, 0, 64, 200, 255);

	// DRAW MAP
	for (int i = 0; i < m_Rows; ++i)
	{
		for (int j = 0; j < m_Cols; ++j)
		{
			if (m_Map[i][j] == Cell::Wall)
			{
				const SDL_Rect rect{ j * m_CellSize, i * m_CellSize, m_CellSize, m_CellSize };
				SDL_RenderFillRect(pRenderer, &rect);
			}
		}
	}
}

bool LW2D::MapComponent::IsWall(const Vector2f& pos) const
{
	if (pos.x < 0 || pos.x > m_Cols * m_CellSize || pos.y < 0 || pos.y > m_Rows * m_CellSize)
		return false;

	float row = (pos.y / m_CellSize);
	float col = (pos.x / m_CellSize);

	for (unsigned char a = 0; a < 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
			case 0: //top left cell
			{
				x = static_cast<short>(floor(col));
				y = static_cast<short>(floor(row));
				break;
			}
			case 1: //top right cell
			{
				x = static_cast<short>(ceil(col));
				y = static_cast<short>(floor(row));
				break;
			}
			case 2: //bottom left cell
			{
				x = static_cast<short>(floor(col));
				y = static_cast<short>(ceil(row));
				break;
			}
			case 3: //bottom right cell
			{
				x = static_cast<short>(ceil(col));
				y = static_cast<short>(ceil(row));
				break;
			}
		}

		if (m_Map[y][x] == Cell::Wall)
			return true;
	}

	return false;
}

void LW2D::MapComponent::ReadMap(const std::vector<std::string>& map)
{
	int currRow{ 0 };
	m_Map.reserve(map.size());
	std::for_each(begin(map), end(map), [&](const std::string& line)
		{
			m_Map.emplace_back(std::vector<Cell>());

			for (int i = 0; i < line.size(); i++)
			{
				switch (line[i])
				{
					default:
					{
						m_Map[currRow].emplace_back(Cell::Empty);
						break;
					}

					case '#':
					{
						m_Map[currRow].emplace_back(Cell::Wall);
						break;
					}
					
					case '.':
					{
						m_Map[currRow].emplace_back(Cell::Pellet);
						break;
					}

					case 'o':
					{
						m_Map[currRow].emplace_back(Cell::PowerPellet);
						break;
					}
				}
			}

			currRow++;
		});

	m_Rows = currRow;
	m_Cols = static_cast<int>(m_Map[0].size());
}