#include "MapComponent.h"
#include <algorithm>
#include <SDL.h>
#include "Renderer.h"

LW2D::MapComponent::MapComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pOnPelletCollected = std::make_unique<Event<int>>();

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

	// DRAW MAP
	for (int i = 0; i < m_Rows; ++i)
	{
		for (int j = 0; j < m_Cols; ++j)
		{
			if (m_Map[i][j] == Cell::Wall)
			{
				SDL_SetRenderDrawColor(pRenderer, 0, 64, 200, 255);
				const SDL_Rect rect{ j * m_CellSize, i * m_CellSize, m_CellSize, m_CellSize };
				SDL_RenderFillRect(pRenderer, &rect);
			}
			else if (m_Map[i][j] == Cell::Pellet)
			{
				SDL_SetRenderDrawColor(pRenderer, 200, 200, 64, 255);
				SDL_RenderDrawPointF(pRenderer, j * m_CellSize + m_CellSize * 0.5f, i * m_CellSize + m_CellSize * 0.5f);
			}
		}
	}
}

bool LW2D::MapComponent::IsWall(const Vector2f& pos) const
{
	int row{ static_cast<int>(pos.y / m_CellSize) };
	int col{ static_cast<int>(pos.x / m_CellSize) };

	return m_Map[row][col] == Cell::Wall;
}

bool LW2D::MapComponent::IsPellet(const Vector2f& pos) const
{
	int row{ static_cast<int>(pos.y / m_CellSize) };
	int col{ static_cast<int>(pos.x / m_CellSize) };

	return m_Map[row][col] == Cell::Pellet;
}

void LW2D::MapComponent::CollectPellet(const Vector2f& pos)
{
	int row{ static_cast<int>(pos.y / m_CellSize) };
	int col{ static_cast<int>(pos.x / m_CellSize) };

	m_Map[row][col] = Cell::Empty;
	m_pOnPelletCollected->Invoke(m_PelletScore);
}

void LW2D::MapComponent::ReadMap(const std::vector<std::string>& map)
{
	uint8_t currRow{ 0 };
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
	m_Cols = static_cast<uint8_t>(m_Map[0].size());
}