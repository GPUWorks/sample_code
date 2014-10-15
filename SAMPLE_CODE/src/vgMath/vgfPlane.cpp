/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#include <vgStableHeaders.h>
#include <vgMath/vgfPlane.h>
#include <vgMath/vgfVector3.h>
#include <vgMath/vgfAxisAlignedBox.h>

namespace vgMath {
	

	//-----------------------------------------------------------------------
	Plane::Plane ()
	{
		normal = Vector3::ZERO;
		d = 0.0;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Plane& rhs)
	{
		normal = rhs.normal;
		d = rhs.d;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3& rkNormal, Real fConstant)
	{
		normal = rkNormal;
		d = -fConstant;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3& rkNormal, const Vector3& rkPoint)
	{
		redefine(rkNormal, rkPoint);
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
		const Vector3& rkPoint2)
	{
		redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	Real Plane::getDistance (const Vector3& rkPoint) const
	{
		return normal.dotProduct(rkPoint) + d;
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide (const Vector3& rkPoint) const
	{
		Real fDistance = getDistance(rkPoint);

		if ( fDistance < 0.0 )
			return Plane::NEGATIVE_SIDE;

		if ( fDistance > 0.0 )
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}


	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide (const AxisAlignedBox& box) const
	{
		if (box.isNull()) 
			return NO_SIDE;
		if (box.isInfinite())
			return BOTH_SIDE;

		return getSide(box.getCenter(), box.getHalfSize());
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide (const Vector3& centre, const Vector3& halfSize) const
	{
		// Calculate the distance between box centre and the plane
		Real dist = getDistance(centre);

		// Calculate the maximise allows absolute distance for
		// the distance between box centre and plane
		Real maxAbsDist = normal.absDotProduct(halfSize);

		if (dist < -maxAbsDist)
			return Plane::NEGATIVE_SIDE;

		if (dist > +maxAbsDist)
			return Plane::POSITIVE_SIDE;

		return Plane::BOTH_SIDE;
	}
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
		const Vector3& rkPoint2)
	{
		Vector3 kEdge1 = rkPoint1 - rkPoint0;
		Vector3 kEdge2 = rkPoint2 - rkPoint0;
		normal = kEdge1.crossProduct(kEdge2);
		normal.normalise();
		d = -normal.dotProduct(rkPoint0);	///点乘,得到在法线上的投影,即为距离
	}
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3& rkNormal, const Vector3& rkPoint)
	{
		normal = rkNormal;
		d = -rkNormal.dotProduct(rkPoint);
	}
	//-----------------------------------------------------------------------
	Vector3 Plane::projectVector(const Vector3& p)
	{
		// We know plane normal is unit length, so use simple method
		Matrix3 xform;
		xform[0][0] = normal.x * normal.x - 1.0f;
		xform[0][1] = normal.x * normal.y;
		xform[0][2] = normal.x * normal.z;
		xform[1][0] = normal.y * normal.x;
		xform[1][1] = normal.y * normal.y - 1.0f;
		xform[1][2] = normal.y * normal.z;
		xform[2][0] = normal.z * normal.x;
		xform[2][1] = normal.z * normal.y;
		xform[2][2] = normal.z * normal.z - 1.0f;
		return xform * p;

	}
	//-----------------------------------------------------------------------
	Real Plane::normalise(void)
	{
		Real fLength = normal.length();

		// Will also work for zero-sized vectors, but will change nothing
		if (fLength > 1e-08f)
		{
			Real fInvLength = 1.0f / fLength;
			normal *= fInvLength;
			d *= fInvLength;
		}

		return fLength;
	}
	//-----------------------------------------------------------------------



	
	
}
