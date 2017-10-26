#pragma once

class CGoodies
{
public:
	CGoodies(void);
	~CGoodies(void);
protected:
	// id of this goodie
	int id;
	// Position of this goodie in Columns and Rows
	int pos_column, pos_row;
	// Status of this goodie
	bool status;
public:
	// Get the id of this goodie
	int GetID(void) const;
	// Set the id of this goodie
	void SetID(const int ID);

	// Get the pos column of this goodie
	int GetPos_Column(void) const;
	// Get the pos row of this goodie
	int GetPos_Row(void) const;
	// Set the pos of this goodie
	void SetPos(const int pos_column, const int pos_row);

	// Get the status of this goodie
	bool GetStatus(void) const;
	// Set the status of this goodie
	void SetStatus(const bool status);
};
