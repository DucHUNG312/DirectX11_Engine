#include "Mouse.h"

namespace dxe
{
	std::queue<Mouse::Event> Mouse::buffer{};

	std::pair<i32, i32> Mouse::GetPos() const noexcept
	{
		return { x,y };
	}

	i32 Mouse::GetPosX() const noexcept
	{
		return x;
	}

	i32 Mouse::GetPosY() const noexcept
	{
		return y;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return isInWindow;
	}

	bool Mouse::IsInClientRegion(i32 width, i32 height) const noexcept
	{
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	bool Mouse::LeftIsPressed() const noexcept
	{
		return leftIsPressed;
	}

	bool Mouse::MiddleIsPressed() const noexcept
	{
		return middleIsPressed;
	}

	bool Mouse::RightIsPressed() const noexcept
	{
		return rightIsPressed;
	}

	Mouse::Event Mouse::Read() noexcept
	{
		if (buffer.size() > 0u)
		{
			Mouse::Event e = buffer.front();
			buffer.pop();
			return e;
		}
		else
		{
			return Mouse::Event();
		}
	}

	void Mouse::Flush() noexcept
	{
		buffer = std::queue<Event>();
	}

	void Mouse::OnMouseMove(i32 newx, i32 newy) noexcept
	{
		x = newx;
		y = newy;

		buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseLeave() noexcept
	{
		isInWindow = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseEnter() noexcept
	{
		isInWindow = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::Enter , *this));
		TrimBuffer();
	}

	void Mouse::OnLeftPressed(i32 x, i32 y) noexcept
	{
		leftIsPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftReleased(i32 x, i32 y) noexcept
	{
		leftIsPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnMiddlePressed(i32 x, i32 y) noexcept
	{
		middleIsPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
		TrimBuffer();
	}

	void Mouse::OnMiddleReleased(i32 x, i32 y) noexcept
	{
		middleIsPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnRightPressed(i32 x, i32 y) noexcept
	{
		rightIsPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
		TrimBuffer();
	}

	void Mouse::OnRightReleased(i32 x, i32 y) noexcept
	{
		rightIsPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelUp(i32 x, i32 y) noexcept
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelDown(i32 x, i32 y) noexcept
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelDelta(i32 x, i32 y, i32 delta) noexcept
	{
		wheelDeltaCarry += delta;
		// generate events for every WHEEL_DELTA 
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUp(x, y);
		}
		while (wheelDeltaCarry <= -WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelDown(x, y);
		}
	}

	void Mouse::TrimBuffer() noexcept
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}
