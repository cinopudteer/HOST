
// testDoc.h: CtestDoc 类的接口
//


#pragma once
#include "CCard.h"

class CtestDoc : public CDocument
{
protected: // 仅从序列化创建
	CtestDoc() noexcept;
	DECLARE_DYNCREATE(CtestDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CtestDoc();
	           //中间没有发牌的牌堆
	CObList m_cardListHand;     //自己的牌
	CObList m_leftPlayerHand;   // 左边玩家手牌
	CObList m_rightPlayerHand;  // 右边玩家手牌
	CObList m_playedCardList;   //已经打出去的牌
	CObList m_cardListAtTop;
	CImage backGround;//背景图
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void InitializeGame();      // 初始化游戏和牌局
	void ShuffleDeck(CObList&);         // 洗牌
	void CleanAllList(); // 用于释放内存
	void CleanOneList(CObList& list);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CString StrName;
	virtual void DeleteContents();
};
