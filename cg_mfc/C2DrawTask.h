#pragma once
#include "afxwin.h"

class C2DrawTask
{
	public:
	/*
	任务：绘制像素
	在一个正方形区域内，设置像素点阵，像素的颜色值采用随机值。
	*/
	static void TaskPt_PixelArray(CDC* pDC);

	/*
	任务：绘制直线
	通过绘制直线的方式，来绘制三角形
	*/
	static void TaskLine_Triangle(CDC* pDC);
};

