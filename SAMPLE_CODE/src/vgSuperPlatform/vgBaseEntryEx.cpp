


#include <vgStableHeaders.h>
#include "vgUIController.h"
#include "vgBaseEntryEx.h"



PropertiesParamVector vgBaseEntryEx::_params;

vgBaseEntryEx::vgBaseEntryEx( vgEntryType type ,vgKernel::Renderer* renderer ) :
vgBaseEntry( type , renderer )
{

}

vgBaseEntryEx::~vgBaseEntryEx(void)
{

}

void vgBaseEntryEx::OnPropertyChanged(string paramName)
{
	updateInternalFromUI( paramName );
}

void vgBaseEntryEx::AddNodeTabs()
{
	/**
		最开始需要刷新
	*/
	updateUIFromInternal();

	vgUIController::GetInstance()->RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = 
		vgUIController::GetInstance()->GetPropertiesViewBar();

	/**
		首先清空param队列
	*/
	_params.clear();

	/**
		开始添加属性 
	*/
	pushPropertiesParam( _params );


	/**
		插入到属性框中
	*/
	vgPropertyPage* propPage = vgUIController::GetInstance()->GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, &_params[0],  _params.size() );

#if VGK_LANG_CHS
	pageViewBar->AddTab("自动属性", propPage);
#else
	pageViewBar->AddTab("Auto Properties", propPage);
#endif
}
//----------------------------------------------------------------
void* vgBaseEntryEx::getConnectPtrByName( const String& param_name )
{
	PropertiesParamVector& params = getPropertiesParamVector();

	PropertiesParamVector::iterator iter = params.begin();
	PropertiesParamVector::iterator iter_end = params.end();

	for ( ; iter != iter_end ; ++ iter )
	{
		PropertiesParam& pa = *iter;
		if ( pa.label == param_name )
		{
			return pa.connectedPtr;
		}
	}

	return NULL;
}

