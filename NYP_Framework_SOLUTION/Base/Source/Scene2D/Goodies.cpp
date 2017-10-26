#include "Goodies.h"

CGoodies::CGoodies(void)
	: id(-1)
	, pos_column(0)
	, pos_row(0)
	, status(true)
{
}

CGoodies::~CGoodies(void)
{
}

// Get the id of this goodie
int CGoodies::GetID(void) const
{
	return id;
}

// Set the id of this goodie
void CGoodies::SetID(const int ID)
{
	this->id = ID;
}

// Get the pos column of this goodie
int CGoodies::GetPos_Column(void) const
{
	return pos_column;
}
// Get the pos row of this goodie
int CGoodies::GetPos_Row(void) const
{
	return pos_row;
}

// Set the pos of this goodie
void CGoodies::SetPos(const int pos_column, const int pos_row)
{
	this->pos_column = pos_column;
	this->pos_row = pos_row;
}



// Get the status of this goodie
bool CGoodies::GetStatus(void) const
{
	return status;
}
// Set the status of this goodie
void CGoodies::SetStatus(const bool status)
{
	this->status = status;
}
