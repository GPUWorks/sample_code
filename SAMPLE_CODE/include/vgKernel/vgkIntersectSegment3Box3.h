


#ifndef __VGKINTERSECTSEGMENT3BOX3_H__
#define __VGKINTERSECTSEGMENT3BOX3_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkIntersector.h>
#include <vgKernel/vgkBox3.h>
#include <vgKernel/vgkSegment3.h>

	
namespace vgKernel {

	/**
		@date 	2008/11/13  20:19	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT IntrSegment3Box3
		: public Intersector
	{
	public:
		IntrSegment3Box3 (const Segment3& rkSegment,
			const Box3& rkBox/*, bool bSolid*/);

		// Object access.
		const Segment3& GetSegment () const;
		const Box3& GetBox () const;

		// Static test-intersection query.
		virtual bool Test ();

		//// Static find-intersection query.  The point(s) of intersection are
		//// accessed by GetQuantity() and GetPoint(i).
		//virtual bool Find ();

		//// Dynamic test-intersection query.
		//virtual bool Test (float fTMax, const Vec3& rkVelocity0,
		//	const Vec3& rkVelocity1);

		// Dynamic find-intersection query.  The first point of contact is
		// accessed by GetPoint(0), when there is a single contact, or by
		// GetPoint(0) and GetPoint(1), when the contact is a segment, in which
		// case the fetched points are the segment endpoints.  The first time of
		// contact is accessed by GetContactTime().
		//virtual bool Find (float fTMax, const Vec3& rkVelocity0,
		//	const Vec3& rkVelocity1);

		int GetQuantity () const;
		const Vec3& GetPoint (int i) const;

	private:
		using Intersector::IT_EMPTY;
		using Intersector::IT_POINT;
		using Intersector::IT_SEGMENT;
		using Intersector::m_fContactTime;
		using Intersector::m_iIntersectionType;

		// The objects to intersect.
		const Segment3* m_pkSegment;
		const Box3* m_pkBox;
		//bool m_bSolid;

		// Information about the intersection set.
		int m_iQuantity;
		Vec3 m_akPoint[2];
	};

	
	
}// end of namespace vgKernel
	


#endif // end of __VGKINTERSECTSEGMENT3BOX3_H__