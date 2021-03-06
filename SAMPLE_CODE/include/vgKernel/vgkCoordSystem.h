


#ifndef __VGKCOORDSYSTEM_H__
#define __VGKCOORDSYSTEM_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkRendererObserver.h>
	
namespace vgKernel {

	/**
		@date 	2008/12/02  9:17	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT CoordSystem : 
		public vgKernel::Singleton<CoordSystem>,
		public vgKernel::ObserverContainer
	{
	private:
		friend class vgKernel::Singleton<CoordSystem>;
	private:
		CoordSystem()
			: vgKernel::Singleton<CoordSystem>( VGK_SINGLETON_LEFE_COORDSYS )
		{

		}

	public:

		virtual ~CoordSystem()
		{

		}

		void reset(){ _coord = Vec3();}

	protected:

		virtual bool initialise()
		{

			return true;
		}
		virtual bool shutdown()
		{
			return true;
		}


	public:

		vgKernel::Vec3 getProjectionCoord()
		{
			return _coord;
		}

		vgKernel::Vec3* getProjectionCoordRef()
		{
			return &_coord;
		}

		void setProjectionCoord( const vgKernel::Vec3& new_coord );

		vgKernel::Vec3 transCoordOpenGLToProj( 
			const vgKernel::Vec3& opengl_coord )
		{
			vgKernel::Vec3 ret;
			ret.x = opengl_coord.x + _coord.x;
			ret.y = opengl_coord.y + _coord.y;
			ret.z = opengl_coord.z + _coord.z;
			ret.z = -ret.z; // ȡ��

			return ret;
		}

		vgKernel::Vec3 transCoordProjToOpenGL( 
			const vgKernel::Vec3& gis_coord )
		{
			vgKernel::Vec3 ret;
			ret.x = gis_coord.x - _coord.x;
			ret.y = gis_coord.y - _coord.y;
			ret.z = -gis_coord.z; // ȡ��
			ret.z = ret.z - _coord.z;

			return ret;
		}


		void readProject(const String& projpath , const String& projname);

		void saveProject();

	private:

		vgKernel::Vec3 _coord;

	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKCOORDSYSTEM_H__