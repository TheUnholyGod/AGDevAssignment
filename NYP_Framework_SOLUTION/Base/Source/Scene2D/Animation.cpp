#include "Animation.h"


CAnimation::CAnimation()
	: m_bAnimationInvert(false)
	, m_iAnimation_Index(0)
	, m_iRight_Start(0)
	, m_iRight_End(0)
	, m_iLeft_Start(0)
	, m_iLeft_End(0)
{
}


CAnimation::~CAnimation()
{
}

// Set Animation status; left or right
void CAnimation::SetAnimationStatus(bool m_bAnimationInvert)
{
	this->m_bAnimationInvert = m_bAnimationInvert;
	if (m_bAnimationInvert == false)
	{
		// If the player is facing right
		m_iAnimation_Index = m_iRight_Start;
	}
	else
	{
		// If the player is facing left
		m_iAnimation_Index = m_iLeft_Start;
	}
}
// Update the Animation Index
void CAnimation::UpdateAnimationIndex(void)
{
	if (m_bAnimationInvert == false)
	{
		// If the player is facing right
		m_iAnimation_Index += 1;
		if (m_iAnimation_Index >= m_iRight_End)
			m_iAnimation_Index = m_iRight_Start;
	}
	else
	{
		// If the player is facing left
		m_iAnimation_Index -= 1;
		if (m_iAnimation_Index <= m_iLeft_Start)
			m_iAnimation_Index = m_iLeft_End;
	}
}
// Get the Animation status
bool CAnimation::GetAnimationStatus(void) const
{
	return m_bAnimationInvert;
}
// Get the Animation index
int CAnimation::GetAnimationIndex(void) const
{
	return m_iAnimation_Index;
}

// Set right indices
void CAnimation::SetRightIndices(const int m_iRight_Start, const int m_iRight_End)
{
	this->m_iRight_Start = m_iRight_Start;
	this->m_iRight_End = m_iRight_End;
}

// Set left indices
void CAnimation::SetLeftIndices(const int m_iLeft_Start, const int m_iLeft_End)
{
	this->m_iLeft_Start = m_iLeft_Start;
	this->m_iLeft_End = m_iLeft_End;
}
