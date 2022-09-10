#include "pch.h"
#include "CoordSystem.h"


void CoordSystem::SetNormalCoordSystem(CDC* pDC, CRect& rect)
{
	// �Զ�������ϵ
	// ����ӳ��ģʽ�����ã���ͼ����ʾ���豸��Ļ����ϵ�ϣ�
	// MM_ANISOTROPIC:���ڷ�Χ���豸��Χ��������ı䣬Ҫ��ʹ��SetWindowExt�������ô��ڵķ�Χ��ʹ��SetViewPortExt�������������ķ�Χ
	pDC->SetMapMode(MM_ANISOTROPIC);
	// ���ô��ڷ�Χ����.SetWindowExtEx(cx, cy); cx��cy���߼���λ��m��cm��Ӣ���.
	pDC->SetWindowExt(rect.Width(), rect.Height());
	// ����������Χ������SetViewPortExt(cx, cy). cx��cy���豸��λ��Ĭ����Ļ����X����Ϊ����Y����Ϊ���������Ϊ����Ϊ������������˼ά��
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	// ��������ԭ�㺯��.����ԭ��Ϊ����������
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	rect.OffsetRect(-rect.Width() / 2, -rect.Height());
}

CPoint CoordSystem::TransformToNormalCoordSystem(const CRect& rect, CPoint pt)
{
	CPoint newPt;
	// ƽ��
	newPt.x = pt.x - rect.Width() / 2;
	newPt.y = rect.Height() / 2 - pt.y;
	
	return newPt;
}

