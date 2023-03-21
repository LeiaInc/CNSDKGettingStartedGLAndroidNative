/************************************************************************************

Filename    :   OVR_Win32_IncludeWindows.h
Content     :   Small helper header to include Windows.h properly
Created     :   Oct 16, 2014
Authors     :   Chris Taylor, Scott Bassett

Copyright   :   Copyright 2014 Oculus, Inc. All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.3 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.3

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*************************************************************************************/

#ifndef OVR_Win32_IncludeWindows_h
#define OVR_Win32_IncludeWindows_h

#include "OVR_Types.h"

// Automatically avoid including the Windows header on non-Windows platforms.
#ifdef OVR_OS_MS

// It is common practice to define WIN32_LEAN_AND_MEAN to reduce compile times.
// However this then requires us to define our own NTSTATUS data type and other
// irritations throughout our code-base.
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

// Prevents <Windows.h> from #including <Winsock.h>, as we use <Winsock2.h> instead.
#ifndef _WINSOCKAPI_
#define DID_DEFINE_WINSOCKAPI
#define _WINSOCKAPI_
#endif

// Prevents <Windows.h> from defining min() and max() macro symbols.
#ifndef NOMINMAX
#define NOMINMAX
#endif

OVR_DISABLE_ALL_MSVC_WARNINGS()
// d3dkmthk.h requires an NTSTATUS type, but WIN32_LEAN_AND_MEAN will prevent.
#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
//#include <ntstatus.h>
OVR_RESTORE_ALL_MSVC_WARNINGS()

#ifdef DID_DEFINE_WINSOCKAPI
#undef _WINSOCKAPI_
#undef DID_DEFINE_WINSOCKAPI
#endif

#endif // OVR_OS_MS

#endif // OVR_Win32_IncludeWindows_h
