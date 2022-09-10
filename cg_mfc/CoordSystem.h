#pragma once
class CoordSystem
{
public:

	/*
	设置屏幕视区坐标系。
	默认情况下，屏幕上坐标系：原点为屏幕左上角，X向右正向，Y向下正向。
							  也就是说，默认情况下，只能显示第一象限的视图，且坐标轴Y方向和理解上的Y轴方向相反
	 这里，规范化了坐标系统：原点为屏幕上的中点，X向右为正，Y向上为正。
							  也就是说，现在能显示四个象限的视图了
	*/
	static void SetNormalCoordSystem(CDC* pDC, CRect& rect);
};

