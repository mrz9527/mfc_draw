
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

#include"C2DrawTask.h"

// CcgmfcView

IMPLEMENT_DYNCREATE(CcgmfcView, CView)

BEGIN_MESSAGE_MAP(CcgmfcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CcgmfcView 构造/析构

CcgmfcView::CcgmfcView() noexcept
{
	// TODO: 在此处添加构造代码

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

// CcgmfcView 绘图
void CcgmfcView::SetNormalCoordSystem(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	// 自定义坐标系
	// 设置映射模式（作用：把图形显示到设备屏幕坐标系上）
	// MM_ANISOTROPIC:窗口范围和设备范围可以任意改变，要求使用SetWindowExt函数设置窗口的范围，使用SetViewPortExt函数设置视区的范围
	pDC->SetMapMode(MM_ANISOTROPIC);
	// 设置窗口范围函数.SetWindowExtEx(cx, cy); cx和cy是逻辑单位，m、cm、英寸等.
	pDC->SetWindowExt(rect.Width(), rect.Height());
	// 设置视区范围函数。SetViewPortExt(cx, cy). cx和cy是设备单位。默认屏幕坐标X向右为正，Y向下为正，这里改为向上为正，符合正常思维。
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	// 设置视区原点函数.设置原点为视区的中心
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	rect.OffsetRect(-rect.Width() / 2, -rect.Height());
}

void CcgmfcView::OnDraw(CDC *pDC)
{
	CcgmfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码坐标）

	SetNormalCoordSystem(pDC);

	CRect rect;
	GetClientRect(&rect);
	auto& task = C2DrawTask::Create2DrawTask();
	task.Init(rect);
	
	task.TaskPoint_Array(pDC);
	task.TaskLine_Triangle(pDC);
	task.TaskRectangle(pDC);
	task.TaskEllipse(pDC);
	task.TaskArc(pDC);
	task.TaskPie(pDC);
	task.TaskPie_BaGuaGraph(pDC);
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
