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
		const f32 c = sin(timer.Peek()) / 2.f + 0.5f;
		wnd.Gfx().ClearBuffer(c, c, 1.0f);
		wnd.Gfx().EndFrame();
	}
}
