


#ifndef __VGKSHADOWCASTOBJECT_H__
#define __VGKSHADOWCASTOBJECT_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkVec3.h>

	
namespace vgKernel {

	/**
		@date 	2009/03/30  19:12	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class ShadowCastObject
	{
	public:
		ShadowCastObject()
			: _shadowLightDirection( 0.1f, -0.5f, 0.1f ),
			_enableShadowCast( false )
		{

		}

		virtual ~ShadowCastObject()
		{
			
		}
	
	
	public:


		// �����4������Ҫ�̳���д

		// ��Ⱦshadow��caster
		virtual void renderInShadowMode() = 0;

		virtual void turnOffShadowCast()
		{
			// �̳д������Ҫ��һ��ʼ��д���������
			_enableShadowCast = false;
		}

		virtual void turnOnShadowCast( const vgKernel::Vec3& light_dir,
			bool force_to_load )
		{
			_shadowLightDirection = light_dir;
			_enableShadowCast = true;
		}

		// ��Ӱ�ķ����ڵ�һ�ε�ʱ���������
		// ������ķ�������ʱ��Ҳʹ�ô˺�������֪ͨ��
		virtual void onLightDirectionChanged( const vgKernel::Vec3& light_dir )
		{
			// �̳д�����Ҳ��Ҫд��һ��
			_shadowLightDirection = light_dir;
		}

		// get set ����

		vgKernel::Vec3 getShadowLightDirection() const
		{
			return _shadowLightDirection;
		}

		bool getShadowCastEnable() const
		{
			return _enableShadowCast;
		}

	private:

		bool _enableShadowCast;
		vgKernel::Vec3 _shadowLightDirection;
	};
	
	
	typedef std::vector<ShadowCastObject*> ShadowCastObjectVec;


}// end of namespace vgKernel
	


#endif // end of __VGKSHADOWCASTOBJECT_H__