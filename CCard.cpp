#include "pch.h"
#include "CCard.h"

CCard::CCard(int suit,int rank):m_suit(suit),m_rank(rank) {
	m_bIsSelected = FALSE;
	m_isFaceUp = FALSE; // Ä¬ÈÏ±³Ãæ³¯ÉÏ
}
CCard::~CCard() {

}

CImage CCard::s_cardBackImage;

void CCard::Draw(CDC* pDC) {
	if (m_isFaceUp) {
		if (!m_image.IsNull()) {
			m_image.Draw(pDC->GetSafeHdc(), m_rect);
		}
	}
	else {
		if (!s_cardBackImage.IsNull())
			s_cardBackImage.Draw(pDC->GetSafeHdc(), m_rect);
	}
}
BOOL CCard::HitTest(CPoint point) {
	return m_rect.PtInRect(point);
}

void CCard::LoadBackImage()
{
	CImage tempImage;
	tempImage.Load(_T("D:\\lCardBig.bmp"));
	int width = tempImage.GetWidth()/13;
	int height = tempImage.GetHeight()/5;
	s_cardBackImage.Create(width, height, tempImage.GetBPP());
	CDC* pimageDC = CDC::FromHandle(s_cardBackImage.GetDC());
	tempImage.BitBlt(pimageDC->GetSafeHdc(), 0, 0, width, height, 2*width, 4*height);
	s_cardBackImage.ReleaseDC();
}

bool operator<(const CCard& pCard1, const CCard& pCard2)
{
	if (pCard1.m_rank < pCard2.m_rank) {
		return 1;
	}
	if (pCard1.m_rank == pCard2.m_rank) {
		if (pCard1.m_suit < pCard2.m_suit) {
			return 1;
		}
		else {
			return 0;
		}
	}
	return false;
}
