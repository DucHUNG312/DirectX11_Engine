#pragma once

#include "DXE.h"
#include "Window.h"
#include "Timer.h"
#include "Box.h"

namespace dxe
{
	class App
	{
	public:
		App();
		~App() = default;
		// master frame / message loop
		i32 Run();
	private:
		void DoFrame();
	private:
		Window wnd;
		Timer timer;
		std::vector<std::unique_ptr<class Box>> boxes;
	};
}
