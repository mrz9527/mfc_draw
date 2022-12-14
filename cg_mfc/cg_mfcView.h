
// cg_mfcView.h: CcgmfcView 类的接口
//

#pragma once
#include "CBezier.h"

typedef CPoint2D<float> FloatPoint;
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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
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

protected:
	void DoubleBuffer(CDC* pDC);

private:

	void UpdateData() {
		for (int i = 0; i <= 1; ++i) {
			m_sLine[i] = m_ptArray[i];
		}

		for (int i = 1; i <= 4; ++i) {
			m_ctlPts[i - 1] = m_ptArray[i];
		}

		for (int i = 4; i <= 5; ++i) {
			m_eLine[i - 4] = m_ptArray[i];
		}
	}

	void Draw(CDC* pDC) {
		//DrawLine(pDC, m_sLine);
		DrawBezier(pDC);
		/*DrawLine(pDC, m_eLine);
		DrawEndPt(pDC);*/
	}

	template<typename Type>
	void DrawLine(CDC* pDC, CPoint2D<Type> line[2]) {
		CPen redPen, * oldPen;
		redPen.CreatePen(0, 1, RGB(255, 0, 0));
		oldPen = (CPen*)pDC->SelectObject(&redPen);
		
		for (int i = 0; i < 2; ++i) {
			CPoint pt = CPoint(int(line[i].x), int(line[i].y));
			if (i == 0) {
				pDC->MoveTo(pt);
			}
			else {
				pDC->LineTo(pt);
			}
		}

		pDC->SelectObject(oldPen);
	}
	
	void DrawEndPt(CDC* pDC) {
		CPen blackPen, * oldPen;
		blackPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

		CBrush blackBrush, * oldBrush;
		blackBrush.CreateSolidBrush(0);

		// 黑色画笔，绘制控制多边形
		oldPen = (CPen*)pDC->SelectObject(&blackPen);
		// 黑色画刷，绘制控制多边形顶点
		oldBrush = (CBrush*)pDC->SelectObject(&blackBrush);

		int ptCount = sizeof(m_ptArray) / sizeof(m_ptArray[0]);
		for (int i = 0; i < ptCount; ++i) {
			if (i != 0 && i != ptCount - 1) {
				continue;
			}
			CPoint pt = CPoint(int(m_ptArray[i].x + 0.5), int(m_ptArray[i].y + 0.5));
			CRect rect = CRect(pt.x - 4, pt.y + 4, pt.x + 4, pt.y - 4);
			pDC->Ellipse(rect);
		}
		

		// 绘制文字
		auto oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(128, 128, 128));

		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		lf.lfCharSet = GB2312_CHARSET;

		CFont font, * oldFont;
		font.CreateFontIndirectW(&lf);
		oldFont = (CFont*)pDC->SelectObject(&font);

		for (int i = 0; i < ptCount; ++i) {
			CPoint pt = CPoint(int(m_ptArray[i].x), int(m_ptArray[i].y));
			CString strPt;
			if (i == 0)
				strPt.Format(_T("s(%d, %d)"), pt.x, pt.y);
			else if (i == ptCount - 1)
				strPt.Format(_T("e(%d, %d)"), pt.x, pt.y);
			else
				continue;

			pDC->TextOutW(pt.x + 10, pt.y + 20, strPt);
		}

		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(oldFont);
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}

	void DrawBezier(CDC* pDC) {
		CBezier<FloatPoint>::DoBezier(pDC, m_ctlPts, sizeof(m_ctlPts) / sizeof(m_ctlPts[0]));
	}

	FloatPoint m_ptArray[6];

	// m_ptArray [0,1]构成第一条直线
	FloatPoint m_sLine[2];
	// m_ptArray [1,4]构成bezier控制点
	FloatPoint m_ctlPts[4];
	// m_ptArray [4,5]构成第二条直线
	FloatPoint m_eLine[2];

	bool m_bBtnDown;
	int m_selectPtId;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // cg_mfcView.cpp 中的调试版本
inline CcgmfcDoc* CcgmfcView::GetDocument() const
   { return reinterpret_cast<CcgmfcDoc*>(m_pDocument); }
#endif