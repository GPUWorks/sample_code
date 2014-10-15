// vgnContourDrawer.h: interface for the CContourDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_)
#define AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*===================等值线绘制器===========================*/
//
//  其负责等值线的绘制以及绘制属性的管理

/*==============================================================*/

#include "vgNoise\vgnContourObj.h"

//typedef CTypedPtrArray<CPtrArray,CContourObj*> CContourObjArray;
//typedef CTypedPtrArray<CPtrArray,CContourObj*> CContourFillArray;
namespace vgNoise
{
class CContour;
class CContourDrawer  
{
public:
	CContourDrawer(CContour* pContour);
	virtual ~CContourDrawer();

	
	void ModifyColorTable();
	//---------管理绘制属性--------------------
	//  SetColorTable();.....
	//  SetDrawStyle();......

private:
	//-----绘制相关---------------



	//-----绘制对象-------------
	

private:
	//CContour* m_pContourOwner; //所要绘制的等值线
	//CContourObjArray m_ContourObjSets; //绘制对象集合
//	CColorLookUpTable m_ColorTable;
//CContourFillArray m_ContourFillSets;
	//------绘制属性--------------
	//   
	//   CDrawStyle......

	//------实现缩放和平移--------------
	//float m_ScaleRatio; //缩放比例
	//float m_TranslateX; //X方向平移量
	//float m_TranslateY; //Y方向平移量

};

#endif // !defined(AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_)
}