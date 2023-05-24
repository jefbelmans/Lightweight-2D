#pragma once

namespace LW2D
{
	enum class Direction : short
	{
		Up,
		Down,
		Left,
		Right
	};

	enum class Cell : short
	{
		Empty,
		Wall,
		Door,
		Pellet,
		PowerPellet
	};
}
