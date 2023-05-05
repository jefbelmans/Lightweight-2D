#pragma once
#include "EngineComponents/Component.h"

#include <string>
#include <vector>
#include "../Utils.h"


namespace LW2D
{
	class MapComponent final : public Component
	{
	public:
		MapComponent(std::weak_ptr<GameObject> go);
		~MapComponent() = default;

		void Render() const override;
		bool IsWall(const Vector2f& pos) const;

	private:
		std::vector<std::vector<Cell>> m_Map{};
		const uint8_t m_CellSize{16u};
		uint8_t m_Rows{};
		uint8_t m_Cols{};

		void ReadMap(const std::vector<std::string>& map);
	};
}