#pragma once

#include <vector>
using std::vector;

#define FLOAT_PRECISION 0.000001f

struct CPoint2F {
	float x = 0.0f;
	float y = 0.0f;

public:
	CPoint2F() {
		x = 0.0f;
		y = 0.0f;
	}

	CPoint2F(float ix, float iy) {
		x = ix; 
		y = iy;
	}
	CPoint2F(const CPoint2F& other) {
		this->x = other.x;
		this->y = other.y;
	}

	CPoint2F operator*(float a) {
		this->x *= a;
		this->y *= a;
		return *this;
	}

	CPoint2F operator+(const CPoint2F& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	CPoint2F& operator+=(const CPoint2F& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
};

// ����bezier���߲���t�Ĳ�������
enum class EBezierStepType{
	BST_T, // ��bezier���ߵȲ���t�����ֲ���
	BST_ARC // ��bezier���ߵȻ������ֲ���
};

template<typename PointType>
class CBezier
{
public:
	static void DoBezier(CDC* pDC, PointType* ptArray, int count) {
		static CBezier bezier;
		bezier.Init(ptArray, count);
		bezier.MakeBezier();
		bezier.Draw(pDC);
	}

	void Init(PointType*ptArray, int count, float step = 0.01f, EBezierStepType stepType = EBezierStepType::BST_T) {
		m_ctlPtArray = ptArray;
		m_ctlPtCount = count;

		m_step = step;
		m_stepType = stepType;

		m_cn = nullptr;
	}

	CBezier() {
		ReleaseCn();
	}

	void Draw(CDC* pDC) {
		DrawBezier(pDC);
		DrawControlPloygon(pDC);
		/*DrawControlPt(pDC);
		DrawControlPtText(pDC);*/
	}

	bool MakeBezier() {
		ReleaseBezierPtArray();

		if (!MakeCn()) return false;

		int bezierPtCount = 1.0f / m_step + 1;
		m_bezierPtArray.reserve(bezierPtCount);

		float t = 0.0f;
		for (int i = 0; i < bezierPtCount && t < 1.0f; ++i, t += m_step) {
			PointType pt = MakeBn(t);
			m_bezierPtArray.push_back(pt);
		}

		if (t - 1.0f > FLOAT_PRECISION) {
			m_bezierPtArray.push_back(m_ctlPtArray[m_ctlPtCount - 1]);
		}

		return true;
	}

	vector<PointType> GetBezierPtArray() {
		return m_bezierPtArray;
	}


private:
	void DrawBezier(CDC* pDC) {
		CPen redPen, * oldPen;
		redPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		
		// ��ɫ���ʣ�����bezier����
		oldPen = (CPen*)pDC->SelectObject(&redPen);
		for (int i = 0; i < m_bezierPtArray.size(); ++i) {
			CPoint pt = CPoint(int(m_bezierPtArray[i].x + 0.5), int(m_bezierPtArray[i].y + 0.5));
			if (i == 0) {
				pDC->MoveTo(pt);
			}
			else {
				pDC->LineTo(pt);
			}
		}

		pDC->SelectObject(oldPen);
	}

	void DrawControlPloygon(CDC* pDC) {
		CPen blackPen, * oldPen;
		blackPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

		// ��ɫ���ʣ����ƿ��ƶ����
		oldPen = (CPen*)pDC->SelectObject(&blackPen);
		for (int i = 0; i < m_ctlPtCount; ++i) {
			CPoint pt = CPoint(int(m_ctlPtArray[i].x + 0.5), int(m_ctlPtArray[i].y + 0.5));
			if (i == 0) {
				pDC->MoveTo(pt);
			}
			else {
				pDC->LineTo(pt);
			}
		}

		pDC->SelectObject(oldPen);
	}

	void DrawControlPt(CDC* pDC) {
		CPen blackPen, * oldPen;
		blackPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

		CBrush blackBrush, * oldBrush;
		blackBrush.CreateSolidBrush(0);

		// ��ɫ���ʣ����ƿ��ƶ����
		oldPen = (CPen*)pDC->SelectObject(&blackPen);
		// ��ɫ��ˢ�����ƿ��ƶ���ζ���
		oldBrush = (CBrush*)pDC->SelectObject(&blackBrush);
		
		for (int i = 0; i < m_ctlPtCount; ++i) {
			CPoint pt = CPoint(int(m_ctlPtArray[i].x + 0.5), int(m_ctlPtArray[i].y + 0.5));
			CRect rect = CRect(pt.x - 4, pt.y + 4, pt.x + 4, pt.y - 4);
			pDC->Ellipse(rect);
		}

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}

	void DrawControlPtText(CDC* pDC) {
		auto oldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(128, 128, 128));

		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		lf.lfCharSet = GB2312_CHARSET;
		
		CFont font, *oldFont;
		font.CreateFontIndirectW(&lf);
		oldFont = (CFont*)pDC->SelectObject(&font);

		for (int i = 0; i < m_ctlPtCount; ++i) {
			CPoint pt = CPoint(int(m_ctlPtArray[i].x), int(m_ctlPtArray[i].y));
			CString strPt;
			strPt.Format(_T("p%d(%d, %d)"), i, pt.x, pt.y);
			pDC->TextOutW(pt.x + 10, pt.y + 20, strPt);
		}

		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(oldFont);
	}

	void ReleaseBezierPtArray() {
		vector<PointType>().swap(m_bezierPtArray);
	}

	bool MakeCn() {
		// ���Ƶ�������2��������������һ��ֱ��
		if (!m_ctlPtArray || m_ctlPtCount <= 1) {
			return false;
		}

		
		int n = m_ctlPtCount - 1; // nΪ�ײ㣬n=5
		// c(n,0) = c(n,n) ... c(n,i) = c(n, n-i)��ֻ��Ҫ���ڶԳƵ㣬����һ������ݼ���
		int midIndex = n / 2;  //n = 5, midIndex = 2.  0,1,2 3,4,5
								// n = 6, midIndex = 3, 0,1,2,3,4,5,6
								// n = 2, midIndex=1, 0,1,2
								// n = 1, midIndex = 0,  0,1
		
		m_cn = new (std::nothrow) float[m_ctlPtCount];
		if (!m_cn) return false;

		m_cn[0] = 1;

		// ����[0, midIndex]֮���ֵ
		for (int i = 1; i <= midIndex; ++i) {
			if (i == 1) {
				m_cn[i] = n;
			}
			else {
				// c(n,i) = n! / (n-i)! i! 
			   // c(n, i+1) =  n! / (i+1)! (n-i-1)!
				//          =  c(n, i) * (n-1)/(i+1)
				m_cn[i] = m_cn[i - 1] * (n - 1) / (i + 1);
			}
		}

		// ����ԳƵ��ֵ
		for (int i = midIndex + 1; i <= n; ++i) {
			m_cn[i] = m_cn[n - i];
		}
		return true;
	}

	void ReleaseCn() {
		if (m_cn) {
			delete[] m_cn;
			m_cn = nullptr;
		}
	}

	PointType MakeBn(float t) {
		PointType pt;
		int n = m_ctlPtCount - 1;
		for (int i = 0; i < m_ctlPtCount; ++i) { // sigma(0,n)  { p[i] * c(n,i) * t^i * (1 - t)^(n-i) }
			PointType ctlPt{ m_ctlPtArray[i].x, m_ctlPtArray[i].y };
			pt += ctlPt * m_cn[i] * pow(t, i) * pow(1 - t, n - i);
		}
		return pt;
	}

	PointType* m_ctlPtArray;
	int m_ctlPtCount;
	// ��ǰ���㲢���棬��߼���Ч�� C(n,i)
	// ������Ƶ�n+1����Ϊn�����ߣ�C(n,i)��i��0��n������n+1��C(n,i)��ֵ������m_ctlPtCount = n+1��m_cnҲ��n+1
	float* m_cn;
	float m_step;
	EBezierStepType m_stepType;

	vector<PointType> m_bezierPtArray;
};

