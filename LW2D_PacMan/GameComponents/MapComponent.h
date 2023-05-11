#pragma once
#include <string>
#include <vector>

#include "EngineComponents/Component.h"
#include "../Utils.h"
#include "Event.h"

namespace LW2D
{
	class MapComponent final : public Component
	{
	public:
		MapComponent(std::weak_ptr<GameObject> go);
		~MapComponent() = default;

		void Render() const override;

		bool IsWall(const Vector2f& pos) const;
		bool IsPellet(const Vector2f& pos) const;

		void CollectPellet(const Vector2f& pos);

		// GETTERS

		uint8_t GetCellSize() const { return m_CellSize; }
		uint8_t GetRows() const { return m_Rows; }
		uint8_t GetCols() const { return m_Cols; }
		Event<int>* GetOnPelletCollectedEvent() const { return m_pOnPelletCollected.get(); }

	private:
		std::vector<std::vector<Cell>> m_Map{};

		const uint8_t m_CellSize{16u};
		uint8_t m_Rows{};
		uint8_t m_Cols{};

		const int m_PelletScore{ 50 };

		std::unique_ptr<Event<int>> m_pOnPelletCollected;

		void ReadMap(const std::vector<std::string>& map);
		std::pair<int, int> GetRowCol(const Vector2f& pos) const;
	};
}