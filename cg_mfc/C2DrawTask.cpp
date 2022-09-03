#include "pch.h"
#include "C2DrawTask.h"


void C2DrawTask::TaskPt_PixelArray(CDC* pDC)
{
	CPoint leftBottom(100, 100), rightTop(200, 200);
	srand(time(0));
	for (int row = leftBottom.y; row <= rightTop.y; ++row) {
		for (int col = leftBottom.x; col <= rightTop.x; ++col) {
			COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
			pDC->SetPixelV(col, row, color);
		}
	}
}

void C2DrawTask::TaskLine_Triangle(CDC* pDC)
{
	CPoint p0(0, 100), p1(-100, -100), p2(100, -100);
	
	// 创建gdi（graphic device interface 图形设备接口）对象：画笔
	CPen redPen, greenPen, bluePen;
	redPen.CreatePen(0, 2, 0xFF0000);
	greenPen.CreatePen(0, 2, 0x00FF00);
	bluePen.CreatePen(0, 2, 0x0000FF);

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
}

