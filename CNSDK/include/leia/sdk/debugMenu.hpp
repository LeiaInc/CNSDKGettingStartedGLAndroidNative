#pragma once

#include "leia/sdk/gui.hpp"
#include "leia/sdk/helpers.hpp"

#include <functional>

namespace leia {
namespace sdk {

class LeiaSDK;

class ThreadedInterlacer;

struct DebugMenuInitArgs
{
	GuiInitArgs gui;
	std::function<void()> customGui;
};

class DebugMenu
{
public:
	DebugMenu(
		LeiaSDK const&,
		ThreadedInterlacer const&,
		DebugMenuInitArgs const&);
	~DebugMenu();

	bool ToggleGui();
	bool IsGuiVisible();
	bool NewFrame(int width, int height);
	void RenderGui();

#if defined(LEIA_OS_WINDOWS)
	Gui::InputState ProcessInput(GuiSurface surface, uint32_t msg, uint64_t wparam, int64_t lparam);
#elif defined(LEIA_OS_ANDROID)
	Gui::InputState ProcessInput(AInputEvent const *);
	Gui::InputState ProcessMotionInput(JNIEnv *, jobject motionInput);
#endif

private:
	const ThreadedInterlacer &mReadOnlyThreadedInterlacer;
	const LeiaSDK &mReadOnlyLeiaSdk;
	std::function<void()> mCustomGui;
	std::unique_ptr<Gui> mGui;

	RollingAverage mFrameDeltaT;
	RollingAverage mFaceDelay;
	RollingAverage mBlinkProcessingTime;
};

} // namespace sdk
} // namespace leia
