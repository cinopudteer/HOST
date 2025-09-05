
// testDoc.cpp: CtestDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "test.h"
#endif

#include "testDoc.h"

#include <propkey.h>

#include<random>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CtestDoc

IMPLEMENT_DYNCREATE(CtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestDoc, CDocument)
END_MESSAGE_MAP()


// CtestDoc 构造/析构

CtestDoc::CtestDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	StrName = _T("This is a test!");
}

CtestDoc::~CtestDoc()
{
}

BOOL CtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	InitializeGame();
	return TRUE;
}

void CtestDoc::InitializeGame(){
	CCard::LoadBackImage();
	CImage masterImage;//包含所有扑克牌的那张大图
	if (FAILED(masterImage.Load(_T("D:\\lCardBig.bmp")))) {
		return;
	}
	if (FAILED(backGround.Load(_T("D:\\ReviewBK.bmp")))) {
		return;
	}
	int masterWidth = masterImage.GetWidth();
	int masterHeight = masterImage.GetHeight();
	const int CardWidth = masterWidth / 13;
	const int CardHeight = masterHeight / 5;


	std::vector<CCard*> tempDeck;//将每张牌的图像加载出来
	for (int suit = 0; suit <= 4; suit++) {
		for (int rank = 0; rank < 13; rank++) {
			CCard* pCard = nullptr;
			if (suit == 4) {
				if (rank == 0) {
					pCard = new CCard(SUIT_JOKER, RANK_SK);
				}
				else if(rank==1) {
					pCard = new CCard(SUIT_JOKER, RANK_BK);
				}
				else if (rank == 2) {
					pCard = new CCard(SUIT_BACK, RANK_BACK);
				}
				else {

				}
			}
			else {
				if (rank < 2) {
					pCard = new CCard(suit, rank + 11);//创建一个CCard对象，充当添加到tempDeck的运输车
				}
				else{
					pCard = new CCard(suit, rank -2);//创建一个CCard对象，充当添加到tempDeck的运输车
				}
			}
			if (pCard != nullptr) {//下面将包含所有扑克牌的大图切割，一个一个添加到tempDeck
				pCard->m_image.Create(CardWidth, CardHeight, masterImage.GetBPP());//设置参数宽度为 cardWidth，高度为 cardHeight。图像的位深度与原始图片保持一致。
				CDC* pCardDC = CDC::FromHandle(pCard->m_image.GetDC());//获取上下文
				masterImage.BitBlt(pCardDC->GetSafeHdc(), 0, 0, CardWidth, CardHeight, rank * CardWidth, suit * CardHeight);//复制图像
				pCard->m_image.ReleaseDC();
				CPoint centerPoint(1000, 640);
				pCard->m_rect.SetRect(centerPoint.x - CardWidth / 2, centerPoint.y - CardHeight / 2,centerPoint.x + CardWidth / 2, centerPoint.y + CardHeight / 2);//刚开始把牌放在屏幕中央
				tempDeck.push_back(pCard);
			}

		}
	}
	std::random_device rd;//洗牌
	std::mt19937 g(rd());
	std::shuffle(tempDeck.begin(), tempDeck.end()-1, g);
	int i = 0;
	for (int j = 0; j < 51;j++) {
		if (i == 0) {
			m_leftPlayerHand.AddTail(tempDeck[j]);
		}
		else if (i == 1) {
			m_rightPlayerHand.AddTail(tempDeck[j]);
		}
		else {
			m_cardListHand.AddTail(tempDeck[j]);
		}
		i ++;
		i %= 3;
	}
	m_cardListAtTop.AddTail(tempDeck[51]);
	m_cardListAtTop.AddTail(tempDeck[52]);
	m_cardListAtTop.AddTail(tempDeck[53]);
	ShuffleDeck(m_leftPlayerHand);
	ShuffleDeck(m_rightPlayerHand);
	ShuffleDeck(m_cardListHand);
}

void CtestDoc::ShuffleDeck(CObList& cardList)
{
	if (cardList.IsEmpty()) {
		return;
	}
	CArray<CCard*> sortArray;
	sortArray.SetSize(cardList.GetCount());
	int i=0;
	POSITION pos = cardList.GetHeadPosition();
	while (pos) {
		sortArray[i++] = (CCard*)cardList.GetNext(pos);
	}
	
	int n = sortArray.GetSize();
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (*sortArray[j] < *sortArray[j + 1]) {
				CCard* temp = sortArray[j];
				sortArray[j] = sortArray[j + 1];
				sortArray[j + 1] = temp;
			}
		}
	}
	
	cardList.RemoveAll();
	for (int i = 0; i < n; i++) {
		cardList.AddTail(sortArray[i]);
	}
}

void CtestDoc::CleanAllList()
{
	CleanOneList(m_playedCardList);
	CleanOneList(m_leftPlayerHand);
	CleanOneList(m_rightPlayerHand);
	CleanOneList(m_cardListHand);
}
void CtestDoc::CleanOneList(CObList& list) {
	POSITION pos = list.GetHeadPosition();
	while (pos) {
		delete list.GetNext(pos);
	}
	list.RemoveAll();
}

// CtestDoc 序列化

void CtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CtestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CtestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}



void CtestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CtestDoc 诊断

#ifdef _DEBUG
void CtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestDoc 命令

void CtestDoc::DeleteContents()
{
	// TODO: 在此添加专用代码和/或调用基类
	CleanAllList();
	CDocument::DeleteContents();
}
