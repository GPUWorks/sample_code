


#ifndef __VGKDYNAMICLIBRARYUTILITY_H__
#define __VGKDYNAMICLIBRARYUTILITY_H__

#include <WinDef.h>
#include <cassert>


	
namespace vgKernel {

	/**
		@date 	2009/05/13  14:53	
		@author  xy
	
		@brief 	
		注意!下面所有的宏都用在cpp文件中。
		1.VGK_DLL_RES_DECLARE用于声明，放在全局里。
		2.VGK_DLL_RES_SWITCH根据当前的dll进行切换，放在dll的cpp里
		3.VGK_DLL_RES_SWITCH_NAME用于主程序(exe)调用dll的dialog时,
		需要事先声明dll的资源而用。
	
		@see    
	*/
	class DynamicLibraryResourceSwitch 
	{ 
	public: 
		DynamicLibraryResourceSwitch( HINSTANCE hCurInstance ) 
		{ 
			assert( hCurInstance != INVALID_HANDLE_VALUE );
			_prevInstance = AfxGetResourceHandle(); 
			AfxSetResourceHandle(hCurInstance); 
		}; 

		~DynamicLibraryResourceSwitch()
		{ 
			AfxSetResourceHandle(_prevInstance); 
		}

	private:

		HINSTANCE _prevInstance; 
	};


	
	VGK_EXPORT bool checkDLLisLoad( const char* dllname );

	VGK_EXPORT bool loadDLL( const char* dllname );

	VGK_EXPORT bool unloadDLL( const char* dllname );

}// end of namespace vgKernel



#define VGK_DLL_RES_DECLARE()	\
	extern AFX_EXTENSION_MODULE* currentDynamicLibModule

#define VGK_DLL_RES_SWITCH() \
	assert( currentDynamicLibModule != NULL );\
	vgKernel::DynamicLibraryResourceSwitch __dynResSwitch( \
	currentDynamicLibModule->hModule )\

#define VGK_DLL_RES_SWITCH_NAME( lib_name ) \
	HINSTANCE __instance = (HINSTANCE)GetModuleHandle( lib_name );\
	assert( __instance != INVALID_HANDLE_VALUE );\
	vgKernel::DynamicLibraryResourceSwitch __dynResSwitch( __instance )\




#define VGK_DLL_FUNC_HELPER( ... ) \
		( __VA_ARGS__ );\
		}\
	}


#define VGK_DLL_RUN_CLASSFUNC( dll_name , classname , class_fun ) \
	{\
	vgKernel::Plugin* mod_ = vgKernel::PluginManager::getSingleton().getPluginRef( dll_name );\
		if ( mod_ != NULL )\
		{\
		pfn##classname##_##class_fun  pfn_;\
			pfn_ = ( pfn##classname##_##class_fun )(mod_->getProcAddr(\
			#classname##"_"###class_fun ));\
			(*pfn_)VGK_DLL_FUNC_HELPER






#define VGK_DLL_RUN_FUNC( dll_name , func ) \
	{\
		HMODULE mod_ = GetModuleHandle( dll_name );\
		SetLastError( NO_ERROR );\
		if ( mod_ != NULL )\
		{\
		pfn##func  pfn_;\
			pfn_ = ( pfn##func )GetProcAddress( mod_ ,\
			#func );\
			(*pfn_)VGK_DLL_FUNC_HELPER




#define VGK_DLL_CHECK_LOAD( dllname ) \
	vgKernel::checkDLLisLoad( dllname )
	

#define VGK_DLL_LOAD( dllname ) \
	vgKernel::loadDLL( dllname )

#define VGK_DLL_UNLOAD( dllname ) \
	vgKernel::unloadDLL( dllname )



#define VGK_DLL_DECLARE_CLASSFUNC( plug , classname , class_fun , func ) \
		pfn##classname##_##class_fun  func;\
		func = ( pfn##classname##_##class_fun )plug->getProcAddr(\
			#classname##"_"###class_fun );



#define VGK_DLL_DECLARE_FUNC( plug , normal_func , func ) \
	pfn##normal_func  func;\
	func = ( pfn##normal_func )plug->getProcAddr(\
		#normal_func );


#endif // end of __VGKDYNAMICLIBRARYUTILITY_H__