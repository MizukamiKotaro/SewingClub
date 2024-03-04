#pragma once

namespace Kyoko {
	namespace Engine {
		void Initialize(const char* windowName = "LE2A_21_ミズカミ_コタロウ", int width = 1280, int height = 720);

		const bool ProcessMessage();

		void FirstUpdateInLoop();

		void PreDraw();

		void PostDraw();

		void Finalize();
	}
}