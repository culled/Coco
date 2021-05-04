#pragma once
#include "Core/Base.h"
#include "Event.h"
#include "EventArgs.h"
#include "EventCallback.h"
#include "Core/App/Log.h"

namespace Coco
{
	struct COCO_API DispatchedEvent
	{
		EventArgs* Args = nullptr;

		DispatchedEvent() {}
		DispatchedEvent(EventArgs* args) : Args(args) {}
	};

	class COCO_API EventDispatcher
	{
	public:
		using DispatchFunc = std::function<void(DispatchedEvent&)>;

		template<typename T, typename... Args>
		T Invoke(Args... args)
		{
			T eventArgs(std::forward<Args>(args)...);
			DispatchedEvent e(&eventArgs);

			for (auto it = m_Listeners.end(); it != m_Listeners.begin(); )
			{
				(*--it).second(e);

				if (e.Args->Handled)
					break;
			}

			return eventArgs;
		}

		uint32_t Connect(DispatchFunc func)
		{
			uint32_t key = m_NextKey;
			m_NextKey++;
			m_Listeners[key] = func;

			return key;
		}

		void Disconnect(uint32_t key)
		{
			auto it = m_Listeners.find(key);

			if (it != m_Listeners.end())
			{
				m_Listeners.erase(it);
			}
			else
			{
				LOG_CORE_ERROR("Callback was not registered to event dispatcher.");
			}
		}

		template <typename T, typename U, typename = std::enable_if_t<std::is_convertible_v<T&, EventArgs&>>>
		static void Dispatch(DispatchedEvent& e, U* target, void (U::* method)(T*))
		{
			T* args = dynamic_cast<T*>(e.Args);

			if (args != nullptr)
			{
				auto func = std::bind(method, target, args);
				func();
			}
		}

	private:
		uint32_t m_NextKey;
		std::unordered_map <uint32_t, DispatchFunc> m_Listeners;
	};
}