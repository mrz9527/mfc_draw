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
	CBrush ellipseBrush;
	ellipseBrush.CreateSolidBrush(RGB(255, 255, 0));
	auto oldBrush = pDC->SelectObject(ellipseBrush);
	pDC->Ellipse(rect);
	
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(rect);
	
	pDC->SelectObject(oldBrush);

	IncreaseId();
}