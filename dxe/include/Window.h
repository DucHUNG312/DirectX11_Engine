#pragma once

#include "DXE.h"

namespace dxe
{
	class Window
	{
	private:
		// singleton manages registration/cleanup of window class
		class WindowClass
		{
		public:
			static const cw16* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();
			DXE_NONCOPYABLE(WindowClass);
		private:
			static constexpr const cw16* wndClassName = L"DXEWindowClass";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};
	public:
		Window(i32 width, i32 height, const cw16* name = L"DirectX11 Engine") noexcept;
		~Window();
		DXE_NONCOPYABLE(Window);
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		i32 width;
		i32 height;
		HWND hWnd;
	};
}
