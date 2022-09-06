#pragma once
#include "afxwin.h"

class C2DrawTask
{
	public:
	static C2DrawTask& Create2DrawTask(const CRect& rect) {
		static C2DrawTask task;
		task.Init(rect);

		return task;
	}

	/*
	���񣺻�������
	��һ�������������ڣ��������ص������ص���ɫֵ�������ֵ��
	pDC->SetPixel();
	����: ���ص���ɫ����
	*/
	void TaskPoint_Array(CDC* pDC);

	/*
	���񣺻���ֱ��
	ͨ������ֱ�ߵķ�ʽ��������������
	pDC->MoveTo();
	pDC->LineTo();
	���ԣ��ߵ���ɫ���ԡ��ߵ��������ԣ��߿����ͣ���ͨ������ָ���ߵ�����
	���Ƶ�ֱ�ߣ�������㣬�������յ㣬����ҿ�
	*/
	void TaskLine_Triangle(CDC* pDC);

	/*
	���񣺻��ƾ���
	pDC->Rectangle()
	���ԣ����εı߽磨ͨ������ָ���������ε��ڲ��������ͨ����ˢָ����
	���Ƶľ��Σ�������߽���ϱ߽磬�������б߽���±߽磬����ҿ�
	*/
	void TaskRectangle(CDC* pDC);

	/*
	���񣺻�����Բ
	pDC->Ellipse()
	���ԣ���Բ�ı߽磨ͨ������ָ���߽����ԣ�����Բ���ڲ��������ͨ����ˢָ�����ԣ�
	*/
	void TaskEllipse(CDC* pDC);

	/*
	���񣺻�����Բ��
	pDC->Arc().ָ����Բ��������Բ�ľ�������rect��ָ����Բ���������յ㡣��Բ����Ĭ�Ϸ�������ʱ�롣
	pDC->SetArcDirection(). ����Բ���ķ���˳ʱ�����ʱ�룩
	˵���������յ㲻һ������Բ�ϣ�ֻҪ������Բ��������ָ������γɵ�ֱ����Բ���н��㣬�ý����Ϊ��Բ����������㡣�յ�ͬ��
	���ԣ���Բ��ֻ�б߽磬û���ڲ��������
	*/
	void TaskArc(CDC* pDC);

	/*
	���񣺻�����Բ����
	pDC->Pie().ָ����Բ����������Բ�ľ�������rect��ָ����Բ���ε������յ㡣��Բ���ε�Ĭ�Ϸ�������ʱ�롣
	pDC->SetArcDirection(). ����Բ���ķ���˳ʱ�����ʱ�룩������Բ�������εķ�����Ч��
	˵���������յ㲻һ������Բ�ϣ�ֻҪ������Բ��������ָ������γɵ�ֱ����Բ���н��㣬�ý����Ϊ��Բ���ε�������㡣�յ�ͬ��
	���ԣ���Բ�����б߽磬Ҳ���ڲ��������
	*/
	void TaskPie(CDC* pDC);
	
	/*
	���񣺲�����Բ + ��Բ���Σ����ư���ͼ
	*/
	void TaskPie_BaGuaGraph(CDC* pDC);

	/*
	���񣺻��ƶ����
	pDC->Ploygon()
	���ԣ��߽���������
	*/
	void TaskPolygon(CDC* pDC);

	/*
	���񣺶���η�ʽ���������
	pDC->SetPolyFillMode(); ���ö���ε����ģʽ��
		ALTERNATE��Ĭ��ģʽ��ˮƽɨ����䣬ɨ�����������ཻ���������ż��֮�������ɫ��
		WINDING: ��ȡ�������һ�㣬���������ߣ������������ཻ������εı߰���˳ʱ���
			��ʱ������ʾ����ʱ��-1��˳ʱ��+1�������������ཻ���ܺ�Ϊ0����䣬��֮���
	*/
	void TaskPolygon_FiveStar(CDC* pDC);

	/*
	����ʹ��ʵ����ɫ��������
	pDC->FillSolidRect(rect, color);
	*/
	void TaskFillSolidRect(CDC* pDC);

	/*
	����ʹ�û�ˢ��������
	pDC->FillRect(rect, brush);
	*/
	void TaskFillRect(CDC* pDC);

	/*
	����ʹ��·���������ƶ����
	pDC->BeginPath(); ��ʼ·����
	MoveTo()��LineTo() �����ƶ����
	pDC->EndPath();	����·����
	pDC->FillPath();ʹ�õ�ǰ��ˢ�����·��
	pDC->StrokeAndFillPath(); ʹ�õ�ǰ����������·���߽磬ʹ�õ�ǰ��ˢ�����·��
	*/
	void TaskPath(CDC* pDC);

	/*
	���񣺻���һ��3�׵�bezier����.��Ҫ�ĸ���
	pDC->Bezier(pts, ptNum);
	˵����ֻ֧��3��bezier���ߣ�����һ��bezier���ߣ������Ƕ�ι⻬��bezier����ƴ�ӡ�
	*/
	void TaskBezier_Curve(CDC* pDC);

	/*
	���񣺻���n�ι⻬������3��bezier���ߣ���Ҫ�ĵ���Ϊ(3n + 1)
	pDC->Bezier(pts, ptNum);
	˵����ֻ֧��3��bezier���ߣ�����һ��bezier���ߣ������Ƕ�ι⻬��bezier����ƴ�ӡ�
	*/
	void TaskBezier_MulSmoothCurve(CDC* pDC);


private:
	C2DrawTask() = default;
	C2DrawTask(const C2DrawTask&) = delete;
	C2DrawTask operator= (const C2DrawTask&) = delete;

	void Init(const CRect& rect) {
		screenLeftTop.x = -rect.Width() / 2;
		screenLeftTop.y = rect.Height() / 2;

		screenWidth = rect.Width();
		screenHeight = rect.Height();

		// edgeLen = screenHeight / 3 - 2 * margin; // ���ö�̬�߳�
		edgeLen = 150; // ���ù̶��߳�

		id = 0;
	}

	bool GetCurrentPosition(int& row, int& col)
	{
		long d = 2 * margin + edgeLen;
		long colNum = screenWidth / d;
		long rowNum = screenHeight / d;

		// 3 �� 4��
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

	void DrawPolygon(CDC* pDC, CPoint* pts, int num) {
		if (num < 2) {
			return;
		}

		pDC->MoveTo(pts[0]);
		for (int i = 1; i < num; ++i) {
			pDC->LineTo(pts[i]);
		}
	}
	void DrawPtByEllipse(CDC* pDC, CPoint* pts, int num) {
		if (num < 1) {
			return;
		}

		for (int i = 0; i < num; ++i) {
			pDC->Ellipse(pts[i].x - 5, pts[i].y + 5, pts[i].x + 5, pts[i].y - 5);
		}
	}

private:
	CPoint screenLeftTop{};
	long screenWidth = 0;
	long screenHeight = 0;
	long edgeLen = 0;
	int id = 0;
	const long margin = 20;
};

