
// cg_mfcView.h: CcgmfcView 类的接口
//

#pragma once


class CcgmfcView : public CView
{
protected: // 仅从序列化创建
	CcgmfcView() noexcept;
	DECLARE_DYNCREATE(CcgmfcView)

// 特性
public:
	CcgmfcDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CcgmfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	/*
	设置屏幕视区坐标系。
	默认情况下，屏幕上坐标系：原点为屏幕左上角，X向右正向，Y向下正向。
							  也就是说，默认情况下，只能显示第一象限的视图，且坐标轴Y方向和理解上的Y轴方向相反
     这里，规范化了坐标系统：原点为屏幕上的中点，X向右为正，Y向上为正。
							  也就是说，现在能显示四个象限的视图。
	*/
	void CcgmfcView::SetNormalCoordSystem(CDC* pDC);

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // cg_mfcView.cpp 中的调试版本
inline CcgmfcDoc* CcgmfcView::GetDocument() const
   { return reinterpret_cast<CcgmfcDoc*>(m_pDocument); }
#endif

