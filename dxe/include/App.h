#pragma once

#include "DXE.h"
#include "Window.h"
#include "Timer.h"

namespace dxe
{
	class App
	{
	public:
		App();
		// master frame / message loop
		i32 Run();
	private:
		void DoFrame();
	private:
		Window wnd;
		Timer timer;
	};
}
