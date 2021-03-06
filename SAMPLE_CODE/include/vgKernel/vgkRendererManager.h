


#ifndef __VGKRENDERERMANAGER_H__
#define __VGKRENDERERMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRenderCommand.h>



namespace vgKernel {



	class AbstractTree;


	/**
	@date 	2008/09/05  13:41	
	@author  xy

	@brief 	

	@see    
	*/
	class  VGK_EXPORT RendererManager : public vgKernel::Singleton<RendererManager>
	{
		friend class vgKernel::Singleton<RendererManager>;
	private:
		RendererManager();
	public:
		virtual~RendererManager();

	protected:

		virtual bool initialise();

		virtual bool shutdown();
		
		vgKernel::RenderCommand *m_pRenderBegin;
		vgKernel::RenderCommand *m_pRenderEnd;

	public:

		void reset();


		void initAfterOpenGLSetup();

		void clear();

		void invalidate();

		//void switchTreeType(vgTree::TREE_TYPE treeType)
		//{
		//	_treeType = treeType;
		//	initialise();
		//	invalidate();
		//}


		void renderBegin();
		void render();
		void renderEnd();


		void readProject(const String& projpath , const String& projname);

		void saveProject();

		/**
			添加成功的话,返回的是一个总的队列 
			如果_renderersAll中没有这个render type时, 会自动添加一个RendererQueue
		*/
		RendererQueue* addRenderer( Renderer* renderer );

		/**
			删除成功的话,返回删除过后的相应类型的总队列
		*/

		bool deleteRenderer( Renderer* renderer );

		/**
			删除所有的renderer, 用于resetScene
		*/
		bool deleteAllRenderers();

		void setAllRenderersVisiable(bool visiable);

		RendererQueue* getRendererQueueByType(
			const RendererType& renderertype ,
			const bool& add_if_needed );

		/**
			得到所有物体的一个队列
		*/
		RendererPackage* getAllRendererPackage()
		{
			return &_renderersAll;
		}

		RendererPackage* getCulledRendererPackage()
		{
			return &_renderersCulled;
		}

		RendererQueue* getCulledRenderQueueByType( 
			const RendererType& rendertype );

		Renderer* getRendererbyName(const String& name,
			bool bCaseSensitive = true);



		/**
			所有Renderer整体平移
		*/
		void	translateAllRenderers( const Vec3& vec);

		vgKernel::Box	getAllRendererBox();

		String getDetails();

	private:

		void caluateVisiblePackage();

	public:
		/**
			如果含有相应的类型,则返回相对应的所有物体的队列
			如果没有相应的类型,返回添加过后的物体的队列.
		*/
		RendererQueue* getRenderQueue(
			const RendererType& type ,
			const bool& add_if_needed );
		
		Box getAllRendererBoxWithoutshp();

	private:

		// 用于裁剪的tree
		AbstractTree* _sceneTree;


		RendererPackage _renderersAll;

		RendererPackage _renderersCulled;

	};


}// end of namespace vgKernel



#endif // end of __VGKRENDERERMANAGER_H__