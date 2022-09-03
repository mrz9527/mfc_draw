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

		// edgeLen = screenHeight / 3 - 2 * margin; // ���ö�̬�߳�
		edgeLen = 150; // ���ù̶��߳�

		id = 0;
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
	pDC->
	*/
	void TaskEllipse(CDC* pDC);

private:
	C2DrawTask() = default;
	C2DrawTask(const C2DrawTask&) = delete;
	C2DrawTask operator= (const C2DrawTask&) = delete;

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
private:
	CPoint screenLeftTop{};
	long screenWidth = 0;
	long screenHeight = 0;
	long edgeLen = 0;
	int id = 0;
	const long margin = 20;
};

