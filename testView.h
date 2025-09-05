
// testView.h: CtestView 类的接口
//

#pragma once
#include "CCard.h"
#include <vector>

#define ANIMATION_TIMER_ID1 1
#define ANIMATION_TIMER_ID2 2
#define CARDS_TO_DEAL 17 // 每人发17张牌

class CtestView : public CView
{
protected: // 仅从序列化创建
	CtestView() noexcept;
	DECLARE_DYNCREATE(CtestView)

// 特性
public:
	CtestDoc* GetDocument() const;

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
	virtual ~CtestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CImage m_image;// 用于加载和管理图片的对象 (CImage 支持 JPG, PNG, BMP 等)
 // 标志位，判断当前是否正在拖动
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	
	CButton m_wndPlayButton;    //出牌按钮

	UINT_PTR m_nAnimationTimerID1;//动画计时器ID,用于发牌
	CCard*   m_pAnimatingCard;     //当前移动的牌
	CPoint   m_ptAnimationStart; //移动起始点
	CPoint   m_ptAnimationEnd;   //移动终点
	CPoint   m_centerPoint;      //屏幕正中心的左边
	int      m_nAnimationSteps;  //移动总步数
	int      m_nCurrentStep;     // 当前是第几步
	int      m_nNextPlayer;      // 下一张牌发给谁 (0=左, 1=右, 2=下)
	int      m_cardWidth, m_cardHeight;  //卡片的长和宽
	int m_account = 0;

	void DealNextCard(CtestDoc*);        // 设置并开始下一张牌的动画,index表明发牌给谁,account表示这发的是第几张牌
	void DrawList(CObList&,CDC*);
	void ClearList(CObList&);
	void ArrangeHand(CObList& hand, CRect clientRect, bool isMe);
	void ArrangeSideHand(CObList& hand, CRect clientRect, bool isLeft);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//玩斗地主时处理窗口创建事件
	afx_msg void OnPlayCards();//处理出牌按钮点击事件
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // testView.cpp 中的调试版本
inline CtestDoc* CtestView::GetDocument() const
   { return reinterpret_cast<CtestDoc*>(m_pDocument); }
#endif

