#include "Window.h"
#include "resource.h"

namespace dxe
{
	Window::WindowClass Window::WindowClass::wndClass;

	Window::WindowClass::WindowClass() noexcept
		: hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}

	const cw16* Window::WindowClass::GetName() noexcept
	{
		return wndClassName;
	}

	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return wndClass.hInst;
	}

	Window::Window(i32 width, i32 height, const cw16* name /*= L"DirectX11 Engine"*/)
		: width(width), height(height), name(name)
	{
		// calculate window size based on desired client region size
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		{
			throw DXE_LAST_EXCEPT();
		}
		
		// create window & get hWnd
		hWnd = CreateWindow(
			WindowClass::GetName(), name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this
		);

		if (hWnd == nullptr)
		{
			throw DXE_LAST_EXCEPT();
		}

		// show window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	Window::~Window()
	{
		DestroyWindow(hWnd);
	}

	void Window::SetTitle(const std::string& title)
	{
		if (SetWindowTextA(hWnd, title.c_str()) == 0)
		{
			throw DXE_LAST_EXCEPT();
		}
	}

	LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// forward message to window class handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		/// Clear keystate when window loses focus
		case WM_KILLFOCUS:
			keyboard.ClearState();
			break;

		/// Keyboard
		case WM_SYSKEYDOWN: // To track Alt key
		case WM_KEYDOWN:
			if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled())
			{
				keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_SYSKEYUP: // To track Alt key
		case WM_KEYUP:
			keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			keyboard.OnChar(static_cast<unsigned char>(wParam));
			break;

		/// Mouse
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMouseMove(pt.x, pt.y);
			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (mouse.IsInClientRegion(width, height))
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMiddlePressed(pt.x, pt.y);
			break;
		}		
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (!mouse.IsInClientRegion(width, height))
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMiddleReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (!mouse.IsInClientRegion(width, height))
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}	
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (!mouse.IsInClientRegion(width, height))
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
