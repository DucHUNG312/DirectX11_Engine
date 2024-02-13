#include "App.h"

namespace dxe
{
	App::App()
		: wnd(800, 600, L"DirectX11 Engine")
	{}

	i32 App::Run()
	{
		while (true)
		{
			// process all messages pending, but to not block for new messages
			if (const auto ecode = Window::ProcessMessages())
			{
				// if return optional has value, means we're quitting so return exit code
				return *ecode;
			}

			DoFrame();
		}
	}

	void App::DoFrame()
	{
		const f32 t = timer.Peek();
		std::ostringstream oss;
		oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
		wnd.SetTitle(oss.str());
	}
}
