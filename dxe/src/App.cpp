#include "App.h"
#include "DrawableFactory.h"

namespace dxe
{
	App::App()
		: wnd(800, 600, L"DirectX11 Engine")
	{
		DrawableFactory f(wnd.Gfx());
		drawables.reserve(nDrawables);
		std::generate_n(std::back_inserter(drawables), nDrawables, f);

		wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	}

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
		const auto dt = timer.Mark();
		wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
		for (auto& d : drawables)
		{
			d->Update(dt);
			d->Draw(wnd.Gfx());
		}
		wnd.Gfx().EndFrame();
	}
}
