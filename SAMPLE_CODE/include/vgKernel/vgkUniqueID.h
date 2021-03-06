



#ifndef __VGKUNIQUEID_H__
#define __VGKUNIQUEID_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	struct UuidStruct {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];
	};

	/**
		@date 	2008/07/04  14:55	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT UniqueID
	{
	public:

		UniqueID()
			: _id1(0),
			_id2(0)
		{

		}

		UniqueID( UuidStruct* uuid );
		UniqueID( const UniqueID& unique_id );
		~UniqueID()
		{

		}

		bool operator == ( const UniqueID& value ) const
		{
			assert( value.isValid() );
			if ( _id1 == value._id1 && 
				_id2 == value._id2 )
			{
				return true;
			}

			return false;
		}

		bool operator <  ( const UniqueID& value ) const 
		{ 
			assert( value.isValid() );
			if ( _id1 < value._id1 )
			{
				return true;
			}
			else if ( _id1 != value._id1 )
			{
				return false;
			}

			// now, _id1 == value._id1
			return _id2 < value._id2;
		}

		bool operator > ( const UniqueID& value ) const 
		{ 
			assert( value.isValid() );
			if ( _id1 > value._id1 )
			{
				return true;
			}
			else if ( _id1 != value._id1 )
			{
				return false;
			}

			// now, _id1 == value._id1
			return _id2 > value._id2;
		}

		UniqueID& operator = ( const UniqueID& value )
		{
			assert( value.isValid() );
			_id1 = value._id1;
			_id2 = value._id2;
			return *this;
		}

		String getString();

		bool isValid() const
		{
			return !(_id1 == 0 && _id2 == 0);
		}
	public:

		unsigned long _id1;
		unsigned long _id2;
	};
	

	typedef std::vector<UniqueID> UniqueIDVector;

	/**
		专用于产生UniqueID
	*/
	class  VGK_EXPORT UniqueIDFactory
	{
	private:
		UniqueIDFactory()
		{
			
		}
	public:
		~UniqueIDFactory()
		{
			
		}

	public:
		static UniqueID* getUniqueIDPtr();

		static UniqueID getUniqueID();
	};

	
	
}// end of namespace vgKernel
	


#endif // end of __VGKUNIQUEID_H__