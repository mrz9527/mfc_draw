
// cg_mfcView.h: CcgmfcView 类的接口
//

#pragma once
#include "CBezier.h"

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

		m_ptArray[4].x = m_ptArray[3].x + (m_ptArray[3].x - m_ptArray[2].x) * 1.5; // 2阶bezier1和3阶bezier2在交点处的比例为1.5，才能保证光滑连续
		m_ptArray[4].y = m_ptArray[3].y + (m_ptArray[3].y - m_ptArray[2].y) * 1.5;

		// sLine
		for (int i = 0; i <= 1; ++i) {
			m_sLine[i] = m_ptArray[i];
		}

		// bezier1
		for (int i = 1; i <= 3; ++i) {
			m_bezier1ctlPts[i - 1] = m_ptArray[i];
		}

		// bezier2
		for (int i = 3; i <= 6; ++i) {
			m_bezier2ctlPts[i - 3] = m_ptArray[i];
		}

		// eLine
		for (int i = 6; i <= 7; ++i) {
			m_eLine[i - 6] = m_ptArray[i];
		}
	}

	void Draw(CDC* pDC) {
		DrawLine(pDC, m_sLine);
		DrawLineK(pDC, m_sLine);
		DrawBezier(pDC, m_bezier1ctlPts, sizeof(m_bezier1ctlPts)/sizeof(m_bezier1ctlPts[0]));
		DrawBezier(pDC, m_bezier2ctlPts, sizeof(m_bezier2ctlPts)/sizeof(m_bezier2ctlPts[0]));
		DrawLine(pDC, m_eLine);

		CPoint2F pt34[2];
		pt34[0] = m_ptArray[3];
		pt34[1] = m_ptArray[4];
		DrawLineK(pDC, pt34);

		DrawLineK(pDC, m_eLine);
		DrawPt(pDC);
	}

	void DrawLine(CDC* pDC, CPoint2F line[2]) {
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

	float GetLineK(CPoint2F line[2])
	{
		return (line[0].y - line[1].y) / (line[0].x - line[1].x);
	}

	float GetLineK(CPoint2F sPt, CPoint2F ePt)
	{
		return (sPt.y - ePt.y) / (sPt.x - ePt.x);
	}

	float CalcPtBaseLine(CPoint2F sPt, float k, float x)
	{
		// y
		return k * (x - sPt.x) + sPt.y;
	}

	float CalcPtBaseLine(CPoint2F sPt, CPoint2F ePt, float x)
	{
		float k = GetLineK(sPt, ePt);
		// ePtY
		return CalcPtBaseLine(sPt, k, x);
	}

	CPoint2F GetMidPt(CPoint2F line[2])
	{
		return CPoint2F((line[0].x + line[1].x) / 2, (line[0].y + line[1].y) / 2);
	}
	
	void DrawPt(CDC* pDC) {
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
			strPt.Format(_T("p%d(%d, %d)"), i, pt.x, pt.y);

			pDC->TextOutW(pt.x + 10, pt.y + 20, strPt);
		}

		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(oldFont);
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}

	void DrawLineK(CDC* pDC, CPoint2F line[2]) {
		// 绘制文字
		auto oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(128, 128, 128));

		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		lf.lfCharSet = GB2312_CHARSET;

		CFont font, *oldFont;
		font.CreateFontIndirectW(&lf);
		oldFont = (CFont*)pDC->SelectObject(&font);

		// 寻找中点，是为了在中点位置输出直线斜率k
		CPoint2F midPt = GetMidPt(line);
		float lineK = GetLineK(line);

		CPoint pt = CPoint(int(midPt.x), int(midPt.y));
		CString strK;
		strK.Format(_T("k = %.4f"), lineK);
		pDC->TextOutW(pt.x + 10, pt.y + 20, strK);

		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(oldFont);
	}

	void DrawBezier(CDC* pDC, CPoint2F* ptArray, int count) {
		CBezier<CPoint2F>::DoBezier(pDC, ptArray, count);
	}

	CPoint2F m_ptArray[8];
	float m_k23;

	// m_ptArray [0,1]构成第一条直线
	CPoint2F m_sLine[2];
	// m_ptArray [1,3]构成bezier1控制点,共3个点
	CPoint2F m_bezier1ctlPts[3];
	// m_ptArray [3,6]构成bezier2控制点,共4个点
	CPoint2F m_bezier2ctlPts[4];
	// m_ptArray [6,7]构成第二条直线
	CPoint2F m_eLine[2];

	bool m_bBtnDown;
	int m_selectPtId;
	bool m_bFlush;

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