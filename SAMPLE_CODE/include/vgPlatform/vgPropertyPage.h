#pragma once

#include "vgBasedNode.h"
#include "vgBasedTabPage.h"
#include "Resource.h"
#include <string>

using std::string;



typedef struct PropertiesParam_t
{
	unsigned int	typeId;				// 属性项的类型
	string			label;				// 属性项名称
	string			comment;			// 属性项注释
	unsigned int	dataType;			// 属性数据类型
	void*			connectedPtr;		// 原数据指针
	CBCGPProp*		pProp;				// 生成的属性对象指针
} PropertiesParam;

/**
* vgPropertyPage 实现属性页的自动生成
* 可根据不同的PropertiesParam产生相应
* 的属性页.
*
* @see		   vgTestNode1
* @see		   vgCameraNode
*
* @author      ZSC
* @version     VRGIS FrameWork RC
* @since       VRGIS 1.0+
*/

// 自动化Property Page不是BasedTabPage的派生类
class vgPropertyPage : public CDialog
{
	DECLARE_DYNAMIC(vgPropertyPage)

public:
	vgPropertyPage(CWnd* pParent = NULL); 
	virtual ~vgPropertyPage();

	enum { IDD = IDD_PROPERTY };
 
	string GetTitle() { return "属性"; }

	CWnd* GetWnd() { return this; }

	// 根据Node传入的PropertiesParam生成属性页
	// @see vgTestNode1::AddNodeTabs
	void ConnectNode(vgBasedNode* node, PropertiesParam* propParam, int numOfProp);
	
	void AdjustLayout();
	
	// 属性更改回调函数，根据原数据指针更新数据
	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CBCGPPropList		m_wndPropList;

	vgBasedNode*		m_pNode;
	PropertiesParam*	m_pPropParam;
	int					m_numOfParam;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
