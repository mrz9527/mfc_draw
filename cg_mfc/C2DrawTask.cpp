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

	IncreaseId();
}

void C2DrawTask::TaskRectangle(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
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

	IncreaseId();
}

void C2DrawTask::TaskEllipse(CDC* pDC)
{
	CPoint leftTop, rightBottom;
	if (!GetCurrentArea(leftTop, rightBottom))
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
	IncreaseId();
}