#pragma once
#include "afxwin.h"

class C2DrawTask
{
	public:
	/*
	���񣺻�������
	��һ�������������ڣ��������ص������ص���ɫֵ�������ֵ��
	*/
	static void TaskPt_PixelArray(CDC* pDC);

	/*
	���񣺻���ֱ��
	ͨ������ֱ�ߵķ�ʽ��������������
	*/
	static void TaskLine_Triangle(CDC* pDC);
};

