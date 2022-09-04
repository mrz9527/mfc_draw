#include "pch.h"
#include "C2DrawTask.h"


void C2DrawTask::TaskPoint_Array(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	srand(time(0));
	for (decltype(leftTop.x) x = leftTop.x; x <= rightBottom.x; ++x) {
		for (decltype(leftTop.y) y = leftTop.y; y >= rightBottom.y; --y) {
			COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
			pDC->SetPixelV(x, y, color);
		}
	}
	
	IncreaseId();
}

void C2DrawTask::TaskLine_Triangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	CPoint p0, p1, p2;
	p0.x = (leftTop.x + rightBottom.x) / 2; // ��
	p0.y = leftTop.y - (leftTop.y - rightBottom.y) / 4;

	p1.x = leftTop.x; // ����
	p1.y = rightBottom.y;

	p2 = rightBottom; // ����
	
	// ����gdi��graphic device interface ͼ���豸�ӿڣ����󣺻���
	CPen redPen, greenPen, bluePen;
	redPen.CreatePen(0, 2, RGB(255, 0, 0));
	greenPen.CreatePen(0, 2, RGB(0, 255, 0));
	bluePen.CreatePen(0, 2, RGB(0, 0, 255));

	// ѡ�񻭱ʣ����ؾɵĻ��ʣ�����������
	auto oldPen = pDC->SelectObject(redPen);
	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	pDC->SelectObject(greenPen);
	pDC->LineTo(p2);

	pDC->SelectObject(bluePen);
	pDC->LineTo(p0);

	// ��ԭ����Ϊ�ɻ���
	pDC->SelectObject(oldPen);

	IncreaseId();
}

void C2DrawTask::TaskRectangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	// ����gdi���󣺻�ˢ������
	// ��ˢ��Ҫ���������ͣ�ʵ����ˢSolidBrush����Ӱ��ˢHatchBrish��ͼ����ˢPatternBrush
	CBrush brush;
	brush.CreateSolidBrush(RGB(55, 0, 55));
	CPen redPen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));

	// ѡ��ˢ������ɻ�ˢ
	auto oldBrush = pDC->SelectObject(brush);
	auto oldPen = pDC->SelectObject(redPen);

	// ���ƾ���
	pDC->Rectangle(CRect(leftTop, rightBottom));

	// �ͷŻ�ˢ
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	brush.DeleteObject();

	IncreaseId();
}

void C2DrawTask::TaskEllipse(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	CRect rect(leftTop, rightBottom);
	rect.DeflateRect(0, -20);

	// gdi������Բ����ˢ����������ˢ
	// �Ȼ���Բ���ٻ����Σ����ε��������Ḳ����Բ��Ϊ��ʹ��Բ��ʾ����������Ϊ���ε������ѡ��͸����ˢNULL_BRUSH��
	CBrush ellipseBrush;
	ellipseBrush.CreateSolidBrush(RGB(255, 255, 0));
	auto oldBrush = pDC->SelectObject(ellipseBrush);
	pDC->Ellipse(rect);
	
	pDC->SelectStockObject(NULL_BRUSH); // ѡ��ϵͳ���еİ�ɫ��ˢ
	pDC->Rectangle(rect);
	
	pDC->SelectObject(oldBrush);

	IncreaseId();
}

void C2DrawTask::TaskArc(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	CRect rect(leftTop, rightBottom);
	rect.DeflateRect(0, -20);
	CPoint startPt(leftTop), endPt(leftTop.x, rightBottom.y);

	// ������ʼ���ߺ���ֹ����
	pDC->MoveTo(rect.CenterPoint());
	pDC->LineTo(startPt);

	pDC->MoveTo(rect.CenterPoint());
	pDC->LineTo(endPt);
	
	// ����gdi������������
	CPen redPen, bluePen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	bluePen.CreatePen(0, 3, RGB(0, 0, 255));


	// ѡ���ɫ���ʣ���ʱ�������Բ������ɫ���ʣ�˳ʱ�������Բ��
	auto oldPen = pDC->SelectObject(redPen);
	pDC->Arc(rect, startPt, endPt);

	pDC->SelectObject(bluePen);
	auto oldArcDirection = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(rect, startPt, endPt);

	// �ָ�Ĭ��pDC״̬
	pDC->SelectObject(oldPen);
	pDC->SetArcDirection(oldArcDirection);

	IncreaseId();
}

void C2DrawTask::TaskPie(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	CRect rect(leftTop, rightBottom);
	rect.DeflateRect(0, -20);
	CPoint startPt(leftTop), endPt(leftTop.x, rightBottom.y);


	// ����gdi�����������ʣ�����������ˢ
	CPen redPen, bluePen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	bluePen.CreatePen(0, 3, RGB(0, 0, 255));
	CBrush redBrush, blueBrush;
	redBrush.CreateSolidBrush(RGB(255, 0, 0));
	blueBrush.CreateSolidBrush(RGB(0, 0, 255));

	// ѡ���ɫ���ʣ���ʱ��������Σ���ɫ���ʣ�˳ʱ���������
	auto oldPen = pDC->SelectObject(redPen);
	auto oldBrush = pDC->SelectObject(redBrush);
	pDC->Pie(rect, startPt, endPt);

	pDC->SelectObject(bluePen);
	pDC->SelectObject(blueBrush);
	auto oldArcDirection = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Pie(rect, startPt, endPt);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SetArcDirection(oldArcDirection);
	IncreaseId();
}

void C2DrawTask::TaskPie_BaGuaGraph(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
		return;

	CRect bigRect(leftTop, rightBottom);
	CPoint bigCenter(bigRect.CenterPoint());
	CPoint startPt(bigCenter.x, leftTop.y), endPt(bigCenter.x, rightBottom.y);

	// ����gdi����:����������ˢ��������ɫ����
	CBrush whiteBrush, blackBrush;
	whiteBrush.CreateSolidBrush(RGB(255, 255, 255));
	blackBrush.CreateSolidBrush(RGB(0, 0, 0));
	CPen whitePen;
	whitePen.CreatePen(0, 1, RGB(255, 255, 255));

	// �����������Բ��������Բ������
	auto oldBrush = pDC->SelectObject(whiteBrush);
	pDC->Pie(bigRect, startPt, endPt);
	pDC->SelectObject(blackBrush);
	auto oldArcDirection = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Pie(bigRect, startPt, endPt);

	// ���Ƶڶ��ݶȵ�Բ������������Բ����Բ��ɫ����Բ��ɫ��
	long midR = (leftTop.y - rightBottom.y) / 4;
	CPoint midCenterUpper(bigCenter.x, bigCenter.y + midR);
	CPoint midCenterDown(bigCenter.x, bigCenter.y - midR);
	CRect midRectUpper(midCenterUpper.x - midR, midCenterUpper.y + midR, midCenterUpper.x + midR, midCenterUpper.y - midR);
	CRect midRectDown(midCenterDown.x - midR, midCenterDown.y + midR, midCenterDown.x + midR, midCenterDown.y - midR);
	
	// ���Ƶڶ��ݶȵ���Բ
	pDC->SelectObject(whiteBrush);
	auto oldPen = pDC->SelectObject(whitePen); // Ϊ��ȥ����ɫ���Բ�ĺ�ɫ�߽�
	pDC->Ellipse(midRectUpper);

	// ���Ƶڶ��ݶȵ���Բ
	pDC->SelectObject(oldPen);
	pDC->SelectObject(blackBrush);
	pDC->Ellipse(midRectDown);

	// ���Ƶ����ݶȵ�Բ������������Բ����Բ��ɫ����Բ��ɫ��
	long smallR = midR / 3;
	CPoint smallCenterUpper(midCenterUpper), smallCenterDown(midCenterDown);
	CRect smallRectUpper(smallCenterUpper.x - smallR, smallCenterUpper.y + smallR, smallCenterUpper.x + smallR, smallCenterUpper.y - smallR);
	CRect smallRectDown(smallCenterDown.x - smallR, smallCenterDown.y + smallR, smallCenterDown.x + smallR, smallCenterDown.y - smallR);

	// ���Ƶ����ݶȵ���Բ����ɫ��
	pDC->SelectObject(blackBrush);
	//auto oldPen = pDC->SelectObject(whitePen); // Ϊ��ȥ����ɫ���Բ�ĺ�ɫ�߽�
	pDC->Ellipse(smallRectUpper);

	// ���Ƶڶ��ݶȵ���Բ
	pDC->SelectObject(whiteBrush);
	pDC->Ellipse(smallRectDown);


	pDC->SelectObject(oldBrush);
	pDC->SetArcDirection(oldArcDirection);
	IncreaseId();
}