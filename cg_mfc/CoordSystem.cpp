#include "pch.h"
#include "CoordSystem.h"


void CoordSystem::SetNormalCoordSystem(CDC* pDC, CRect& rect)
{
	// 自定义坐标系
	// 设置映射模式（作用：把图形显示到设备屏幕坐标系上）
	// MM_ANISOTROPIC:窗口范围和设备范围可以任意改变，要求使用SetWindowExt函数设置窗口的范围，使用SetViewPortExt函数设置视区的范围
	pDC->SetMapMode(MM_ANISOTROPIC);
	// 设置窗口范围函数.SetWindowExtEx(cx, cy); cx和cy是逻辑单位，m、cm、英寸等.
	pDC->SetWindowExt(rect.Width(), rect.Height());
	// 设置视区范围函数。SetViewPortExt(cx, cy). cx和cy是设备单位。默认屏幕坐标X向右为正，Y向下为正，这里改为向上为正，符合正常思维。
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	// 设置视区原点函数.设置原点为视区的中心
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	rect.OffsetRect(-rect.Width() / 2, -rect.Height());
}

CPoint CoordSystem::TransformToNormalCoordSystem(const CRect& rect, CPoint pt)
{
	CPoint newPt;
	// 平移
	newPt.x = pt.x - rect.Width() / 2;
	newPt.y = rect.Height() / 2 - pt.y;
	
	return newPt;
}

