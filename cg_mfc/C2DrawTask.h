#pragma once
#include "afxwin.h"

class C2DrawTask
{
	public:
	static C2DrawTask& Create2DrawTask() {
		static C2DrawTask task;
		return task;
	}

	void Init(const CRect& rect) {
		screenLeftTop.x = -rect.Width() / 2;
		screenLeftTop.y = rect.Height() / 2;

		screenWidth = rect.Width();
		screenHeight = rect.Height();

		// edgeLen = screenHeight / 3 - 2 * margin; // 设置动态边长
		edgeLen = 150; // 设置固定边长

		id = 0;
	}

	/*
	任务：绘制像素
	在一个正方形区域内，设置像素点阵，像素的颜色值采用随机值。
	pDC->SetPixel();
	属性: 像素的颜色属性
	*/
	void TaskPoint_Array(CDC* pDC);

	/*
	任务：绘制直线
	通过绘制直线的方式，来绘制三角形
	pDC->MoveTo();
	pDC->LineTo();
	属性：线的颜色属性、线的类型属性（线宽、线型），通过画笔指定线的类型
	绘制的直线，包括起点，不包括终点，左闭右开
	*/
	void TaskLine_Triangle(CDC* pDC);

	/*
	任务：绘制矩形
	pDC->Rectangle()
	属性：矩形的边界（通过画笔指定）、矩形的内部填充区域（通过画刷指定）
	绘制的矩形，包括左边界和上边界，不包括有边界和下边界，左闭右开
	*/
	void TaskRectangle(CDC* pDC);

	/*
	任务：绘制椭圆
	pDC->Ellipse()
	属性：椭圆的边界（通过画笔指定边界属性）、椭圆的内部填充区域（通过画刷指定属性）
	*/
	void TaskEllipse(CDC* pDC);

	/*
	任务：绘制椭圆弧
	pDC->Arc().指定椭圆弧所在椭圆的矩形区域rect，指定椭圆弧的起点和终点。椭圆弧的默认方向是逆时针。
	pDC->SetArcDirection(). 设置圆弧的方向（顺时针或逆时针）
	说明：起点和终点不一定在椭圆上，只要连接椭圆的中心与指定起点形成的直线与圆弧有交点，该交点就为椭圆弧的真正起点。终点同理。
	属性：椭圆弧只有边界，没有内部填充区域。
	*/
	void TaskArc(CDC* pDC);

	/*
	任务：绘制椭圆扇形
	pDC->Pie().指定椭圆扇形所在椭圆的矩形区域rect，指定椭圆扇形的起点和终点。椭圆扇形的默认方向是逆时针。
	pDC->SetArcDirection(). 设置圆弧的方向（顺时针或逆时针）。对椭圆弧和扇形的方向有效。
	说明：起点和终点不一定在椭圆上，只要连接椭圆的中心与指定起点形成的直线与圆弧有交点，该交点就为椭圆扇形的真正起点。终点同理。
	属性：椭圆扇形有边界，也有内部填充区域。
	*/
	void TaskPie(CDC* pDC);
	
	/*
	任务：采用椭圆 + 椭圆扇形，绘制八卦图
	*/
	void TaskPie_BaGuaGraph(CDC* pDC);

private:
	C2DrawTask() = default;
	C2DrawTask(const C2DrawTask&) = delete;
	C2DrawTask operator= (const C2DrawTask&) = delete;

	bool GetCurrentPosition(int& row, int& col)
	{
		long d = 2 * margin + edgeLen;
		long colNum = screenWidth / d;
		long rowNum = screenHeight / d;

		// 3 行 4列
		row = id / colNum;
		col = id % colNum;
		return row < rowNum;
	}

	bool GetCurrentArea(CPoint& leftTop, CPoint& rightBottom) {
		int row, col;
		if (!GetCurrentPosition(row, col))
			return false;

		long d = 2 * margin + edgeLen;
		leftTop.x = screenLeftTop.x + d * col + margin;
		leftTop.y = screenLeftTop.y - d * row - margin;

		rightBottom.x = leftTop.x + edgeLen;
		rightBottom.y = leftTop.y - edgeLen;

		return true;
	}

	void IncreaseId() {
		++id;
	}
private:
	CPoint screenLeftTop{};
	long screenWidth = 0;
	long screenHeight = 0;
	long edgeLen = 0;
	int id = 0;
	const long margin = 20;
};

