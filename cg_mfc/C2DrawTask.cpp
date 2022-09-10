#include "pch.h"
#include "C2DrawTask.h"
#include <cmath>
#include "Resource.h"

#include "CoordSystem.h"

const float MATH_PI = std::atan(1.0f) * 4.0f;

void C2DrawTask::TaskPoint_Array(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	srand(time(0));
	for (decltype(leftTop.x) x = leftTop.x; x <= rightBottom.x; ++x) {
		for (decltype(leftTop.y) y = leftTop.y; y >= rightBottom.y; --y) {
			COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
			pDC->SetPixelV(x, y, color);
		}
	}
	
	m_layout->IncreaseId();
}

void C2DrawTask::TaskLine_Triangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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

	m_layout->IncreaseId();
}

void C2DrawTask::TaskRectangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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

	m_layout->IncreaseId();
}

void C2DrawTask::TaskEllipse(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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

	m_layout->IncreaseId();
}

void C2DrawTask::TaskArc(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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

	m_layout->IncreaseId();
}

void C2DrawTask::TaskPie(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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
	m_layout->IncreaseId();
}

void C2DrawTask::TaskPie_BaGuaGraph(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
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
	m_layout->IncreaseId();
}

void C2DrawTask::TaskPolygon(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	CPoint pts[4];
	pts[0].x = (leftTop.x + rightBottom.x) / 2; // ��
	pts[0].y = leftTop.y - (leftTop.y - rightBottom.y) / 4;

	pts[1].x = leftTop.x; // ����
	pts[1].y = rightBottom.y;

	pts[2] = rightBottom; // ����

	pts[3] = CPoint(rightBottom.x, leftTop.y);

	// ����gdi���󣺻�ˢ
	CBrush brush;
	//brush.CreateSolidBrush(RGB(107, 54, 200)); // ʵ�廭ˢ
	brush.CreateHatchBrush(HS_BDIAGONAL, RGB(107, 54, 200));

	auto oldBrush = pDC->SelectObject(brush);
	pDC->Polygon(pts, sizeof(pts)/sizeof(pts[0]));

	pDC->SelectObject(oldBrush);

	m_layout->IncreaseId();
}

void C2DrawTask::TaskPolygon_FiveStar(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	CRect rect(leftTop, rightBottom);
	auto center = rect.CenterPoint();
	auto r = (leftTop.y - rightBottom.y) / 2;

	float intervalDegree = 360.0f / 5;
	float startDegree = 90.0f;

	CPoint pt[5];

	for (int i = 0; i < 5; ++i) {
		float degree = startDegree + intervalDegree * i;
		float rad = degree * 2 * MATH_PI / 360;
		float dx = r * cos(rad);
		float dy = r * sin(rad);
		pt[i] = CPoint((int)round(dx + 0.5) + center.x, (int)round(dy + 0.5) + center.y);
	}

	CPoint fiveStar[5];
	fiveStar[0] = pt[0];
	fiveStar[1] = pt[2];
	fiveStar[2] = pt[4];
	fiveStar[3] = pt[1];
	fiveStar[4] = pt[3];

	// ����gdi���󣺺�ɫ���ʺ���ɫ��ˢ
	CPen redPen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	auto oldPen = pDC->SelectObject(redPen);
	CBrush blueBrush;
	blueBrush.CreateSolidBrush(RGB(0, 0, 255));
	auto oldBrush = pDC->SelectObject(blueBrush);

	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(fiveStar, 5);

	pDC->SelectObject(redPen);
	pDC->SelectObject(blueBrush);

	m_layout->IncreaseId();
}

void C2DrawTask::TaskFillSolidRect(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	int n = 12;
	int xStep = (rightBottom.x - leftTop.x) / n;
	float colorStep = 256 / n;

	CRect firstRect(leftTop.x, leftTop.y, leftTop.x + (int)xStep, rightBottom.y);
	CRect rect = firstRect;
	for (int i = 0; i < 12; ++i) {
		rect.MoveToX(rect.right); // rect.right = leftTop.x + i * xStep 
		COLORREF color = RGB(xStep * i, xStep * i, xStep * i);
		pDC->FillSolidRect(rect, color);
	}

	m_layout->IncreaseId();
}

void C2DrawTask::TaskFillRect(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 255, 0));

	pDC->FillRect(CRect(leftTop, rightBottom), &brush);

	m_layout->IncreaseId();
}

void C2DrawTask::TaskPath(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;


	long tempY = abs(leftTop.y - rightBottom.y) / 3;
	CPoint pt[4];
	pt[0] = CPoint((leftTop.x + rightBottom.x) / 2, leftTop.y - tempY);
	pt[1] = CPoint(rightBottom.x, (leftTop.y + rightBottom.y) / 2);
	pt[2] = CPoint(pt[0].x, rightBottom.y + tempY);
	pt[3] = CPoint(leftTop.x, pt[1].y);

	// ��ʼ·����
	pDC->BeginPath();
	pDC->MoveTo(pt[0]);
	for (int i = 1; i < 4; ++i) {
		pDC->LineTo(pt[i]);
	}
	// �ر�·����
	pDC->EndPath();

	// ʹ�õ�ǰ��ˢ���·����
	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_BDIAGONAL, RGB(44, 44, 44));
	CBrush* oldBrush = (CBrush*)pDC->SelectObject(&hatchBrush);

	CPen redPen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	CPen* oldPen = (CPen*)pDC->SelectObject(&redPen);
	//pDC->FillPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	m_layout->IncreaseId();
}

void C2DrawTask::TaskBezier_Curve(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	CPoint pt[4];
	pt[0] = CPoint(leftTop.x, rightBottom.y);
	pt[1] = CPoint((leftTop.x + rightBottom.x) / 2, rightBottom.y);
	pt[2] = leftTop;
	pt[3] = CPoint(rightBottom.x, leftTop.y);

	COLORREF black(RGB(0, 0, 0)), red(RGB(255, 0, 0));
	CPen redPen, blackPen;
	redPen.CreatePen(0, 1, red);
	blackPen.CreatePen(0, 3, black);
	CBrush blackBrush;
	blackBrush.CreateSolidBrush(black);

	auto oldPen = pDC->SelectObject(blackPen);
	auto oldBrush = pDC->SelectObject(blackBrush);

	DrawPolygon(pDC, pt, sizeof(pt)/sizeof(pt[0]));
	DrawPtByEllipse(pDC, pt, sizeof(pt) / sizeof(pt[0]));

	pDC->SelectObject(redPen);

	pDC->PolyBezier(pt, sizeof(pt)/sizeof(pt[0]));

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	m_layout->IncreaseId();
}

void C2DrawTask::TaskBezier_MulSmoothCurve(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	const int n = 7;
	CPoint pt[n];
	pt[0] = CPoint(leftTop.x, rightBottom.y);
	pt[1] = CPoint(leftTop.x + (rightBottom.x - leftTop.x) / 4, rightBottom.y);
	pt[2] = CPoint(pt[0].x, (leftTop.y + rightBottom.y) / 2);
	pt[3] = CPoint((leftTop.x + rightBottom.x)/2, pt[2].y);

	pt[4] = CPoint(rightBottom.x, pt[3].y);
	pt[5] = CPoint(rightBottom.x - (rightBottom.x - leftTop.x) / 4, leftTop.y);
	pt[6] = CPoint(rightBottom.x, leftTop.y);

	COLORREF black(RGB(0, 0, 0)), red(RGB(255, 0, 0));
	CPen redPen, blackPen;
	redPen.CreatePen(0, 1, red);
	blackPen.CreatePen(0, 3, black);
	CBrush blackBrush;
	blackBrush.CreateSolidBrush(black);

	auto oldPen = pDC->SelectObject(blackPen);
	auto oldBrush = pDC->SelectObject(blackBrush);

	DrawPolygon(pDC, pt, sizeof(pt) / sizeof(pt[0]));
	DrawPtByEllipse(pDC, pt, sizeof(pt) / sizeof(pt[0]));

	pDC->SelectObject(redPen);

	pDC->PolyBezier(pt, sizeof(pt) / sizeof(pt[0]));

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	m_layout->IncreaseId();
}

//void C2DrawTask::TaskBitmap(CDC* pDC)
//{
//	CPoint leftTop, rightBottom;
//	if (!GetCurrentArea(leftTop, rightBottom))
//		return;
//
//	int nWidth = rightBottom.x - leftTop.x;
//	int nHeight = leftTop.y - rightBottom.y;
//
//	// �����ڴ�����豸������
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//
//	CRect rect(leftTop, rightBottom);
//
//	// ����λͼ����
//	CBitmap bitmap, * oldBitmap;
//	bitmap.LoadBitmapW(IDB_BITMAP_216);
//	BITMAP bmp;
//	bitmap.GetBitmap(&bmp);
//
//	CRect bitmapRect(0, 0, bmp.bmWidth, bmp.bmHeight);
//	oldBitmap = (CBitmap*)memDC.SelectObject(bitmap);
//
//	CoordSystem::SetNormalCoordSystem(&memDC, bitmapRect);
//
//
//
//	// ��ȡλͼ��Ϣ����
//	//BITMAP bmp; // BITMAP�ṹ�嶨�����߼�λͼ�Ŀ�ȡ��߶ȡ���ɫ��ʽ��λͼ���ֽ�����
//	//bitmap.GetBitmap(&bmp);
//	
//	// λ�鴫�ͺ���
//	// x, yĿ������������Ͻǵ���߼����꣬width,heightĿ����κ�Դλͼ�Ŀ�Ⱥ͸߶ȣ����߼������ʾ��psrcDC�����˽�Ҫ��������λͼ��
//	// xsrc, ysrc��Դλͼ�����Ͻǵ��߼����꣬dwrop�ǹ�դ�����롣gdi�ж��ֹ�դ�����룬��õ���srccopy����ʾ��Դλͼֱ�ӿ�����Ŀ��
//	// �豸��������
//	//pDC->BitBlt(leftTop.x, leftTop.y, nWidth, -nHeight, &memDC, 0, 0, SRCCOPY);
//
//	// ����λͼ����
//	// ���Ŀ�������Դλͼ�Ĵ�С��һ�£���Ҫ������λͼ������
//	pDC->StretchBlt(leftTop.x, leftTop.y, nWidth, -nHeight, &memDC, 0, 0, bitmapRect.Width(), -bitmapRect.Height(), SRCCOPY);
//	IncreaseId();
//}


void C2DrawTask::TaskText(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	// ����Ϊ͸��
	pDC->SetBkMode(TRANSPARENT);

	// ����������ַ���ɫ
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(lfont));
	lfont.lfCharSet = GB2312_CHARSET;
	lfont.lfHeight = 50;
	wcscpy_s(lfont.lfFaceName, _T("���Ĳ���"));
	CFont font;
	font.CreateFontIndirectW(&lfont);
	auto olfFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->TextOutW(leftTop.x, leftTop.y, _T("��ʫ��"));
	pDC->SelectObject((CFont*)olfFont);
	m_layout->IncreaseId();
}