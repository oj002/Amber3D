#pragma once

namespace Amber3D
{
	class State
	{
	public:
		virtual void Init() = 0;

		virtual void Update(float dt) = 0;
		virtual void Render(float dt) = 0;

		virtual void Pause() {}
		virtual void Resume() {}
	};
}