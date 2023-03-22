#pragma once
#include <memory>

namespace LW2D
{
	class GenericController final
	{
		class GenericControllerImpl;
		std::unique_ptr<GenericControllerImpl> pImpl;
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonSouth = 0x1000,
			ButtonWest = 0x2000,
			ButtonEast = 0x4000,
			ButtonNorth = 0x8000
		};

		void Update();

		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;

		explicit GenericController(int controllerIndex);
		~GenericController();
	};
}