// FrameWorkDoc.cpp : implementation of the CFrameWorkDoc class
//

#include <vgStableHeaders.h>
#include "FrameWork.h"

#include "FrameWorkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameWorkDoc

IMPLEMENT_DYNCREATE(CFrameWorkDoc, CDocument)

BEGIN_MESSAGE_MAP(CFrameWorkDoc, CDocument)
	//{{AFX_MSG_MAP(CFrameWorkDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameWorkDoc construction/destruction

CFrameWorkDoc::CFrameWorkDoc()
{
	// TODO: add one-time construction code here

}

CFrameWorkDoc::~CFrameWorkDoc()
{
}

BOOL CFrameWorkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFrameWorkDoc serialization

void CFrameWorkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWorkDoc diagnostics

#ifdef _DEBUG
void CFrameWorkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFrameWorkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrameWorkDoc commands
