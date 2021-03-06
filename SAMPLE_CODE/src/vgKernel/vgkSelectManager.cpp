
#include <vgStableHeaders.h>
#include <vgKernel/vgkSelectManager.h>
#include <vgKernel/vgkVec3.h>
#include <vgKernel/vgkRay.h>
#include <vgKernel/vgkMath.h>
#include <vgKernel/vgkCamMgrImpl.h>
#include <vgKernel/vgkCamMgrHolder.h>
#include <vgKernel/vgkRenderCommandManager.h>
#include <vgKernel/vgkColorableObject.h>
#include <vgKernel/vgkSelectObserver.h>
#include <gdal/ogr_geometry.h>


#define DEQUEMAX 32//chunyongma

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

namespace vgKernel {

// 	//////////////////////////////////////////////////////////////////////////
// 	void OperatorHandler::registerOperatorHandle()
// 	{
// 		SelectManager::getSingleton().m_operators.push_back( this );
// 	}
// 	//////////////////////////////////////////////////////////////////////////



	SelectManager::SelectManager() : Singleton<SelectManager>(VGK_SINGLETON_LEFE_SELECTMANAGER)
	{
		// 在选择模式下响应输入事件
		m_lButtonDown = false;
		// m_mergeBox.setColor(0,0,1);

		_enableRenderSelected = true;
		_enableRenderBoundingBox = true;

		m_start_x = 0;
		m_start_y = 0;
		m_drag_x = 0;
		m_drag_y = 0;
		m_end_x = 0;
		m_end_y = 0;

		m_window_width = 0;
		m_window_height = 0;
		
		statusFlag = VG_INPUTMODE_SELECT;
		//m_mergeBox.setColor(0, 1, 0);
		m_axis.Init();
		vgKernel::InputSystem::getSingleton().registerHandle(this);

		_culledRenderers = 
			RendererManager::getSingletonPtr()->getCulledRendererPackage();

		assert( _culledRenderers != NULL );

		_hasoperator = false;//chunyongma

		m_bIsMenuOperate = false;

		IniOperate();//chunyongma

		m_pSelectRenderCmd = new SelectRenderCmd;

		vgKernel::RenderCommandFacade::AddCommand(m_pSelectRenderCmd);

		// 向属性管理器注册，获取对屏幕尺寸的监控
		if (! vgKernel::PropertyManager::getSingleton().registerPropertyObserver("PROP_SCREEN_SIZE", this))
		{
			vgKernel::Property<vgKernel::Vec2> *screenSizeProp
				= new vgKernel::Property<vgKernel::Vec2>("PROP_SCREEN_SIZE");

			screenSizeProp->setValue(vgKernel::Vec2(1,1));
			vgKernel::PropertyManager::getSingleton().addProperty(screenSizeProp);
			vgKernel::PropertyManager::getSingleton().registerPropertyObserver("PROP_SCREEN_SIZE", this);	
		}

		m_pSelectMode = new SelectMode;
		setDefaultSelectMode();
		m_bSelectStop = false;
		m_bDrawSelRegion = true;

	}

	SelectManager::~SelectManager()
	{
		bool re;
		vgKernel::RenderCommandFacade::RemoveCommand(m_pSelectRenderCmd);
		re = vgKernel::PropertyManager::getSingleton().removePropertyObserver("PROP_SCREEN_SIZE", this);

		assert(re);

		if ( m_pSelectMode != NULL )
		{
			delete m_pSelectMode;
			m_pSelectMode = NULL;
		}
	}

	bool SelectManager::initialise()
	{
		return true;
	}

	bool SelectManager::shutdown()
	{
		return true;
	}

	void SelectManager::onEvent(unsigned int eventId, void *param)
	{
		if (eventId == UnTypedProperty::PROPERTY_ON_CHANGE)
		{
			vgKernel::Vec2 *pSize = (vgKernel::Vec2*)param;
			assert(pSize != NULL);

			SetViewSize(pSize->x, pSize->y);
		}
	}

	void SelectManager::IniOperate()//chunyongma
	{
		m_sumtrans = 0;

		m_sumrot = 0;

		m_sumscale = Vec3(1,1,1);

		m_opcenter = m_selectedCenter;
	}

	void SelectManager::SetViewSize(int cx, int cy)
	{
		m_window_width = cx;
		m_window_height = cy;
	}

	void SelectManager::renderSelected()
	{
		if ( _enableRenderSelected == false )
		{
			return;
		}

		RendererQueue::iterator iter = m_bufferItems.begin();
		RendererQueue::iterator end = m_bufferItems.end();

		while (iter != end)
		{	
			if ( _enableRenderBoundingBox )
			{
				(*iter)->getBoundingBox().render();
			}

			(*iter)->setSelected(true);
			iter ++;
		}
		
		if (m_bufferItems.size() != 0)
		{
			if ( _enableRenderBoundingBox )
			{
				m_mergeBox.render();
			}
            //当选中的是vcr节点时，不显示编辑轴 [9/10/2008 zhu]
			vgKernel::Vec3 boxCenter = m_mergeBox.getCenter();

			vgKernel::Vec3 camPos = CamMgrHolder::getSingleton().getCamManager()->getCurrentPosition();

			float distance = boxCenter.distance(camPos);
			
			// 自动放缩坐标轴
			//if (distance / 25 > 10)
			m_axis.SetSize(distance / 10/*25*/);

			m_axis.Render();

			// Modified by ZhouZY 2010-5-9 与YuXin乐山属性查询功能冲突
			//colorbleSelectedRender( m_bufferItems );  
		}

#if 1

		if (KEY_DOWN(VK_CONTROL))
		{
			return ;
		}

		glPushAttrib(GL_CURRENT_BIT);
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		float t_begin_x = 
			( (float) m_start_x / (float)m_window_width ) * 2.0f - 1.0f;
		float t_begin_y = 
			( 1.0f - (float) m_start_y / (float)m_window_height ) * 2.0f - 1.0f;

		float t_end_x = 
			( (float)(m_drag_x) / (float)m_window_width ) * 2.0f - 1.0f;
		float t_end_y = 
			( 1.0f - (float) (m_drag_y) / (float)m_window_height ) * 2.0f - 1.0f;
	
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_DEPTH_TEST);
		glLineWidth(2.0f);
		glColor3f(1.0f, 0.8f, 0.5f);
		//------------------------------------------
		// 画线
		//------------------------------------------
		if (m_lButtonDown)
		{
			glBegin(GL_LINE_LOOP);
			{
				glVertex2f( t_begin_x, t_begin_y );
				glVertex2f( t_begin_x, t_end_y );
				glVertex2f( t_end_x, t_end_y );
				glVertex2f( t_end_x , t_begin_y );
			}
			glEnd();
		}

		glMatrixMode( GL_PROJECTION );
		glPopMatrix();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
		glEnable( GL_TEXTURE_2D );

		glPopAttrib();
#endif
		renderSelectRegion();

	}
	
	void SelectManager::updateSelectedItem()
	{
		if (m_axis.NeedUpdate())
		{

			Vec3 tran = m_axis.GetCenter();
			Vec3 rot = m_axis.GetRot();
			Vec3 scale = m_axis.GetScal();

			Vec3 lastCenter = m_selectedCenter;
			Vec3 offset = tran - m_selectedCenter;

			RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();
			vgKernel::SelectManager* smanager = vgKernel::SelectManager::getSingletonPtr();

			if (scale.x || scale.y || scale.z)
			{
				Vec3 sCenter = m_mergeBox.getCenter(); //selectedBuffer[i]->getBoundingBox().getCenter();

				for (int i=0; i<selectedBuffer.size(); i++)
				{
					selectedBuffer[i]->scale(sCenter.x, sCenter.y, sCenter.z, 1-scale.x, 1-scale.y, 1-scale.z);
				}
				// 缩放操作不改变中点，无需计算
				vgKernel::SelectManager::getSingleton().mergeBox( getSelectedItems() );
				m_axis.ResetUpdate();
				
				// chuyongma

				m_sumscale.x *= (1 - scale.x );//chunyongma
				m_sumscale.y *= (1 - scale.y );
				m_sumscale.z *= (1 - scale.z );

				vgKernel::RendererManager::getSingleton().invalidate();
				return ;
			}

			if (offset.x || offset.y || offset.z)
			{
				////TRACE("translate \n");
				for (int i=0; i<selectedBuffer.size(); i++)
				{
					selectedBuffer[i]->translate(offset.x, offset.y, offset.z);
				}
				vgKernel::SelectManager::getSingleton().mergeBox( getSelectedItems() );
				// 移动过后重新计算中点
				m_selectedCenter = tran;//m_mergeBox.getCenter(); 
				m_axis.ResetUpdate();
				
				m_sumtrans += offset;//chunyongma

				vgKernel::RendererManager::getSingleton().invalidate();
				return ;
			}

			Vec3 center = m_selectedCenter;
			if (rot.x || rot .y|| rot.z)
			{
				for (int i=0; i<selectedBuffer.size(); i++)
				{
					////TRACE("处理旋转 \n");
					// 旋转绕着中点为原点的轴
					selectedBuffer[i]->rotate(rot.x * 90, center.x ,center.y, center.z, 1, 0, 0);
					selectedBuffer[i]->rotate(rot.y * 90, center.x ,center.y, center.z, 0, 1, 0);
					selectedBuffer[i]->rotate(rot.z * 90, center.x ,center.y, center.z, 0, 0, 1);
				}
				//　为避免旋转抖动，不重新计算中点
				vgKernel::SelectManager::getSingleton().mergeBox( getSelectedItems() );
				m_axis.ResetUpdate();
				
				m_sumrot += rot;//chunyongma

				vgKernel::RendererManager::getSingleton().invalidate();
				return ;
			}
		}
		else if (m_axis.GetCenter() != m_selectedCenter)
		{
			m_axis.SetCenter(m_selectedCenter);
		}

	}

	void SelectManager::OnMouseMove(UINT nFlags, CPoint position)
	{
		if ( !m_bIsMenuOperate ) //非菜单编辑方式即按Ctrl的快捷键方式
		{
			UINT flag = _hasoperator ? nFlags : 0;
			m_axis.OnMouseMove( flag, position );
		}
		else                     //菜单编辑方式
		{
			m_axis.OnMouseMove( getEditMode(), position, _hasoperator );
		}

		if (_hasoperator && !KEY_DOWN(VK_CONTROL) && !m_bIsMenuOperate )
		{
			PushOperatorDeque();
			_hasoperator = false;
			IniOperate();
		}

		// 矩形框选择模式
		if ( m_pSelectMode->selectRegionShape == SRS_Rect )
		{
			if (m_lButtonDown)
			{
				m_drag_x = position.x;
				m_drag_y = position.y;
			}
		}

		// 多边形选择模式   ZhouZY 2010-4-29 21:56
		else if ( m_pSelectMode->selectRegionShape == SRS_Polygon )
		{
			m_clickPt = Math::trans2DPointTo3DVec(position.x, position.y);
		}
	}

	void SelectManager::OnLButtonDown(UINT nFlags, CPoint position)
	{
		//TRACE("Down %d %d \n", position.x, position.y);
		if ( m_pSelectMode->selectRegionShape == SRS_Rect )
		{
			m_lButtonDown = true;

			m_start_x = position.x;
			m_start_y = position.y;

			m_drag_x = position.x;
			m_drag_y = position.y;
		}
		else if ( m_pSelectMode->selectRegionShape == SRS_Polygon )
		{
			m_bSelectStop = false;
		}

		//如果按下Ctrl不管是否为菜单操作，都按快捷键方式处理
		if ( KEY_DOWN(VK_CONTROL) && getEditMode() == vgKernel::Default )
		{
			m_axis.OnLButtonDown( nFlags, position );

			_hasoperator = true;
			m_lButtonDown = false;
			m_bIsMenuOperate = false;
			m_bSelectStop = true;

			IniOperate();
		}
		//如果没按Ctrl并且使用菜单操作执行此步
		if ( getEditMode() != vgKernel::Default )
		{
			m_axis.OnLButtonDown( nFlags, position );

			_hasoperator = true;
			m_lButtonDown = false;
			m_bIsMenuOperate = true;
			m_bSelectStop = true;

			IniOperate();
		}
	}

	void SelectManager::OnLButtonUp(UINT nFlags, CPoint position)
	{
		//TRACE("Up %d %d \n", position.x, position.y);
		m_axis.OnLButtonUp(nFlags, position);

		// VGK_SHOW("LButton \n");
		if (_hasoperator)
		{
			// VGK_SHOW("停止记录 \n");
			PushOperatorDeque();

			_hasoperator = false;

			IniOperate();
		}

		if ( KEY_DOWN(VK_CONTROL) || getEditMode() != vgKernel::Default )
		{
			m_lButtonDown = false;
			return ;
		}
 
		if ( m_pSelectMode->selectRegionShape == SRS_Rect )
		{
			m_end_x = position.x;
			m_end_y = position.y;

			if ((m_end_x != m_start_x || m_end_y != m_start_y) && m_lButtonDown)
			{
				SelectRectObject();
				merageSelectedItems();
				updateBox();
			} 

			m_lButtonDown = false;
		}
		else if ( m_pSelectMode->selectRegionShape == SRS_Polygon )
		{
			if ( !m_bSelectStop )
			{
				m_bDrawSelRegion = true;
				m_selectRegionPts.push_back( m_clickPt );
			}
		}
	}

	void SelectManager::OnLButtonDbClick(UINT nFlags, CPoint position)
	{
#if 0
		if ( m_pSelectMode->selectRegionShape == SRS_Rect )
		{
			// 以下功能为双击鼠标左键时选择物体
			Renderer* tmpMesh = NULL;
			Vec3 click_pos = Math::trans2DPointTo3DVec(position.x, position.y);
			Vec3 camera_pos = CamMgrHolder::getSingleton().getCamManager()->getCurrentPosition();
			Ray translatedRay;


			Vec3 click_dir = click_pos - camera_pos;
			click_dir.normalise();

			Ray click_ray(camera_pos , click_dir);
			std::pair<bool, float> result;


			RendererPackage::iterator ipac = _culledRenderers->begin();
			RendererPackage::iterator ipac_end = _culledRenderers->end();

			if (! (GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				m_bufferItems.clear();
			}

			float distance = 1e10f;

			for ( ; ipac != ipac_end; ++ ipac )
			{
				RendererQueue* que = &ipac->second;
				assert( que != NULL );

				RendererQueue::iterator iter  = que->begin();
				RendererQueue::iterator iter_end = que->end();

				for ( ; iter != iter_end ; ++ iter )
				{
					Renderer* cr = *iter;

					if ( cr->isSelectable() == false || !cr->getVisible() )
					{
						continue;
					}

					result = vgKernel::Math::intersects( click_ray, cr->getBoundingBox() );

					if (result.first && result.second < distance)
					{
						std::pair<bool, float> testres = 
							cr->testIntersectWithTriangles( click_ray );

						if ( testres.first == true && testres.second < distance )
						{

							if (find(m_bufferItems.begin(), m_bufferItems.end(), cr) != m_bufferItems.end())
							{
								//VGK_SHOW("Fixed \n");
								continue;
							}

							if (!m_bufferItems.empty() && tmpMesh != NULL)
							{
								m_bufferItems.pop_back();
							}

							m_bufferItems.push_back( cr );
							tmpMesh = cr;
							distance = testres.second;

							TRACE("Select %s Distance : %f \n", cr->getName().c_str(), distance);
						}
						else if (testres.first)
						{
							TRACE("%s Distance : %f \n", cr->getName().c_str(), distance);
						}
						else
						{
							TRACE("No trangle %s Distance : %f \n", cr->getName().c_str(), distance);
						}
					}
					else
					{
						TRACE("Only box %s Distance : %f \n", cr->getName().c_str(), distance);

					}

				}

			}// end ipac


			if (tmpMesh != NULL && tmpMesh->getName().length() > 0)
			{
				tmpMesh->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);
			}
			else
			{
				vgKernel::SelectManager::getSingleton().clearSelection();		
				return ;
			}

			updateBox();
		}
		
#endif
		if ( m_pSelectMode->selectRegionShape == SRS_Polygon )
		{
			SelectPolygonObject( &m_selectRegionPts );
			merageSelectedItems();
			updateBox();
			m_bSelectStop = true;
			m_selectRegionPts.clear();
			m_bDrawSelRegion = false;
		}
		else
		{
			SelectDbClickObject( position );
			updateBox();
		}
	}

	//-------------------------------------------------------------------------
	void SelectManager::SelectDbClickObject( const CPoint& position )
	{
		Renderer* tmpMesh = NULL;
		Vec3 click_pos = Math::trans2DPointTo3DVec(position.x, position.y);
		Vec3 camera_pos = CamMgrHolder::getSingleton().getCamManager()->getCurrentPosition();
		Ray translatedRay;


		Vec3 click_dir = click_pos - camera_pos;
		click_dir.normalise();

		Ray click_ray(camera_pos , click_dir);
		std::pair<bool, float> result;


		RendererPackage::iterator ipac = _culledRenderers->begin();
		RendererPackage::iterator ipac_end = _culledRenderers->end();

		if (! (GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			/*uncolorbleSelectedRender( m_bufferItems );
			m_bufferItems.clear();*/
			clearSelection();
		}

		float distance = 1e10f;

		for ( ; ipac != ipac_end; ++ ipac )
		{
			RendererQueue* que = &ipac->second;
			assert( que != NULL );

			RendererQueue::iterator iter  = que->begin();
			RendererQueue::iterator iter_end = que->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				Renderer* cr = *iter;

				if ( cr->isSelectable() == false || !cr->getVisible() )
				{
					continue;
				}

				result = vgKernel::Math::intersects( click_ray, cr->getBoundingBox() );

				if (result.first && result.second < distance)
				{
					std::pair<bool, float> testres = 
						cr->testIntersectWithTriangles( click_ray );

					if ( testres.first == true && testres.second < distance )
					{
						if (find(m_bufferItems.begin(), m_bufferItems.end(), cr) != m_bufferItems.end())
						{
							continue;
						}

						if (!m_bufferItems.empty() && tmpMesh != NULL)
						{
							m_bufferItems.pop_back();
						}

						m_bufferItems.push_back( cr );
						tmpMesh = cr;
						distance = testres.second;

						TRACE("Select %s Distance : %f \n", cr->getName().c_str(), distance);
					}
					else if (testres.first)
					{
						TRACE("%s Distance : %f \n", cr->getName().c_str(), distance);
					}
					else
					{
						TRACE("No trangle %s Distance : %f \n", cr->getName().c_str(), distance);
					}
				}
				else
				{
					TRACE("Only box %s Distance : %f \n", cr->getName().c_str(), distance);
				}

			}

		}// end ipac


		if (tmpMesh != NULL && tmpMesh->getName().length() > 0)
		{
			tmpMesh->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);
		}
		else
		{
			vgKernel::SelectManager::getSingleton().clearSelection();		
			return ;
		}
	}

	//-------------------------------------------------------------------------
	void SelectManager::SelectRectObject()
	{
		GLdouble proj_mat[16];
		GLdouble model_mat[16];

		glGetDoublev( GL_PROJECTION_MATRIX , proj_mat);
		glGetDoublev( GL_MODELVIEW_MATRIX , model_mat);

		GLdouble winx;
		GLdouble winy;
		GLdouble winz;

		int ViewPort[4];

		//float maxx, maxy, minx, miny;
		CPoint maxPoint;
		CPoint minPoint;

		glGetIntegerv (GL_VIEWPORT,ViewPort);

		if (m_start_x < m_end_x)
		{
			minPoint.x = (long)m_start_x;
			maxPoint.x = (long)m_end_x;
		} 
		else
		{
			minPoint.x = (long)m_end_x;
			maxPoint.x = (long)m_start_x;
		}

		if (ViewPort[3] - m_end_y < ViewPort[3] - m_start_y)
		{
			minPoint.y = ViewPort[3] - m_end_y;
			maxPoint.y = ViewPort[3] - m_start_y;
		}
		else
		{
			minPoint.y = ViewPort[3] - m_start_y;
			maxPoint.y = ViewPort[3] - m_end_y;
		}

		m_currentSelectedItems.clear(); // 首先清空数组

		RendererPackage::iterator ipac = _culledRenderers->begin();
		RendererPackage::iterator ipac_end = _culledRenderers->end();


		for ( ; ipac != ipac_end; ++ ipac )
		{
			RendererQueue* que = &ipac->second;
			assert( que != NULL );

			RendererQueue::iterator iter  = que->begin();
			RendererQueue::iterator iter_end = que->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				Renderer* mi = *iter;

				if ( mi->isSelectable() == false || !mi->getVisible() )
				{
					continue;
				}

				vgKernel::Box boundBox = mi->getBoundingBox();

				bool bResult = false;
				for (int i=0; i<8; i++)
				{
					vgKernel::Vec3 corner = boundBox.getCorner(i);

					gluProject( corner.x , corner.y , corner.z ,
						model_mat , proj_mat , ViewPort , &winx , &winy , &winz );

					if (maxPoint.x < winx || maxPoint.y < winy || minPoint.x > winx || minPoint.y > winy)
					{
						bResult = false;

						// 如果为全包围模式，只要有一点在外部就判为不选择
						if ( m_pSelectMode->isInclude )
						{
							break;
						}
						continue;
					}
					else if (winz < 1.0)
					{
						bResult = true;

						// 如果为半包围模式，只要有一点在内部就判为选择
						if ( !m_pSelectMode->isInclude )
						{
							break;
						}
					}

				} // end for i

				if ( bResult )
				{
					RendererQueue::iterator itr = 
						find(m_currentSelectedItems.begin(), m_currentSelectedItems.end(), mi); 

					if ( itr ==  m_currentSelectedItems.end())
					{
						m_currentSelectedItems.push_back( mi );
					}
				}

			} // end for iter

		} // end for ipac

#if 0
		if (!m_bufferItems.empty() && m_bufferItems[0] != NULL)
		{
			m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);

			for (int i=1; i<m_bufferItems.size(); i++)
			{
				m_bufferItems[i]->notifyOberversOnEvent(VG_OBS_ADDSELECTION, NULL);
			}

			// 框选时通知组 点选时在AddNodeTabs中自动实现
			if (m_bufferItems.size() > 1)
				m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_GROUPUPDATE, NULL);
		}
		
		mergeBox( getSelectedItems() );
		m_selectedCenter = m_mergeBox.getCenter();
		m_axis.SetCenter(m_selectedCenter);
#endif

}

	//-------------------------------------------------------------------------
	void SelectManager::mergeBox( const RendererQueue& renderQuene )
	{
		vgKernel::Renderer *pMeshItem;

		if ( renderQuene.empty() == true )
		{
			return;
		}

		RendererQueue::const_iterator iter = renderQuene.begin();
		RendererQueue::const_iterator end = renderQuene.end();

		pMeshItem = *iter;
		ASSERT(pMeshItem);

		vgKernel::Vec3 minVec = pMeshItem->getBoundingBox().getMinimum();
		vgKernel::Vec3 maxVec = pMeshItem->getBoundingBox().getMaximum();

		while (iter != end)
		{
			pMeshItem = *iter;
			ASSERT(pMeshItem);

			if (minVec.x > pMeshItem->getBoundingBox().getMinimum().x)
			{
				minVec.x = pMeshItem->getBoundingBox().getMinimum().x;
			}
			if (minVec.y > pMeshItem->getBoundingBox().getMinimum().y)
			{
				minVec.y = pMeshItem->getBoundingBox().getMinimum().y;
			}
			if (minVec.z > pMeshItem->getBoundingBox().getMinimum().z)
			{
				minVec.z = pMeshItem->getBoundingBox().getMinimum().z;
			}

			if (maxVec.x < pMeshItem->getBoundingBox().getMaximum().x)
			{
				maxVec.x = pMeshItem->getBoundingBox().getMaximum().x;
			}				
			if (maxVec.y < pMeshItem->getBoundingBox().getMaximum().y)
			{
				maxVec.y = pMeshItem->getBoundingBox().getMaximum().y;
			}				
			if (maxVec.z < pMeshItem->getBoundingBox().getMaximum().z)
			{
				maxVec.z = pMeshItem->getBoundingBox().getMaximum().z;
			}

			iter ++;
		}

		m_mergeBox.setMaximum(maxVec);
		m_mergeBox.setMinimum(minVec);

	}

	//-------------------------------------------------------------------------
	void SelectManager::addSelection( vgKernel::Renderer* renderer, bool reMerge)
	{
		//assert( renderer != NULL );
		if (renderer == NULL)
		{
			return ;
		}

		for (int i=0; i<m_bufferItems.size(); i++)
		{
			if (m_bufferItems[i]->getName() == renderer->getName())
			{
				return ;
			}
		}

		m_bufferItems.push_back(renderer);
		
		if (reMerge)
		{
			mergeBox( getSelectedItems() );
			m_selectedCenter = m_mergeBox.getCenter();
			m_axis.SetCenter(m_selectedCenter);
		}

		fireOnSelectChanged();
	}

	/*void SelectManager::clearSelection()
	{
		m_bufferItems.clear();

		fireOnSelectChanged();
	}*/

	void SelectManager::fireOnSelectChanged()
	{
		for (int i=0; i<m_observerVec.size(); i++)
		{
			m_observerVec[i]->onEvent(0, NULL);
		}
	}

	////-------------------------------------------------------------------------//chunyongma2008.9.8
	//void SelectManager::DeleteRenderItem( vgKernel::Renderer *prender )
	//{
	//	vector<vgKernel::OperatorHandler *>::iterator iter = m_operators.begin();

	//	vector<vgKernel::OperatorHandler *>::iterator end = m_operators.end();

	//	while ( iter != end )
	//	{
	//		if ((*iter)->DeleteRenderer( prender ))
	//		{
	//			break;
	//		}
	//		iter++;
	//	}
	//}

	//-------------------------------------------------------------------------
	bool SelectManager::deleteSelectedRenderers(  const bool& alert /*= true*/ )
	{
		if ( m_bufferItems.empty() == true )
		{
			if ( alert )
			{
				MessageBox( NULL, "请选择物体！", " VG-DELETE", MB_OK );
			}
			
			return false;
		}

		if ( alert )
		{
			if(MessageBox(NULL, "不可恢复，是否删除?","Delete",MB_YESNO) != IDYES)
			{
				return false;
			}
		}

		vgKernel::RendererManager& mgr = 
			vgKernel::RendererManager::getSingleton();

		RendererQueue copyItems( m_bufferItems.begin() , m_bufferItems.end() );

		/** modified by ZhouZY 2010-5-7 
			clearSelection()内执行uncolorbleSelectedRenderer()，因此需要在删除之前
			进行清空操作
		*/
		clearSelection();  

		RendererQueue::iterator iter = copyItems.begin();

		while ( iter != copyItems.end() )
		{	
			vgKernel::Renderer* prenderer = *iter;
			assert( prenderer != NULL );

			//------------------------------------------
			// 删除选择缓冲,并且界面通知.
			//------------------------------------------
			copyItems.erase( iter );

			String outString("删除 ");
			outString += prenderer->getName();
			outString += "\n";

			VGK_SHOW(outString.c_str());

			//------------------------------------------
			// 直接通过renderermanger来删除.
			//------------------------------------------
			if( mgr.deleteRenderer( prenderer ) == false )
			{
				MessageBox( NULL , "出现错误, 操作终止执行. " , "Error" , MB_OK );
				//clearSelection();
				return false;
			}

			iter = copyItems.begin();
		}


		//clearSelection();

		vgKernel::RendererManager::getSingleton().invalidate();

		return true;
	}

	//-------------------------------------------------------------------------
	void SelectManager::updateBox()
	{
		mergeBox( getSelectedItems() );
		m_selectedCenter = m_mergeBox.getCenter();
		m_axis.SetCenter(m_selectedCenter);
	}

	void SelectManager::translateGroup(const float& x, const float& y, const float& z)
	{
		RendererQueue::iterator iter = m_bufferItems.begin();
		RendererQueue::iterator end  = m_bufferItems.end();

		while (iter != end)
		{
			(*iter)->translate(x, y, z);
			iter ++;
		}
		
		// undo redo ==============================================
		setsumtrans(Vec3(x, y, z));

		updateBox();
	}

	// undo redo 
	// 外部接口
	void SelectManager::setsumtrans( Vec3 trans)
	{
		m_sumtrans = trans;

		PushOperatorDeque();
	}

	//void SelectManager::setsumrot( Vec3 rots )
	//{
	//	m_sumrot = rots;

	//	PushOperatorDeque();
	//}

	//void SelectManager::setsumscale( Vec3 scale )
	//{
	//	m_sumscale = scale;

	//	PushOperatorDeque();
	//}


	void SelectManager::PushOperatorDeque()
	{
		// 无物体选中直接返回避免报错	add by lss when 2008-11-4 9:56:44
		RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();
		if(0 == selectedBuffer.size())
			return;

		char buffer[128];

		if (m_sumtrans.x || m_sumtrans.y || m_sumtrans.z)
		{
			vgkEditTrans *pp = new vgkEditTrans( m_sumtrans );

			RendererOperatorObserverQueue *qq = new RendererOperatorObserverQueue;

			//RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();

			//assert( selectedBuffer.size() != 0 );

			for (int i=0; i<selectedBuffer.size(); i++)
			{
				OperationObserver *roo = new OperationObserver( selectedBuffer[i] );

				qq->push_back( roo );
			}
			vgkOperator *curopertor = new vgkOperator( qq, pp );

			if (undodeque.size() >= DEQUEMAX )//15保证最大返回15次操作
			{
				undodeque.pop_front();
			}

			undodeque.push_back( curopertor );

			sprintf_s(buffer, 128, "移动操作入栈 %d \n", undodeque.size());
			//return;

		}

		if ((m_sumscale.x != 1) || (m_sumscale.y != 1) || (m_sumscale.z != 1))
		{
			//Vec3 sCenter = m_mergeBox.getCenter(); //selectedBuffer[i]->getBoundingBox().getCenter();

			vgkEditScale *pp = new vgkEditScale( m_opcenter, (1.0 - m_sumscale));

			RendererOperatorObserverQueue *qq = new RendererOperatorObserverQueue;

			//RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();

			//assert( selectedBuffer.size() != 0 );

			for (int i=0; i<selectedBuffer.size(); i++)
			{
				OperationObserver *roo = new OperationObserver( selectedBuffer[i] );

				qq->push_back( roo );
			}
			vgkOperator *curopertor = new vgkOperator( qq, pp );

			if (undodeque.size() >= DEQUEMAX )//15保证最大返回15次操作
			{
				undodeque.pop_front();
			}

			undodeque.push_back( curopertor );

			sprintf_s(buffer, 128, "缩放操作入栈 %d \n", undodeque.size());

			//m_posbeforeoperate = tran;

			//return;
		}

		if (m_sumrot.x || m_sumrot .y|| m_sumrot.z)
		{
			//Vec3 sCenter = m_mergeBox.getCenter(); //selectedBuffer[i]->getBoundingBox().getCenter();

			vgkEditRotate *pp = new vgkEditRotate( m_opcenter, m_sumrot );

			RendererOperatorObserverQueue *qq = new RendererOperatorObserverQueue;

			//RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();

			//assert( selectedBuffer.size() != 0 );

			for (int i=0; i<selectedBuffer.size(); i++)
			{
				OperationObserver *roo = new OperationObserver( selectedBuffer[i] );

				qq->push_back( roo );
			}

			vgkOperator *curopertor = new vgkOperator( qq, pp );

			if (undodeque.size() >= DEQUEMAX )//15保证最大返回15次操作
			{
				undodeque.pop_front();
			}

			undodeque.push_back( curopertor );

			sprintf_s(buffer, 128, "旋转操作入栈 %d \n", undodeque.size());

			//return ;
		}	

		m_sumtrans = 0;

		m_sumrot = 0;

		m_sumscale = Vec3(1,1,1);
		// VGK_SHOW(buffer);
	}

	void SelectManager::Undo()
	{
//		char buffer[128];

		int aa = undodeque.size();

		if (aa > 0)
		{
			vgKernel::vgkOperator *qq = 
				undodeque[aa-1];

			qq->unexecute();

			if (redodeque.size() >= DEQUEMAX )
			{
				redodeque.pop_front();
			}
			redodeque.push_back( qq );

			// sprintf_s(buffer, 128, "操作出栈 %d \n", undodeque.size());
			// VGK_SHOW(buffer);

			undodeque.pop_back();

			vgKernel::SelectManager::getSingleton().updateBox();

			vgKernel::RendererManager::getSingleton().invalidate();

		}
	}

	void SelectManager::Redo()
	{
		int aa = redodeque.size();

		if (aa > 0)
		{
			vgKernel::vgkOperator *qq = 
				redodeque[aa-1]; 

			qq->execute();
            ///add by kinghj,2009.11.07
			///修改后重做之后能够通过撤销恢复原样
			if (undodeque.size() >= DEQUEMAX )
			{
				undodeque.pop_front();
			}
			undodeque.push_back( qq );

			redodeque.pop_back();

			vgKernel::SelectManager::getSingleton().updateBox();

			vgKernel::RendererManager::getSingleton().invalidate();
		}

	}

	void SelectManager::OnKeyDown( int keyCode )
	{
		int tempSize = 0;
		if(keyCode == vgKernel::InputCode::PERIOD)
		{
			m_axis.ChangeSizeScale(0.1f); 
		}

		if(keyCode == vgKernel::InputCode::COMMA)
		{
			m_axis.ChangeSizeScale(-0.1f);
		}

	}

	
	Renderer* SelectManager::getSelectedRendererByType( RendererType type )
	{
		RendererQueue sel = getSelectedItems();

		if (sel.size()>0)
		{	
			Renderer* tmp=sel[0];

			if(tmp->getType()==type)
			{
				return tmp;
			}
			else
			{
				String str = "类型不匹配，请重新选择！";
				SystemUtility::showModelDialog( str );
			}
		}
		else
		{
			String str = "未选中任何节点，请选择！";
			SystemUtility::showModelDialog( str );
		}

		return NULL;
	}

	void SelectManager::updateUI()
	{
		if (!m_bufferItems.empty() && m_bufferItems[0] != NULL)
		{
			m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);

			for (int i=1; i<m_bufferItems.size(); i++)
			{
				m_bufferItems[i]->notifyOberversOnEvent(VG_OBS_ADDSELECTION, NULL);
			}

			// 框选时通知组 点选时在AddNodeTabs中自动实现
			if (m_bufferItems.size() > 1)
				m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_GROUPUPDATE, NULL);
		}
	}

	void SelectManager::colorbleSelectedRender( const RendererQueue& renderQueue )
	{
		RendererQueue::const_iterator iter = renderQueue.begin();
		RendererQueue::const_iterator iter_end = renderQueue.end();

		for ( ; iter != iter_end; iter++ )
		{
			if ( (*iter)->getType() == 5000 || (*iter)->getType() == 5010 )
			{
				vgKernel::ColorableObject* colorObj = 
					dynamic_cast<vgKernel::ColorableObject*>(*iter);
				vgKernel::RgbaQuad color( 255, 60, 60, 128 );
				colorObj->setColorableValue( color );
			}
		}
	}
	void SelectManager::uncolorbleSelectedRender( const RendererQueue& renderQueue )
	{
		RendererQueue::const_iterator iter = renderQueue.begin();
		RendererQueue::const_iterator iter_end = renderQueue.end();

		for ( ; iter != iter_end; iter++ )
		{
			if ( (*iter)->getType() == 5000 || (*iter)->getType() == 5010 )
			{
				vgKernel::ColorableObject* colorObj = 
					dynamic_cast<vgKernel::ColorableObject*>(*iter);
				vgKernel::RgbaQuad color( 0, 0, 0, 0 );
				colorObj->setColorableValue( color );
			}
		}
	}


	void SelectManager::SelectPolygonObject( const Vec3Vector* pointsVec )
	{
		assert( pointsVec != NULL );
		if( pointsVec == NULL )
		{
			return;
		}	 

		m_currentSelectedItems.clear();  //首先清空数组

		RendererPackage::iterator ipac = _culledRenderers->begin();
		RendererPackage::iterator ipac_end = _culledRenderers->end();
		
		// 1. 将选区构造为第一个OGRPoly
		OGRGeometry* pGeoLine1 = OGRGeometryFactory::createGeometry( wkbLinearRing );
		OGRLinearRing* pLinearRing1 = dynamic_cast<OGRLinearRing*>( pGeoLine1 );

		for ( int i = 0; i < pointsVec->size(); i++ )
		{
			pLinearRing1->addPoint( (pointsVec->at(i)).x, (pointsVec->at(i)).z );
		}
		pLinearRing1->addPoint( (pointsVec->at(0)).x, (pointsVec->at(0)).z );


		OGRGeometry* pGeoPoly1 = OGRGeometryFactory::createGeometry( wkbPolygon );
		OGRPolygon* pPolygon1 = dynamic_cast<OGRPolygon*>( pGeoPoly1 );

		pPolygon1->addRing( pLinearRing1 );


		// 2.循环Renderer用包围盒相交测试
		for ( ; ipac != ipac_end; ++ ipac )
		{
			RendererQueue* que = &ipac->second;
			assert( que != NULL );

			RendererQueue::iterator iter  = que->begin();
			RendererQueue::iterator iter_end = que->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				Renderer* mi = *iter;

				if ( mi->isSelectable() == false || !mi->getVisible() )
				{
					continue;
				}

				Vec3 minPos = mi->getBoundingBox().getMinimum();
				Vec3 maxPos = mi->getBoundingBox().getMaximum();

				// 3. 构造第二个用于相交测试的OGRPloy
				OGRGeometry* pGeoLine2 = OGRGeometryFactory::createGeometry( wkbLinearRing );
				OGRLinearRing* pLinearRing2 = dynamic_cast<OGRLinearRing*>( pGeoLine2 );

				pLinearRing2->addPoint( minPos.x, minPos.z );
				pLinearRing2->addPoint( maxPos.x, minPos.z );
				pLinearRing2->addPoint( maxPos.x, maxPos.z );
				pLinearRing2->addPoint( minPos.x, maxPos.z );
				pLinearRing2->addPoint( minPos.x, minPos.z );

				OGRGeometry* pGeoPoly2 = OGRGeometryFactory::createGeometry( wkbPolygon );
				OGRPolygon* pPolygon2 = dynamic_cast<OGRPolygon*>( pGeoPoly2 );

				pPolygon2->addRing( pLinearRing2 );
				
				// 4. OGRPoly包含测试
				OGRBoolean bContains = false;
				if ( m_pSelectMode->isInclude ) // 全包含
				{
					bContains = pPolygon1->Contains( pPolygon2 );
				}
				else     // 半包含
				{
					bContains = pPolygon1->Intersects( pPolygon2 );
				}
				
				if ( bContains )
				{
					RendererQueue::iterator iter = 
						find( m_currentSelectedItems.begin(), m_currentSelectedItems.end(), mi );

					if (  iter ==  m_currentSelectedItems.end() )
					{
						m_currentSelectedItems.push_back( mi );
					}

				} // end if bIntersect

				OGRGeometryFactory::destroyGeometry( pGeoLine2 );
				OGRGeometryFactory::destroyGeometry( pPolygon2 );
				pGeoLine2 = NULL;
				pGeoPoly2 = NULL;

			} // end for iter

		} // end for ipac

		OGRGeometryFactory::destroyGeometry( pGeoLine1 );
		OGRGeometryFactory::destroyGeometry( pGeoPoly1 );
		pGeoLine1 = NULL;
		pGeoPoly1 = NULL;


#if 0 // modified by zhou

		if (!m_bufferItems.empty() && m_bufferItems[0] != NULL)
		{
			m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);

			for (int i=1; i<m_bufferItems.size(); i++)
			{
				m_bufferItems[i]->notifyOberversOnEvent(VG_OBS_ADDSELECTION, NULL);
			}

			// 框选时通知组 点选时在AddNodeTabs中自动实现
			if (m_bufferItems.size() > 1)
				m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_GROUPUPDATE, NULL);
		}

		mergeBox( getSelectedItems() );
		m_selectedCenter = m_mergeBox.getCenter();
		m_axis.SetCenter(m_selectedCenter);
#endif
	}


	//----------------------------------------------------------------
	void SelectManager::selectAllObject()
	{
		RendererPackage::iterator ipac = _culledRenderers->begin();
		RendererPackage::iterator ipac_end = _culledRenderers->end();

		m_bufferItems.clear();

		for ( ; ipac != ipac_end; ++ ipac )
		{
			RendererQueue* que = &ipac->second;
			assert( que != NULL );

			RendererQueue::iterator iter  = que->begin();
			RendererQueue::iterator iter_end = que->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				if ( !(*iter)->isSelectable() || !(*iter)->getVisible() )
				{
					continue;
				}

				m_bufferItems.push_back( *iter );

			} // end for iter

		} // end for ipac

		if (!m_bufferItems.empty() && m_bufferItems[0] != NULL)
		{
			m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);

			for (int i=1; i<m_bufferItems.size(); i++)
			{
				m_bufferItems[i]->notifyOberversOnEvent(VG_OBS_ADDSELECTION, NULL);
			}

			// 框选时通知组 点选时在AddNodeTabs中自动实现
			if (m_bufferItems.size() > 1)
				m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_GROUPUPDATE, NULL);
		}

		updateBox();
	}

	//----------------------------------------------------------------
	void SelectManager::setDefaultSelectMode()
	{
		assert( m_pSelectMode != NULL );
		if ( m_pSelectMode == NULL )
		{
			return;
		}

		m_pSelectMode->selectRegionShape = SRS_Rect;
		m_pSelectMode->selectRegionMode = SRM_New;
		m_pSelectMode->isInclude = false;
	}

	//----------------------------------------------------------------
	void SelectManager::renderPolygon()
	{
		if ( !m_bPolygonMode )
		{
			return;
		}

		drawLine();
	}


	//----------------------------------------------------------------
	void SelectManager::drawLine()
	{
		if ( m_polygonVec.size() < 1 )
		{
			return;
		}

		glPushMatrix();

			glDisable( GL_TEXTURE_2D );
			glDisable( GL_DEPTH_TEST );
			glDisable( GL_BLEND );

			// 指定线属性
			glLineWidth( 2.0f );
			glColor3f( 1.0f, 0.0f, 0.0f );

			glBegin( GL_LINE_LOOP );
				for ( int i = 0; i < m_polygonVec.size(); i++ )
				{
					glVertex3fv( m_polygonVec[i].v );
				}
				glVertex3fv( m_polygonPt.v );
			glEnd();

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_TEXTURE_2D );

		glPopMatrix();
	}



	//----------------------------------------------------------------
	void SelectManager::renderSelectRegion()
	{
		if ( m_selectRegionPts.size() < 1 || !m_bDrawSelRegion )
		{
			return;
		}

		glPushMatrix();
			glDisable( GL_TEXTURE_2D );
			glDisable( GL_DEPTH_TEST );
			glDisable( GL_BLEND );

			// 指定线属性
			glLineWidth( 2.0f );
			glColor3f( 1.0f, 0.0f, 0.0f );

			glBegin( GL_LINE_LOOP );
				for ( int i = 0; i < m_selectRegionPts.size(); i++ )
				{
					glVertex3fv( m_selectRegionPts[i].v );
				}
				glVertex3fv( m_clickPt.v );
			glEnd();

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_TEXTURE_2D );
		glPopMatrix();
	}
	//----------------------------------------------------------------
	void SelectManager::merageSelectedItems()
	{
		// Modified by ZhouZY 2010-5-9 与YuXin乐山属性查询功能冲突
		//uncolorbleSelectedRender( m_bufferItems );
		m_bufferItems.clear();
		RendererQueue::iterator last_itr = m_lastSelectedItems.begin();
		RendererQueue::iterator last_itr_end = m_lastSelectedItems.end();
		RendererQueue::iterator cur_itr = m_currentSelectedItems.begin();
		RendererQueue::iterator cur_itr_end = m_currentSelectedItems.end();

		switch( m_pSelectMode->selectRegionMode )
		{
		case SRM_New: // 最终选集=当前选集
			{
				m_bufferItems.insert( m_bufferItems.begin(), cur_itr, cur_itr_end );
			}
			break;
		case SRM_Add: // 最终选集=上一步选集+当前选集 ( 不包括last和current的重合 )
			{
				m_bufferItems.insert( m_bufferItems.begin(), last_itr, last_itr_end );

				// 去除重合部分
				for ( ; cur_itr != cur_itr_end; cur_itr++ )
				{
					RendererQueue::iterator iter = 
						find( m_bufferItems.begin(), m_bufferItems.end(), *cur_itr );

					if ( iter == m_bufferItems.end() )
					{
						m_bufferItems.push_back( *cur_itr );
					}
				}
			}
			break;
		case SRM_Subtract: // 最终选集=上一步选集-当前选集 ( 不包括last和current的重合 )
			{
				for ( ; last_itr != last_itr_end; last_itr++ )
				{
					RendererQueue::iterator iter = find( cur_itr, cur_itr_end, *last_itr );
					if ( iter == cur_itr_end )
					{
						m_bufferItems.push_back( *last_itr );
					}
				}
			}
			break;
		case SRM_Intersection:  //最终选集=上一步选集∩当前选集
			{
				for ( ; cur_itr != cur_itr_end; cur_itr++ )
				{
					RendererQueue::iterator iter = find( last_itr, last_itr_end, *cur_itr );
					if ( iter != last_itr_end )
					{
						m_bufferItems.push_back( *cur_itr );
					}
				}
			}
			break;
		case SRM_Reverse:  // 反向选择
			{
				RendererPackage::iterator ipac = _culledRenderers->begin();
				RendererPackage::iterator ipac_end = _culledRenderers->end();

				for ( ; ipac != ipac_end; ++ ipac )
				{
					RendererQueue* que = &ipac->second;
					assert( que != NULL );

					RendererQueue::iterator itr  = que->begin();
					RendererQueue::iterator itr_end = que->end();

					for ( ; itr != itr_end ; ++ itr )
					{
						if ( !(*itr)->isSelectable() || !(*itr)->getVisible() )
						{
							continue;
						}
						
						RendererQueue::iterator iter = find( cur_itr, cur_itr_end, *itr );
						if ( iter == cur_itr_end )
						{
							m_bufferItems.push_back( *itr );
						}

					} // end for itr 

				} // end for ipac
			}
			break;
		default:
			break;
		}

		m_currentSelectedItems.clear();
		m_lastSelectedItems.clear();

		m_lastSelectedItems.insert( m_lastSelectedItems.begin(), 
			m_bufferItems.begin(), m_bufferItems.end() );

		if (!m_bufferItems.empty() && m_bufferItems[0] != NULL)
		{
			m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_SELECTCHAGNED, NULL);

			for (int i=1; i<m_bufferItems.size(); i++)
			{
				m_bufferItems[i]->notifyOberversOnEvent(VG_OBS_ADDSELECTION, NULL);
			}

			// 框选时通知组 点选时在AddNodeTabs中自动实现
			if (m_bufferItems.size() > 1)
				m_bufferItems[0]->notifyOberversOnEvent(VG_OBS_GROUPUPDATE, NULL);
		}
	}
	//----------------------------------------------------------------
	void SelectManager::clearSelection()
	{
		// Modified by ZhouZY 2010-5-9 与YuXin乐山属性查询功能冲突
		//uncolorbleSelectedRender( m_bufferItems );
		m_lastSelectedItems.clear();
		m_currentSelectedItems.clear();
		m_bufferItems.clear();

		fireOnSelectChanged();
	}

}// end of namespace vgKernel
