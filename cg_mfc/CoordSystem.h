#pragma once
class CoordSystem
{
public:

	/*
	������Ļ��������ϵ��
	Ĭ������£���Ļ������ϵ��ԭ��Ϊ��Ļ���Ͻǣ�X��������Y��������
							  Ҳ����˵��Ĭ������£�ֻ����ʾ��һ���޵���ͼ����������Y����������ϵ�Y�᷽���෴
	 ����淶��������ϵͳ��ԭ��Ϊ��Ļ�ϵ��е㣬X����Ϊ����Y����Ϊ����
							  Ҳ����˵����������ʾ�ĸ����޵���ͼ��
	*/
	static void SetNormalCoordSystem(CDC* pDC, CRect& rect);

	static CPoint TransformToNormalCoordSystem(const CRect& rect, CPoint pt);
};
