#pragma once

#include "DXE.h"

namespace dxe
{
	class Mouse
	{
		friend class Window;
	public:
		class Event
		{
		public:
			enum class Type
			{
				LPress,
				LRelease,
				MPress,
				MRelease,
				RPress,
				RRelease,
				WheelUp,
				WheelDown,
				Move,
				Enter, 
				Leave,
				Invalid
			};
		private:
			Type type;
			bool leftIsPressed;
			bool middleIsPressed;
			bool rightIsPressed;
			i32 x;
			i32 y;
		public:
			Event() noexcept
				:
				type(Type::Invalid),
				leftIsPressed(false),
				middleIsPressed(false),
				rightIsPressed(false),
				x(0),
				y(0)
			{}
			Event(Type type, const Mouse& parent) noexcept
				:
				type(type),
				leftIsPressed(parent.leftIsPressed),
				middleIsPressed(parent.middleIsPressed),
				rightIsPressed(parent.rightIsPressed),
				x(parent.x),
				y(parent.y)
			{}
			bool IsValid() const noexcept
			{
				return type != Type::Invalid;
			}
			Type GetType() const noexcept
			{
				return type;
			}
			std::pair<i32, i32> GetPos() const noexcept
			{
				return{ x,y };
			}
			i32 GetPosX() const noexcept
			{
				return x;
			}
			i32 GetPosY() const noexcept
			{
				return y;
			}
			bool LeftIsPressed() const noexcept
			{
				return leftIsPressed;
			}
			bool MiddleIsPressed() const noexcept
			{
				return middleIsPressed;
			}
			bool RightIsPressed() const noexcept
			{
				return rightIsPressed;
			}
		};
	public:
		Mouse() = default;
		DXE_NONCOPYABLE(Mouse);
		std::pair<i32, i32> GetPos() const noexcept;
		i32 GetPosX() const noexcept;
		i32 GetPosY() const noexcept;
		bool IsInWindow() const noexcept;
		bool IsInClientRegion(i32 width, i32 height) const noexcept;
		bool LeftIsPressed() const noexcept;
		bool MiddleIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
		Mouse::Event Read() noexcept;
		bool IsEmpty() const noexcept
		{
			return buffer.empty();
		}
		void Flush() noexcept;
	private:
		void OnMouseMove(i32 x, i32 y) noexcept;
		void OnMouseLeave() noexcept;
		void OnMouseEnter() noexcept;
		void OnLeftPressed(i32 x, i32 y) noexcept;
		void OnLeftReleased(i32 x, i32 y) noexcept;
		void OnMiddlePressed(i32 x, i32 y) noexcept;
		void OnMiddleReleased(i32 x, i32 y) noexcept;
		void OnRightPressed(i32 x, i32 y) noexcept;
		void OnRightReleased(i32 x, i32 y) noexcept;
		void OnWheelUp(i32 x, i32 y) noexcept;
		void OnWheelDown(i32 x, i32 y) noexcept;
		void OnWheelDelta(i32 x, i32 y, i32 delta) noexcept;
		void TrimBuffer() noexcept;
	private:
		static constexpr u32 bufferSize = 16u;
		i32 x;
		i32 y;
		bool leftIsPressed = false;
		bool middleIsPressed = false;
		bool rightIsPressed = false;
		bool isInWindow = false;
		i32 wheelDeltaCarry = 0;
		static std::queue<Event> buffer;
	};
}
