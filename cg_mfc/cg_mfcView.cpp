
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
	m_ptArray[2].x = 0, m_ptArray[2].y = 100;
	m_ptArray[3].x = 0, m_ptArray[3].y = -100;
	m_ptArray[4].x = 200, m_ptArray[4].y = -100;
	m_ptArray[5].x = 300, m_ptArray[5].y = -150;

	UpdateData();

	m_bBtnDown = false;
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
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);


	CLayout* layout = &CLayout::GetInstance();
	layout->Init(rect);
	C2DrawTask::DoTask2D(pDC, layout);

	//CBezier<CPoint2F>::DoBezier(pDC, m_ptArray, sizeof(m_ptArray)/sizeof(m_ptArray[0]));

	//Draw(pDC);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0,0, SRCCOPY);	
}
void CcgmfcView::OnDraw(CDC *pDC)
{
	CcgmfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码坐标）
	CRect rect;
	GetClientRect(&rect);

	CoordSystem::SetNormalCoordSystem(pDC, rect);
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
	//m_bBtnDown = true;
	
	CView::OnLButtonDown(nFlags, point);
}


void CcgmfcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_bBtnDown = false;

	CView::OnLButtonUp(nFlags, point);
}


void CcgmfcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rect;
	//GetClientRect(&rect);
	//CPoint newPt = CoordSystem::TransformToNormalCoordSystem(rect, point);

	//m_selectPtId = -1;
	//int ptCount = sizeof(m_ptArray) / sizeof(m_ptArray[0]);

	//for (int i = 0; i < ptCount; ++i) {
	//	if (i > 0 && i < ptCount - 1) { // sLine和eLine暂时不考虑
	//		CPoint pt = CPoint(int(m_ptArray[i].x + 0.5), int(m_ptArray[i].y + 0.5));

	//		if (abs(pt.x - newPt.x) < 5 && abs(pt.y - newPt.y) < 5) {
	//			auto cursor = LoadCursor(nullptr, IDC_HAND);
	//			SetCursor(cursor);
	//			m_selectPtId = i;
	//			if (m_bBtnDown) {
	//				m_ptArray[i].x = newPt.x;
	//				m_ptArray[i].y = newPt.y;
	//				break;
	//			}
	//		}
	//	}
	//}

	//if (m_selectPtId >= 0) {
	//	UpdateData();
	//	Invalidate(false);
	//}

	CView::OnMouseMove(nFlags, point);
}
