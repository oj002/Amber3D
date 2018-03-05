#pragma once

#include <memory>
#include <stack>

#include "State.hpp"

namespace Amber3D
{
	typedef std::unique_ptr<State> StateRef;

	class StateMachine
	{
	public:
		StateMachine() {}
		~StateMachine() {}

		void AddState(StateRef newState, bool isReplacing = true)
		{
			this->m_isAdding = true;
			this->m_isReplacing = isReplacing;

			this->m_newState = std::move(newState);
		}

		void Pop_back()
		{
			this->m_isRemoving = true;
		}

		void ProcessStateChanges()
		{
			if (this->m_isRemoving && !this->m_states.empty())
			{
				this->m_states.pop();

				if (!this->m_states.empty())
				{
					this->m_states.top()->Resume();
				}

				this->m_isRemoving = false;
			}

			if (this->m_isAdding)
			{
				if (!this->m_states.empty())
				{
					if (this->m_isReplacing)
					{
						this->m_states.pop();
					}
					else
					{
						this->m_states.top()->Pause();
					}
				}

				this->m_states.push(std::move(this->m_newState));
				this->m_states.top()->Init();
				this->m_isAdding = false;
			}
		}

		StateRef &GetActiveState()
		{
			return this->m_states.top();
		}

	private:
		std::stack<StateRef> m_states;
		StateRef m_newState;

		bool m_isRemoving = false;
		bool m_isAdding = false;
		bool m_isReplacing = true;
	};
}
