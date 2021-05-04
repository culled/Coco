#pragma once

#include "Core/Base.h"
#include "EventArgs.h"
#include "EventCallback.h"

namespace Coco
{
	template <typename T>
	class COCO_API Event
	{
		static_assert(std::is_convertible<T, EventArgs>::value, "Event must accept args that inherit from EventArgs");

	public:
		/*@brief Connects the given callback to this event. The callback will be managed by this event and should only be used when disconnecting
		* @param instance - The class instance that owns the method
		* @param method - The method that will be called by this event
		*
		* @returns A key to the event callback that can be used to disconnect it
		*/
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		uint32_t Connect(U* instance, void (U::* method)(V*))
		{
			uint32_t key = m_NextKey;
			m_NextKey++;

			Scope <IEventCallback> callback = nullptr;
			callback.reset(new EventCallback<V>(instance, method));
			m_Connections[key] = std::move(callback);

			return key;
		}

		/*@brief Disconnects the given callback from this event. The callback will be destroyed.
		* @param key - The key of the callback to remove. This is created when the associated method is connected
		*/
		void Disconnect(uint32_t key)
		{
			auto it = m_Connections.find(key);

			if (it != m_Connections.end())
			{
				m_Connections.erase(it);
			}
			else
			{
				LOG_CORE_ERROR("Callback was not registered to event.");
			}
		}

		/*@brief Invokes this event and triggers all listener methods*/
		void Invoke(T& args)
		{
			for (auto it = m_Connections.end(); it != m_Connections.begin(); )
			{
				(*--it).second->Invoke(&args);
			}
		}

	private:
		uint32_t m_NextKey;
		std::unordered_map <uint32_t, Scope<IEventCallback>> m_Connections;
	};
}