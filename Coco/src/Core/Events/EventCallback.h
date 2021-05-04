#pragma once
#include "Core/Base.h"
#include "EventArgs.h"

#include <functional>

namespace Coco
{
	class COCO_API IEventCallback
	{
	public:
		virtual void Invoke(EventArgs* args) = 0;
	};

	template <typename T>
	class COCO_API EventCallback : public IEventCallback
	{
	public:
		template <class U>
		EventCallback(U* target, void (U::* method)(T*))
		{
			m_Method = [target, method](T* arg) { (target->*method)(arg); };
		}

		template <class U>
		EventCallback(U* target, void (U::* method)(T*) const)
		{
			m_Method = [target, method](T* arg) { (target->*method)(arg); };
		}

		virtual void Invoke(EventArgs* args) override
		{
			T* param = dynamic_cast<T*>(args);

			if (param)
			{
				m_Method(param);
			}
		}
	private:
		std::function<void(T*)> m_Method;
	};
}