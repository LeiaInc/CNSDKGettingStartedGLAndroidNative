#pragma once

#include "leia/common/assert.h"

#include <fstream>
#include <vector>
#include <version>

#include <thread>

#ifdef __ANDROID__
struct AAssetManager;
#endif

namespace leia {

	typedef union PixelInfo
	{
		std::uint32_t Colour;
		struct
		{
			std::uint8_t R, G, B, A;
		};
	} *PPixelInfo;

	class Tga
	{
	private:
		std::vector<std::uint8_t> Pixels;
		bool                      ImageCompressed;
		std::uint32_t             width;
		std::uint32_t             height;
		std::uint32_t             size;
		std::uint32_t             BitsPerPixel;
		std::string               filePath;

	public:
		LEIA_COMMON_API
		Tga();
		LEIA_COMMON_API
		Tga(const char* FilePath);

		std::vector<std::uint8_t> GetPixels() { return this->Pixels; }
		void* GetPixelsBuffer() { return &this->Pixels[0]; }
		std::uint32_t GetWidth() const { return this->width; }
		std::uint32_t GetHeight() const { return this->height; }
		std::string GetFilePath() const { return this->filePath; }
		bool HasAlphaChannel() const  { return BitsPerPixel == 32; }
		std::uint32_t GetBitsPerPixel() const { return BitsPerPixel; }
		LEIA_COMMON_API
		void RemoveColor(unsigned char r, unsigned char b, unsigned char c);
		LEIA_COMMON_API
		void Resize(int newWidth, int newHeight);
		LEIA_COMMON_API
		void ReadFromData(const void* data, bool bottomLeftOrigin = true);
		LEIA_COMMON_API
		void SwapRedAndBlue();
	};

#ifdef _WIN32
	LEIA_COMMON_API
	bool GetNonPrimaryDisplayTopLeftCoordinate(int& x, int& y);
#endif

	LEIA_COMMON_API
	void stringReplace(std::string& src, std::string const& from, std::string const& to);

// Explicitly stop compiler from producing unused argument or variable warnings.
#define LNK_UNUSED(value) (void)value

void ThrowIncorrectThreadException(std::thread::id expectedId, const char* func);

#define LNK_CHECK_THREAD_ID(expectedId) \
if (expectedId != std::this_thread::get_id()) { \
	ThrowIncorrectThreadException(expectedId, LNK_PRETTY_FUNCTION); \
}

#define LNK_CHECK_THREAD_ID_NOTHROW(expectedId) \
if (expectedId != std::this_thread::get_id()) { \
	spdlog::critical("{} being used on incorrect thread.", LNK_PRETTY_FUNCTION); \
}
#ifdef __cpp_return_type_deduction
template <typename T, typename DeleteFunc>
auto WrapResource(T* resource, DeleteFunc deleteFunc) {
    return std::unique_ptr<T, DeleteFunc>(resource, deleteFunc);
}

template <typename T, typename DeleteFunc, typename InitFunc>
auto WrapResource(DeleteFunc deleteFunc, InitFunc initFunc) {
    T* resource = nullptr;
    initFunc(&resource);
    return std::unique_ptr<T, DeleteFunc>(resource, deleteFunc);
}
#endif

template <typename T, typename U>
T* polymorphic_pointer_downcast(U* p) {
    if (p) {
        LNK_ASSERT(dynamic_cast<T*>(p));
    }
    return static_cast<T*>(p);
}

LEIA_COMMON_API
void WaitForDebugger();

// To avoid including <bitset>
template <typename T>
auto CountBits(T number) -> T {
	T ret = 0;
	for (T i = 0; i < sizeof(T) * 8; ++i) {
		if (number & (1 << i)) ret++;
	}
	return ret;
}

struct Version {
	enum {
		kMajor = 0,
		kMinor,
		kPatch,
		kCount
	};
	union {
		struct {
			int major;
			int minor;
			int patch;
		};
		int v[kCount];
	};

	Version();
	Version(int major, int minor);
};

#ifdef __cpp_lib_string_view
bool ParseVersion(std::string_view const&, Version*);
#else
bool ParseVersion(std::string const&, Version*);
#endif

bool operator<(Version const&, Version const&);
bool operator<=(Version const&, Version const&);
bool operator>(Version const&, Version const&);
bool operator>=(Version const&, Version const&);

} // namespace leia

namespace std {

#ifndef __cpp_lib_transformation_trait_aliases
template< class T >
using underlying_type_t = typename underlying_type<T>::type;
#endif

} // namespace std
