#include "ccpch.h"
#include "AssetManager.h"

namespace Coco
{
	void AssetManager::ReleaseAll()
	{
		AssetMap::iterator it = m_Assets.begin();

		while (it != m_Assets.end())
		{
			it = m_Assets.erase(it);
		}
	}
}