
// cg_mfcView.cpp: CcgmfcView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg_mfc.h"
#endif

#include "cg_mfcDoc.h"
#include "cg_mfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CoordSystem.h"
#include "C2DrawTask.h"
#include "CLayout.h"
// CcgmfcView

IMPLEMENT_DYNCREATE(CcgmfcView, CView)

BEGIN_MESSAGE_MAP(CcgmfcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CcgmfcView 构造/析构

CcgmfcView::CcgmfcView() noexcept
{
	// TODO: 在此处添加构造代码

	m_ptArray[0].x = -250, m_ptArray[0].y = 120;
	m_ptArray[1].x = -200, m_ptArray[1].y = 100;
	//m_ptArray[2].x = 0, m_ptArray[2].y = 100;
	//m_ptArray[3].x = 0, m_ptArray[3].y = -100;
	m_ptArray[4].x = 200, m_ptArray[4].y = -100;
	m_ptArray[5].x = 300, m_ptArray[5].y = -150;

	m_ptArray[2].x = 0;
	m_ptArray[2].y = CalcPtBaseLine(m_ptArray[0], m_ptArray[1], m_ptArray[2].x);

	m_ptArray[3].x = 0;
	m_ptArray[3].y = CalcPtBaseLine(m_ptArray[4], m_ptArray[5], m_ptArray[3].x);

	UpdateData();

	m_bBtnDown = false;
	m_bFlush = false;
	m_selectPtId = -1;
}

CcgmfcView::~CcgmfcView()
{
}

BOOL CcgmfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CcgmfcView::DoubleBuffer(CDC* pDC)
{
	// 自定义pDC二维坐标系
	CRect originRect, rect;
	GetClientRect(&originRect);
	rect.CopyRect(originRect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 设置pDC坐标系为符合思维的标准坐标系
	CoordSystem::SetNormalCoordSystem(pDC, originRect);
	// 设置memDC坐标系，memDC坐标系与pDC坐标系完全一致
	CoordSystem::SetNormalCoordSystem(&memDC, originRect);

	// 为memDC创建兼容位图，颜色为黑色
	CBitmap bitmap, * oldBitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());  // 默认黑色位图
	oldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);

	// 设置客户区背景色，而客户区的区域由。LT(-502,210) RB(503,-211)
	//rect.SetRect(CPoint(-502, 210), CPoint(503, -211)); 和下面的效果一致
	rect.OffsetRect(-rect.Width() / 2, -rect.Height() / 2); // LT(-502,-210) RB(503,211)
	memDC.FillSolidRect(rect, RGB(255,250,0)); // 填充客户区背景色，覆盖黑色位图

	//rect.OffsetRect(-rect.Width()/2, -rect.Height()/2);

	// 在memDC上绘制图形
	Draw(&memDC);
	
	// memDC中的图形拷贝到pDC上
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, rect.left, rect.top, SRCCOPY);
	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}
void CcgmfcView::OnDraw(CDC *pDC)
{
	CcgmfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码坐标）
	
	DoubleBuffer(pDC);
}


// CcgmfcView 打印

BOOL CcgmfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CcgmfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CcgmfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CcgmfcView 诊断

#ifdef _DEBUG
void CcgmfcView::AssertValid() const
{
	CView::AssertValid();
}

void CcgmfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcgmfcDoc* CcgmfcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcgmfcDoc)));
	return (CcgmfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CcgmfcView 消息处理程序


void CcgmfcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bBtnDown = true;
	
	CView::OnLButtonDown(nFlags, point);
}


void CcgmfcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bBtnDown = false;
	m_bFlush = false;
	m_selectPtId = -1;

	CView::OnLButtonUp(nFlags, point);
}


void CcgmfcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	CPoint newPt = CoordSystem::TransformToNormalCoordSystem(rect, point);

	if (m_selectPtId == -1) {
		int ptCount = sizeof(m_ptArray) / sizeof(m_ptArray[0]);

		for (int i = 0; i < ptCount; ++i) {
				CPoint pt = CPoint(int(m_ptArray[i].x + 0.5), int(m_ptArray[i].y + 0.5));

				if (abs(pt.x - newPt.x) < 5 && abs(pt.y - newPt.y) < 5) {
					auto cursor = LoadCursor(nullptr, IDC_HAND);
					SetCursor(cursor);
					if (m_bBtnDown) {
						m_selectPtId = i;
						break;
					}
				}
		}
	}

	if (m_selectPtId >= 0) {
		m_ptArray[m_selectPtId].x = newPt.x;
		if (m_selectPtId == 2) { // 选中控制点2
			m_ptArray[m_selectPtId].y = CalcPtBaseLine(m_ptArray[0], m_ptArray[1], m_ptArray[m_selectPtId].x);
		}
		else if (m_selectPtId == 3) { // 选中控制点3
			m_ptArray[m_selectPtId].y = CalcPtBaseLine(m_ptArray[4], m_ptArray[5], m_ptArray[m_selectPtId].x);
		}
		else if (m_selectPtId < 2) { //选中直线sLine
			m_ptArray[m_selectPtId].y = newPt.y;

			// 更新控制点2，因为控制点必须与直线sLine共线
			m_ptArray[2].y = CalcPtBaseLine(m_ptArray[0], m_ptArray[1], m_ptArray[2].x);
		}
		else if (m_selectPtId > 3) { // 选中直线eLine
			m_ptArray[m_selectPtId].y = newPt.y;

			// 更新控制点3，因为控制点必须与直线eLine共线
			m_ptArray[3].y = CalcPtBaseLine(m_ptArray[4], m_ptArray[5], m_ptArray[3].x);
		}

		UpdateData();
		Invalidate(false);
	}

	CView::OnMouseMove(nFlags, point);
}
