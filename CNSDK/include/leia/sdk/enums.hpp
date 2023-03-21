#pragma once

#include <cstdint>

namespace leia {
namespace sdk {

enum class eLeiaInterlaceMode : int32_t
{
	ViewPeeling,    ///< Use view peeling (N views with varied perspective)
	ViewSliding,    ///< Use view-sliding (N views with constant perspective)
	StereoSliding,  ///< Use stereo-sliding (2 views with varied perspective)
	COUNT
};

enum class eLeiaShaderDebugMode
{
	None,                   ///< Debugging off
	ShowTextureCoordinates, ///< Show texture coordinates
	ShowAllViews,           ///< Show all views (tiled, horizontal, or vertical)
	ShowFirstView,          ///< Show single first view
	ShowCalibrationImage,   ///< Show calibration image
	ShowBeerGlass,			///< Show beer glass image
	ShowStereoImage,		///< Show stereo image
	ShowStereoVideo,		///< Video Views
	ShowThumbnails,			///< Show stereo thumbnails
	COUNT
};

enum class eLeiaTaskResponsibility
{
	SDK,    ///< The Leia Native SDK is responsible for the task
	User    ///< User is reponsible for the task
};

enum class eLeiaShaderType
{
	Vertex,
	Pixel,
	Geometry
};

enum class eLeiaViewTextureType
{
	Individual = 0,		///< Individual texture objects for each view
	Atlas      = 1,		///< A single texture object with all views
	Array      = 2		///< A texture array holding all views
};

enum class eFitMode
{
	Fill,
	FitCenter,
	CropFill,
	CropFitSquare,
	Count
};

enum class eBufferType
{
	Views,
	InterlacedResult
};

} // namespace sdk
} // namespace leia
