视图区绘图：
1. 视图区坐标系问题。
	默认的屏幕坐标系：坐标原点在屏幕左上角，x向右为正，y向下为正。缺点，只能显示第一象限，且y坐标方向与认知相反。
	更改坐标系。更改后，原点在屏幕正中心，x向右为正，y向上为正。

2. CDC* pDC。图形上下文

3. GDI。图形设备接口。
	gdi对象包括：
	画笔：CPen。用于指定边界线的属性。包括边界线的样式（实线、电话线、双点划线等）、线宽（默认一像素线宽）、颜色（默认黑色）。
		作用于直线LineTo、椭圆ellipse、椭圆弧Arc、椭圆扇形Pie、多边形Ploygon等的边界。
	画刷：CBrush。用于指定封闭图形的填充属性。包括填充的样式（包括实体填充、阴影填充、位图填充）。
		作用于椭圆Ellipse、椭圆扇形Pie、多边形Ploygon等封闭区域的内部。
	字体：CFont。用于指定文字的属性。
	位图：CBitmap。主要是DDB（设备相关位图），一般的bitmap文件（.bmp)是DIB（设备无关位图）。
		 将.bmp文件加载到内存中的DDB中。

4. 绘制图形
	绘制像素点：pDC->SetPixel();
	绘制直线： pDC->MoveTo(起点); pDC->LineTo(终点)
	绘制矩形： pDC->Rectangle();
	绘制椭圆: pDC->Ellipse();
	绘制椭圆弧: pDC->Arc();
	绘制椭圆扇区: pDC->Pie();
	绘制多边形: pDC->Ploygon();

5. 填充
	矩形实体填充: FillSolidRect(rect, color); 用实体颜色填充矩形区域。
		和画刷的区别是：矩形实体填充不支持阴影和位图填充。
		和绘制矩形pDC->Rectangle的区别是：矩形实体填充不绘制边界。
	矩形填充：FillRect(rect, brush): 用画刷填充矩形区域。
		和FillSolidRect区别：使用画刷填充，可以是实体颜色、阴影、位图填充
		和绘制矩形pDC->Rectangle的区别是：矩形填充不绘制边界。
		不需要通过pDC->SelectObject()来选择画刷，而是通过参数传递的方式传递画刷，所以矩形填充结束后，不需要恢复到默认画刷。
									




	CPen pen;
	pen.CreatePen(0, 3, 0xff0000);