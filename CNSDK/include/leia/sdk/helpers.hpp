#pragma once

#include "leia/sdk/api.h"
#include "leia/sdk/enums.hpp"
#include "leia/common/log.h"
#include "leia/common/utils.hpp"
#include "leia/common/jniTypes.h"
#include "leia/headTracking/service/common.hpp"
#include "leia/headTracking/common/frameAdapter.hpp"

namespace leia {

	struct SharedCameraSink;
	struct PlatformInitArgs;
	class AssetManager;

	namespace head { struct Frame; }

} // namespace leia

namespace leia {
namespace sdk {

class ILeiaSDK;

struct LEIASDK_CLASS_API Delegate
{
	LEIASDK_API
	virtual void DidInitialize(ILeiaSDK*);
	LEIASDK_API
	virtual void OnFaceTrackingFatalError(ILeiaSDK*);
};

class LEIASDK_CLASS_API InterlacerDelegate
{
public:
	LEIASDK_API
	virtual void PreInterlace (int width, int height, bool blended);
	LEIASDK_API
	virtual void OnInterlaceRender (int width, int height, bool blended);
	LEIASDK_API
	virtual void PostInterlace (int width, int height, bool blended);
	LEIASDK_API
	virtual void PreSharpen   (int width, int height);
	LEIASDK_API
	virtual void OnSharpenRender   (int width, int height);
	LEIASDK_API
	virtual void PostSharpen   (int width, int height);
};

struct FaceTrackingStateListener {
	/// Face tracking operation has failed and the face tracking backend enountered a fatal error.
	/// An attempt at recovery may be possible by shutting down the face tracking and trying to enable it again.
	virtual void OnFatalError() = 0;
};

class CLeiaSDKFaceTrackingCallback
{
public:
	LEIASDK_API
	virtual void OnFrame(head::Frame const&);
};

LEIASDK_API
const char* ToUiStr(eLeiaInterlaceMode);
LEIASDK_API
const char* ToUiStr(eLeiaShaderDebugMode mode);

template <typename Enum>
std::underlying_type_t<Enum> ToIntegral(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template <typename Enum, typename Int>
typename
std::enable_if<
	std::is_same<
		std::underlying_type_t<Enum>,
		Int
	>::value,
	Enum
>::type
FromIntegral(Int i) {
	return static_cast<Enum>(i);
}

struct RollingAverage {
	std::vector<double> buffer;
	int bufferHead;
	int bufferSize;
	double sum;

	RollingAverage(int windowSize = 60) : buffer(windowSize) {
		Reset();
	}
	double Push(double value) {
		sum += value;

		if (bufferSize < buffer.size()) {
			buffer[bufferHead++] = value;
			bufferSize++;
		} else {
			if (++bufferHead >= buffer.size()) bufferHead = 0;
			sum -= buffer[bufferHead];
			buffer[bufferHead] = value;
		}

		return Get();
	}
	double Get() {
		return sum / bufferSize;
	}
	void Reset() {
		bufferHead = 0;
		bufferSize = 0;
		sum = 0.0;
	}
};

} // namespace sdk
} // namespace leia
