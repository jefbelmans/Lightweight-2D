#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>

#include "EngineComponents/Component.h"
#include "../Game Files/Utils.h"
#include "Event.h"

namespace LW2D
{
	class MapComponent final : public Component
	{
	public:
		MapComponent(std::weak_ptr<GameObject> go);
		~MapComponent() = default;

		void Render() const override;

		Cell GetCellFromPos(const glm::vec2& pos) const;
		void CollectPellet(const glm::vec2& pos);

		// GETTERS
		uint8_t GetCellSize() const { return m_CellSize; }
		uint8_t GetRows() const { return m_Rows; }
		uint8_t GetCols() const { return m_Cols; }
		int GetNumPellets() const { return m_NumPellets; }

		Event<int>* GetOnPelletCollected() const { return m_pOnPelletCollected.get(); }
		Event<int>* GetOnPowerPelletCollected() const { return m_pOnPowerPelletCollected.get(); }

		std::pair<int, int> GetIndicesFromPos(const glm::vec2& pos) const;

	private:
		std::vector<std::vector<Cell>> m_Map{};
		int m_NumPellets{};

		const uint8_t m_CellSize{16u};
		uint8_t m_Rows{};
		uint8_t m_Cols{};

		const int m_PelletScore{ 10 };
		const int m_PowerPelletScore{ 50 };

		std::unique_ptr<Event<int>> m_pOnPelletCollected;
		std::unique_ptr<Event<int>> m_pOnPowerPelletCollected;

		void ReadMap(const std::vector<std::string>& map);
	};
}