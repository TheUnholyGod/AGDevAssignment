#pragma once
class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; left or right
	void SetAnimationStatus(bool m_bAnimationInvert);
	// Update the Animation Index
	void UpdateAnimationIndex(void);
	// Get the Animation status
	bool GetAnimationStatus(void) const;
	// Get the Animation index
	int GetAnimationIndex(void) const;

	// Set right indices
	void SetRightIndices(const int m_iRight_Start, const int m_iRight_End);
	// Set left indices
	void SetLeftIndices(const int m_iLeft_Start, const int m_iLeft_End);

private:
	// Flag to indicate if the Animation is inverted to the left. 
	// true==face right, false==face left
	bool m_bAnimationInvert;

	// 0 == The default hero frame, 1/2/3 == Right facing animations, 
	// 4/5/6 == Left facing animations
	int m_iAnimation_Index;

	// Start index of right facing Animations
	int m_iRight_Start;
	// End index of right facing Animations
	int m_iRight_End;
	// Start index of left facing Animations
	int m_iLeft_Start;
	// End index of left facing Animations
	int m_iLeft_End;
};

