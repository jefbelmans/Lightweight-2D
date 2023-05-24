#include "MapComponent.h"

#include <algorithm>
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

bool LW2D::MapComponent::IsWall(const glm::vec2& pos) const
{
	const auto rowCol{GetRowCol(pos)};

	return m_Map[rowCol.first][rowCol.second] == Cell::Wall;
}

bool LW2D::MapComponent::IsPellet(const glm::vec2& pos) const
{
	const auto rowCol{ GetRowCol(pos) };

	return m_Map[rowCol.first][rowCol.second] == Cell::Pellet;
}

void LW2D::MapComponent::CollectPellet(const glm::vec2& pos)
{
	const auto rowCol{ GetRowCol(pos) };

	m_Map[rowCol.first][rowCol.second] = Cell::Empty;
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

std::pair<int, int> LW2D::MapComponent::GetRowCol(const glm::vec2& pos) const
{
	int row{ std::clamp(static_cast<int>(pos.y / m_CellSize), 0, (int)m_Rows - 1) };
	int col{ std::clamp(static_cast<int>(pos.x / m_CellSize), 0, (int)m_Cols - 1) };
	return std::make_pair(row, col);
}
