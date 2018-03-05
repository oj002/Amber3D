#pragma once

#include <memory>
#include <string>
#include <functional>
#include "StateMachine.hpp"
#include "../Window/Window.hpp"
#include "../Utils/Clock.hpp"

namespace Amber3D
{
	struct GameData
	{
		GameData(Window& wnd) : window(wnd) {}
		StateMachine machine;
		Window window;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game
	{
	public:
		Game(Window wnd, std::function<void(GameDataRef data)> runState)
			: m_data(std::make_shared<GameData>(wnd))
		{

			runState(this->m_data);

			this->Run();
		}

	private:
		Clock m_clock;

		GameDataRef m_data;

		void Run()
		{

			while (this->m_data->window.is_open())
			{
				this->m_data->machine.ProcessStateChanges();
				float dt = m_clock.restart<float>();

				this->m_data->machine.GetActiveState()->Update(dt);


				this->m_data->machine.GetActiveState()->Render(dt);

			}
		}
	};
}
