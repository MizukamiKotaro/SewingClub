#pragma once

#ifdef _DEBUG
#include "Externals/imgui/imgui.h"
#endif // _DEBUG

namespace Kyoko {
	namespace ImGuiManager {
		void Initialize();

		void Finalize();

		void Begin();

		void End();

		void Draw();
	}
}