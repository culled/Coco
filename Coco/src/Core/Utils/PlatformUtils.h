#pragma once
#include "Core/Base.h"
#include <string>

namespace Coco
{
	class COCO_API FileDialog
	{
	public:
		/*@brief Opens a file dialog to open a file
		* 		@returns A path to the file, or an empty string if it was cancelled
		*/
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}