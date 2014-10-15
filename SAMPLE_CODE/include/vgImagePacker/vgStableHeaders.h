
#ifndef __VGSTABLEHEADER_H__
#define __VGSTABLEHEADER_H__


#define _CRT_SECURE_NO_WARNINGS

//#pragma   warning(disable:4503)
//#pragma   warning(disable:4786)
#pragma   warning(disable:4018)
#pragma   warning(disable:4267)

//------------------------------------------
// MFC headers
//------------------------------------------

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//-----------------------------------------
// 新的kernel库
//------------------------------------------
#include <vgKernel/vgkForward.h>
#pragma message("  now input " VGK_LIB_NAME )
#pragma  comment(lib,VGK_LIB_NAME)

#include <vgImage/vgiDefinition.h>
#pragma message("  now input " VGI_LIB_NAME )
#pragma  comment(lib,VGI_LIB_NAME)

// #include <vgTerritory/vgtDefinition.h>
// #pragma message("  now input " VGT_LIB_NAME )
// #pragma  comment(lib,VGT_LIB_NAME)


//------------------------------------------
// user headers
//------------------------------------------

#include <vgKernel/vgkForward.h>

#include <vgKernel/vgkBox.h>
//#include <vgKernel/vgkCamera.h>
#include <vgKernel/vgkException.h>
#include <vgKernel/vgkCrcUtility.h>
#include <vgKernel/vgkLogManager.h>
#include <vgKernel/vgkMat4.h>
#include <vgKernel/vgkOpenGLSupport.h>
#include <vgKernel/vgkQuat.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkStringUtility.h>
#include <vgKernel/vgkStringVector.h>
#include <vgKernel/vgkSystemUtility.h>
#include <vgKernel/vgkTrace.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgKernel/vgkVec3.h>
//#include <vgKernel/vgkViewFrustum.h>
#include <vgKernel/vgkWarning.h>


//------------------------------------------
// Import libs
//------------------------------------------
#pragma message("  now input OpenGL standard lib...")
#pragma   comment(lib,"opengl32.lib")
#pragma	  comment(lib,"glu32.lib")
#pragma   comment(lib,"glaux.lib")
#pragma   comment(lib ,"glew32s.lib")
#pragma   comment(lib ,"glew32.lib")

//#if USE_CXIMAGE_LIB
//#pragma message("  now input cximage.lib...")
//#pragma comment(lib,"cximage.lib")
//#pragma comment(lib,"jbig.lib")
//#pragma comment(lib,"LibJPEG.lib")
//#pragma comment(lib,"LibPNG.lib")
//#pragma comment(lib,"libTiff.lib")
//#pragma comment(lib,"zlib.lib")
//#pragma comment(lib,"libjasper.lib")
//#pragma comment(lib,"j2k.lib")
//#endif // end of USE_CXIMAGE_LIB

//#if USE_IMAGE_SAVING
//
//#pragma message("  now input ImageLibrary.lib...")
//#pragma comment(lib,"ImageLibrary.lib")
//#pragma comment(lib,"squish.lib")

//#endif // end of USE_IMAGE_SAVING

//#if DIRECTINPUT_SUPPORT
//#pragma message("  now input dinput8.lib...")
//	#pragma comment(lib,"dinput8.lib")
//
//#pragma message("  now input dxguid.lib...")
//	#pragma  comment(lib, "dxguid.lib")
//#endif // end of DIRECTINPUT_SUPPORT


//#if CG_PROGRAM_SUPPORT
//#pragma message("  now input cg.lib...")
//#pragma  comment(lib, "cg.lib")
//#pragma message("  now input cgGL.lib...")
//#pragma  comment(lib, "cgGL.lib")
//#endif // end of CG_PROGRAM_SUPPORT



//------------------------------------------
// Standard Headers
//------------------------------------------
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <hash_map>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>



//#if DIRECTINPUT_SUPPORT
///// 初始化DirectX相关文件
//#include <dinput.h>
//#endif // end of DIRECTINPUT_SUPPORT


//#if CG_PROGRAM_SUPPORT
///// 初始化Cg相关文件
//#include <Cg/cg.h>
//#include <Cg/cgGL.h>
//#endif // end of CG_PROGRAM_SUPPORT





#endif // end of __VGSTABLEHEADER_H__