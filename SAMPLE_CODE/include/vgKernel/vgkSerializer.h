



#ifndef __VGKSERIALIZER_H__
#define __VGKSERIALIZER_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/05/12  9:31	
		@author  xy
	
		@brief 	提供统一的序列化和反序列化的接口
	
		@see    
	*/
	class Serializer
	{
	public:
		Serializer()
		{
			
		}
		virtual ~Serializer()
		{
			
		}

	public:
	
	
	};
	


	/**
		提供二进制的序列化
	*/
	class SerializerBinary : public Serializer
	{
	public:
		SerializerBinary()
		{
			
		}
		~SerializerBinary()
		{
			
		}
	
	
	protected:
	
		
	
	private:
	
	
	};


	/**
		提供XML的序列化
	*/
	//class SerializerXML
	//{
	//public:
	//	SerializerXML()
	//	{
	//		
	//	}
	//	~SerializerXML()
	//	{
	//		
	//	}
	//
	//
	//protected:
	//
	//	
	//
	//private:
	//
	//
	//};
	
}// end of namespace vgKernel
	


#endif // end of __VGKSERIALIZER_H__