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
	p0.x = (leftTop.x + rightBottom.x) / 2; // 上
	p0.y = leftTop.y - (leftTop.y - rightBottom.y) / 4;

	p1.x = leftTop.x; // 左下
	p1.y = rightBottom.y;

	p2 = rightBottom; // 右下
	
	// 创建gdi（graphic device interface 图形设备接口）对象：画笔
	CPen redPen, greenPen, bluePen;
	redPen.CreatePen(0, 2, RGB(255, 0, 0));
	greenPen.CreatePen(0, 2, RGB(0, 255, 0));
	bluePen.CreatePen(0, 2, RGB(0, 0, 255));

	// 选择画笔，返回旧的画笔，绘制三角形
	auto oldPen = pDC->SelectObject(redPen);
	pDC->MoveTo(p0);
	pDC->LineTo(p1);

	pDC->SelectObject(greenPen);
	pDC->LineTo(p2);

	pDC->SelectObject(bluePen);
	pDC->LineTo(p0);

	// 还原画笔为旧画笔
	pDC->SelectObject(oldPen);

	m_layout->IncreaseId();
}

void C2DrawTask::TaskRectangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	// 创建gdi对象：画刷、画笔
	// 画刷主要有三种类型：实例画刷SolidBrush，阴影画刷HatchBrish，图案画刷PatternBrush
	CBrush brush;
	brush.CreateSolidBrush(RGB(55, 0, 55));
	CPen redPen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));

	// 选择画刷，保存旧画刷
	auto oldBrush = pDC->SelectObject(brush);
	auto oldPen = pDC->SelectObject(redPen);

	// 绘制矩形
	pDC->Rectangle(CRect(leftTop, rightBottom));

	// 释放画刷
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

	// gdi对象：椭圆区域画刷，矩形区域画刷
	// 先画椭圆，再画矩形，矩形的填充区域会覆盖椭圆，为了使椭圆显示出来，可以为矩形的填充区选择透明画刷NULL_BRUSH。
	CBrush ellipseBrush;
	ellipseBrush.CreateSolidBrush(RGB(255, 255, 0));
	auto oldBrush = pDC->SelectObject(ellipseBrush);
	pDC->Ellipse(rect);
	
	pDC->SelectStockObject(NULL_BRUSH); // 选择系统库中的白色画刷
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

	// 绘制起始射线和终止射线
	pDC->MoveTo(rect.CenterPoint());
	pDC->LineTo(startPt);

	pDC->MoveTo(rect.CenterPoint());
	pDC->LineTo(endPt);
	
	// 创建gdi对象：两个画笔
	CPen redPen, bluePen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	bluePen.CreatePen(0, 3, RGB(0, 0, 255));


	// 选择红色画笔，逆时针绘制椭圆弧；蓝色画笔，顺时针绘制椭圆弧
	auto oldPen = pDC->SelectObject(redPen);
	pDC->Arc(rect, startPt, endPt);

	pDC->SelectObject(bluePen);
	auto oldArcDirection = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(rect, startPt, endPt);

	// 恢复默认pDC状态
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


	// 创建gdi对象：两个画笔，创建两个画刷
	CPen redPen, bluePen;
	redPen.CreatePen(0, 3, RGB(255, 0, 0));
	bluePen.CreatePen(0, 3, RGB(0, 0, 255));
	CBrush redBrush, blueBrush;
	redBrush.CreateSolidBrush(RGB(255, 0, 0));
	blueBrush.CreateSolidBrush(RGB(0, 0, 255));

	// 选择红色画笔，逆时针绘制扇形；蓝色画笔，顺时针绘制扇形
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

	// 创建gdi对象:创建两个画刷，创建黑色画笔
	CBrush whiteBrush, blackBrush;
	whiteBrush.CreateSolidBrush(RGB(255, 255, 255));
	blackBrush.CreateSolidBrush(RGB(0, 0, 0));
	CPen whitePen;
	whitePen.CreatePen(0, 1, RGB(255, 255, 255));

	// 绘制最外面大圆（两个半圆扇区）
	auto oldBrush = pDC->SelectObject(whiteBrush);
	pDC->Pie(bigRect, startPt, endPt);
	pDC->SelectObject(blackBrush);
	auto oldArcDirection = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Pie(bigRect, startPt, endPt);

	// 绘制第二梯度的圆（两个完整的圆，下圆黑色，上圆白色）
	long midR = (leftTop.y - rightBottom.y) / 4;
	CPoint midCenterUpper(bigCenter.x, bigCenter.y + midR);
	CPoint midCenterDown(bigCenter.x, bigCenter.y - midR);
	CRect midRectUpper(midCenterUpper.x - midR, midCenterUpper.y + midR, midCenterUpper.x + midR, midCenterUpper.y - midR);
	CRect midRectDown(midCenterDown.x - midR, midCenterDown.y + midR, midCenterDown.x + midR, midCenterDown.y - midR);
	
	// 绘制第二梯度的上圆
	pDC->SelectObject(whiteBrush);
	auto oldPen = pDC->SelectObject(whitePen); // 为了去除白色填充圆的黑色边界
	pDC->Ellipse(midRectUpper);

	// 绘制第二梯度的下圆
	pDC->SelectObject(oldPen);
	pDC->SelectObject(blackBrush);
	pDC->Ellipse(midRectDown);

	// 绘制第三梯度的圆（两个完整的圆，上圆黑色，下圆白色）
	long smallR = midR / 3;
	CPoint smallCenterUpper(midCenterUpper), smallCenterDown(midCenterDown);
	CRect smallRectUpper(smallCenterUpper.x - smallR, smallCenterUpper.y + smallR, smallCenterUpper.x + smallR, smallCenterUpper.y - smallR);
	CRect smallRectDown(smallCenterDown.x - smallR, smallCenterDown.y + smallR, smallCenterDown.x + smallR, smallCenterDown.y - smallR);

	// 绘制第三梯度的上圆（黑色）
	pDC->SelectObject(blackBrush);
	//auto oldPen = pDC->SelectObject(whitePen); // 为了去除白色填充圆的黑色边界
	pDC->Ellipse(smallRectUpper);

	// 绘制第二梯度的下圆
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
	pts[0].x = (leftTop.x + rightBottom.x) / 2; // 上
	pts[0].y = leftTop.y - (leftTop.y - rightBottom.y) / 4;

	pts[1].x = leftTop.x; // 左下
	pts[1].y = rightBottom.y;

	pts[2] = rightBottom; // 右下

	pts[3] = CPoint(rightBottom.x, leftTop.y);

	// 创建gdi对象：画刷
	CBrush brush;
	//brush.CreateSolidBrush(RGB(107, 54, 200)); // 实体画刷
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

	// 创建gdi对象：红色画笔和蓝色画刷
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

	// 开始路径层
	pDC->BeginPath();
	pDC->MoveTo(pt[0]);
	for (int i = 1; i < 4; ++i) {
		pDC->LineTo(pt[i]);
	}
	// 关闭路径层
	pDC->EndPath();

	// 使用当前画刷填充路径层
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
//	// 创建内存兼容设备上下文
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//
//	CRect rect(leftTop, rightBottom);
//
//	// 导入位图函数
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
//	// 获取位图信息函数
//	//BITMAP bmp; // BITMAP结构体定义了逻辑位图的宽度、高度、颜色格式、位图的字节数据
//	//bitmap.GetBitmap(&bmp);
//	
//	// 位块传送函数
//	// x, y目标矩形区域左上角点的逻辑坐标，width,height目标矩形和源位图的宽度和高度，用逻辑坐标表示。psrcDC包含了将要被拷贝的位图，
//	// xsrc, ysrc是源位图的左上角点逻辑坐标，dwrop是光栅操作码。gdi有多种光栅操作码，最常用的是srccopy，表示将源位图直接拷贝到目标
//	// 设备上下文中
//	//pDC->BitBlt(leftTop.x, leftTop.y, nWidth, -nHeight, &memDC, 0, 0, SRCCOPY);
//
//	// 拉伸位图函数
//	// 如果目标矩形与源位图的大小不一致，需要用拉伸位图函数。
//	pDC->StretchBlt(leftTop.x, leftTop.y, nWidth, -nHeight, &memDC, 0, 0, bitmapRect.Width(), -bitmapRect.Height(), SRCCOPY);
//	IncreaseId();
//}


void C2DrawTask::TaskText(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!m_layout->GetCurrentArea(leftTop, rightBottom))
		return;

	// 设置为透明
	pDC->SetBkMode(TRANSPARENT);

	// 设置字体和字符颜色
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(lfont));
	lfont.lfCharSet = GB2312_CHARSET;
	lfont.lfHeight = 50;
	wcscpy_s(lfont.lfFaceName, _T("华文彩云"));
	CFont font;
	font.CreateFontIndirectW(&lfont);
	auto olfFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->TextOutW(leftTop.x, leftTop.y, _T("周诗琦"));
	pDC->SelectObject((CFont*)olfFont);
	m_layout->IncreaseId();
}