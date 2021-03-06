


#ifndef __VGGGLOBALCONSOLE_H__
#define __VGGGLOBALCONSOLE_H__


#include <vgGlobal/vgglDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkTrace.h>

#include <vgKernel/vgkRenderCommand.h>
	
namespace vgGlobal {

	/**
		@date 	2008/08/05  9:34	
		@author  xy
	
		@brief 	全局控制.用于本系统和外部实现的接口.
		和GlobalFacade的区别在于,这里是必须实现的东西.
		而GlobalFacade是功能集合.
	
		@see    
	*/
	class  VGGL_EXPORT GlobalConsole : public vgKernel::Singleton<GlobalConsole>
	{
		friend class vgKernel::Singleton<GlobalConsole>;
	private:
		GlobalConsole();
		virtual ~GlobalConsole();

	protected:

		virtual bool initialise();

		virtual bool shutdown();

		vgKernel::RenderCommand *m_pRenderPrimitivesCmd;
		vgKernel::RenderCommand *m_pRenderEnd;

	public:

		virtual void reset(){};

		bool initAfterOpenGLSetup();

		void renderBegin();

		void renderPrimitives();

		void renderEnd();
		
		bool uninitBeforeOpenGLDestroy();
	
		void showFps();
		/**
			设置当前的project路径和名称. 
		*/
		bool readProject(  const String& file_path  );

		bool readProjectEnd(  const String& file_path  );

		bool saveProject(   const String& file_path );

	private:

		void setDefault();

		//ZhouZY add 2009-11-17 19:56
		//用于检测是否需要向UI中加入Billboard 节点
		void checkHasBBNodeAddToUI();

	private:
	
		bool  _hasInited;
		bool  _hasUninited;
	
		bool _firstOpenScene;
	};
	
	class ConsoleRenderPrimitives : public vgKernel::RenderCommand
	{
	public:
		ConsoleRenderPrimitives() : vgKernel::RenderCommand(VG_RP_CONSOLE_RENDER_PRIMITIVES) {}

		inline virtual bool render()
		{
			GlobalConsole::getSingleton().renderPrimitives();

			return true;
		}
	};

	class ConsoleRenderEnd : public vgKernel::RenderCommand
	{
	public:
		ConsoleRenderEnd() : vgKernel::RenderCommand(VG_RP_CONSOLE_RENDER_END) {}

		inline virtual bool render()
		{
			GlobalConsole::getSingleton().renderEnd();

			return true;
		}
	};
	
}// end of namespace vgGlobal
	


#endif // end of __VGGGLOBALCONSOLE_H__