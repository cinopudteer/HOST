
// testView.cpp: CtestView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "test.h"
#endif

#include "testDoc.h"
#include "testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestView

IMPLEMENT_DYNCREATE(CtestView, CView)

BEGIN_MESSAGE_MAP(CtestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()//新加的
	ON_BN_CLICKED(IDC_BUTTON_PLAY_CARDS,&CtestView::OnPlayCards)//
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CtestView 构造/析构

CtestView::CtestView() noexcept
{
	// TODO: 在此处添加构造代码
	m_cardWidth = 80;
	m_cardHeight = 105;

}

CtestView::~CtestView()
{
	CtestDoc* pDoc = GetDocument();
	POSITION pos = pDoc->m_cardListHand.GetHeadPosition();
	while (pos) {
		delete pDoc->m_cardListHand.GetNext(pos);
	}
	pDoc->m_cardListHand.RemoveAll();

	pos = pDoc->m_playedCardList.GetHeadPosition();
	while (pos)
	{
		delete pDoc->m_playedCardList.GetNext(pos);
	}
	pDoc->m_playedCardList.RemoveAll();
}

BOOL CtestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CtestView 绘图

void CtestView::OnDraw(CDC* pDC)
{
	CtestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	// 为了防止闪烁，使用双缓冲技术
	CRect clientRect;
	GetClientRect(&clientRect);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC.SelectObject(&memBitmap);
	//memDC.FillSolidRect(clientRect, RGB(0, 128, 0)); // 绿色桌布背景
	CDC* pbk = &memDC;
	pDoc->backGround.Draw(pbk->GetSafeHdc(), clientRect);

	DrawList(pDoc->m_leftPlayerHand, &memDC);
	DrawList(pDoc->m_rightPlayerHand, &memDC);
	DrawList(pDoc->m_cardListHand, &memDC);
	DrawList(pDoc->m_playedCardList, &memDC);
	DrawList(pDoc->m_cardListAtTop, &memDC);

	if (m_pAnimatingCard != nullptr)
	{
		m_pAnimatingCard->Draw(&memDC);
	}

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
}


// CtestView 打印

BOOL CtestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CtestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CtestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CtestView 诊断

#ifdef _DEBUG
void CtestView::AssertValid() const
{
	CView::AssertValid();
}

void CtestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtestDoc* CtestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestDoc)));
	return (CtestDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestView 消息处理程序

void CtestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CtestDoc* pDoc = GetDocument();
	POSITION pos = pDoc->m_cardListHand.GetTailPosition();
	while (pos) {
		CCard* pCard = (CCard*)pDoc->m_cardListHand.GetPrev(pos);
		if (pCard->HitTest(point)) {
			pCard->m_bIsSelected = !pCard->m_bIsSelected;
			if (pCard->m_bIsSelected)
			{
				pCard->m_rect.OffsetRect(0, -20); // 向上移动20像素
			}
			else
			{
				pCard->m_rect.OffsetRect(0, 20); // 向下移动20像素
			}
			break;
		}
	}
	CString str;
	str.Format(_T("x:%d,y:%d"), point.x, point.y);
	MessageBox(str);
	Invalidate(); // 重绘
	CView::OnLButtonDown(nFlags, point);
}

void CtestView::OnInitialUpdate()
{
	// TODO: 在此添加专用代码和/或调用基类
	GetParentFrame()->MoveWindow(0, 0, 2000, 1280);
	m_pAnimatingCard = nullptr;
	m_nNextPlayer = 0;
	m_nAnimationTimerID1 = SetTimer(ANIMATION_TIMER_ID1, 16, NULL);
	CView::OnInitialUpdate();
}

void CtestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonUp(nFlags, point);
}

void CtestView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
}


void CtestView::DealNextCard(CtestDoc* pDoc)
{
	if (m_account == 54) {
		KillTimer(m_nAnimationTimerID1);
		m_pAnimatingCard = nullptr;
		Invalidate();
		return;
	}
	CRect clientRect;
	GetClientRect(&clientRect);
	POSITION pos;
	if (m_account == 51||m_account==52||m_account==53) {
		pos = pDoc->m_cardListAtTop.FindIndex((m_account-51) % 3);
		m_pAnimatingCard = (CCard*)pDoc->m_cardListAtTop.GetAt(pos);
		m_ptAnimationEnd.y = 0;
		m_ptAnimationEnd.x = (clientRect.Width() / 2) - (3 * m_cardWidth / 2) + ((m_account-51) % 3) * m_cardWidth;
	}
	else {
		switch (m_nNextPlayer)
		{
		case 0: // 左边玩家
			pos = pDoc->m_leftPlayerHand.FindIndex(m_account / 3);
			m_pAnimatingCard = (CCard*)pDoc->m_cardListHand.GetAt(pos);
			m_ptAnimationEnd.x = 30;
			m_ptAnimationEnd.y = 200 + (m_account / 3) * m_cardHeight / 4;
			break;
		case 1: // 右边玩家
			pos = pDoc->m_rightPlayerHand.FindIndex(m_account / 3);
			m_pAnimatingCard = (CCard*)pDoc->m_leftPlayerHand.GetAt(pos);
			m_ptAnimationEnd.x = clientRect.right - m_cardWidth - 100;
			m_ptAnimationEnd.y = 200 + (m_account / 3) * m_cardHeight / 4;
			break;
		case 2: // 玩家 (下)
			pos = pDoc->m_cardListHand.FindIndex(m_account / 3);
			m_pAnimatingCard = (CCard*)pDoc->m_rightPlayerHand.GetAt(pos);
			m_ptAnimationEnd.x = (clientRect.Width() / 2) - (17 * m_cardWidth / 2) + (m_account / 3) * m_cardWidth / 4;
			m_ptAnimationEnd.y = clientRect.bottom - m_cardHeight - 100;
			break;
		}
	}
	m_account++;
	m_ptAnimationStart = m_pAnimatingCard->m_rect.TopLeft();
	m_pAnimatingCard->m_rect.SetRect(m_ptAnimationEnd.x, m_ptAnimationEnd.y, m_ptAnimationEnd.x + m_cardWidth, m_ptAnimationEnd.y + m_cardHeight);
	m_nCurrentStep = 0;
	m_nAnimationSteps = 1; // 动画用1步完成
	m_nNextPlayer = (m_nNextPlayer + 1) % 3;
}

void CtestView::DrawList(CObList& obList,CDC* pmemDC)
{
	POSITION pos;
	pos = obList.GetHeadPosition();
	while (pos) {
		CCard* pCard = (CCard*)obList.GetNext(pos);
		pCard->Draw(pmemDC);
	}
}

void CtestView::ClearList(CObList& obList)
{
	POSITION pos = obList.GetHeadPosition();
	while (pos) { delete obList.GetNext(pos); }
}

void CtestView::ArrangeHand(CObList& hand, CRect clientRect, bool isMe)
{
	if (hand.IsEmpty()) {
		return;
	}
	int count = hand.GetCount();
	int totalWidth = (count - 1) * 40 + m_cardWidth; // 每张牌重叠一部分
	int startX = (clientRect.Width() - totalWidth) / 2;

	int startY;
	if (isMe) {
		// 玩家手牌在下方
		startY = clientRect.bottom - m_cardHeight - 20;
	}
	else {
		// 出掉的牌在中间靠下
		startY = clientRect.CenterPoint().y + 50;
	}

	POSITION pos = hand.GetHeadPosition();
	int i = 0;
	while (pos)
	{
		CCard* pCard = (CCard*)hand.GetNext(pos);
		pCard->m_rect.SetRect(startX + i * m_cardWidth/4, startY, startX + i * m_cardWidth/4 + m_cardWidth, startY + m_cardHeight);
		i++;
	}
}

int CtestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_wndPlayButton.Create(
		_T("出牌"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(10, 10, 100, 40),
		this,
		IDC_BUTTON_PLAY_CARDS
	);
	return 0;
}

void CtestView::OnPlayCards()
{
	CtestDoc* pDoc = GetDocument();
	POSITION pos = pDoc->m_playedCardList.GetHeadPosition();//清理所有打出的牌
	while (pos) {
		delete pDoc->m_playedCardList.GetNext(pos);
	}
	pDoc->m_playedCardList.RemoveAll();
	//将选中的牌移到打出的牌列表
	CRect clientRect;
	GetClientRect(&clientRect);
	CPoint centerPoint = clientRect.CenterPoint();
	int currentCardOffset = 0;
	
	pos = pDoc->m_cardListHand.GetHeadPosition();
	while (pos) {
		POSITION currentPos = pos;
		CCard* pCard = (CCard*)pDoc->m_cardListHand.GetNext(pos);
		if (pCard->m_bIsSelected) {
			pDoc->m_cardListHand.RemoveAt(currentPos);
			pDoc->m_playedCardList.AddTail(pCard);
			pCard->m_bIsSelected = FALSE;
			int cardWidth = pCard->m_rect.Width();
			int cardHeight = pCard->m_rect.Height();
			int startX = centerPoint.x + currentCardOffset;
			pCard->m_rect.SetRect(startX, centerPoint.y - (cardHeight / 2), startX + cardWidth, centerPoint.y + (cardHeight / 2));
			currentCardOffset += cardWidth/4;
		}
	}
	Invalidate();
}

void CtestView::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == m_nAnimationTimerID1) {
		CtestDoc* pDoc;
		pDoc = GetDocument();
		if (m_pAnimatingCard == nullptr) {//已经发完了一张牌
			DealNextCard(pDoc);
			return;
		}

		if (m_nCurrentStep <= m_nAnimationSteps)
		{
			int newX = m_ptAnimationStart.x + (m_ptAnimationEnd.x - m_ptAnimationStart.x) * m_nCurrentStep / m_nAnimationSteps;
			int newY = m_ptAnimationStart.y + (m_ptAnimationEnd.y - m_ptAnimationStart.y) * m_nCurrentStep / m_nAnimationSteps;
			m_pAnimatingCard->m_rect.MoveToXY(newX, newY);
			m_nCurrentStep++;
		}
		else {
			CtestDoc* pDoc = GetDocument();
			int playerIndex = (m_nNextPlayer + 2) % 3; // 计算刚才发牌的玩家
			if (playerIndex == 2||(m_account<54&&m_account>51)) {
				m_pAnimatingCard->m_isFaceUp = TRUE; // 只有玩家的牌和放在顶部是正面
			}
			m_pAnimatingCard = nullptr; // 清空动画牌指针，以便下一轮发牌
		}
		Invalidate(FALSE); // 重绘，FALSE表示不擦除背景，防止闪烁
	}
	CView::OnTimer(nIDEvent);
}

void CtestView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CView::OnDestroy();
	KillTimer(m_nAnimationTimerID1); // 确保计时器被销毁

	// 清理所有牌的内存
	auto clearList = [](CObList& list) {
		POSITION pos = list.GetHeadPosition();
		while (pos) { delete list.GetNext(pos); }
		list.RemoveAll();
		};
	CtestDoc* pDoc = GetDocument();
	clearList(pDoc->m_cardListHand);
	clearList(pDoc->m_leftPlayerHand);
	clearList(pDoc->m_rightPlayerHand);
	//if (m_pAnimatingCard) delete m_pAnimatingCard;
}

BOOL CtestView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

/*CMyDoc* CMyView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc)));
	return (CMyDoc*)m_pDocument;
}*/