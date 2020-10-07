#include "pch.h"
#include "Cage.h"

void CCage::Open()
{

	if (isMine_)
	{
		image_ = &ImageStatics::TriggeredMine;
	}
	else
	{
		image_ = &ImageStatics::Numbers[countOfNeighbouringMines_];
	}

	isOpen_ = true;
}

void CCage::ToggleFlag()
{
	isFlagInstalled_ = !isFlagInstalled_;
	image_ = isFlagInstalled_ ? &ImageStatics::Flag : &ImageStatics::Cell;
}

void CCage::Serialize(std::ostream& os) const
{
	os << countOfNeighbouringMines_ << ' ' << isOpen_ << ' ' << isMine_ << ' '
	   << isFlagInstalled_;
}

void CCage::Deserialize(std::istream& is)
{
	is >> countOfNeighbouringMines_ >> isOpen_
	   >> isMine_ >> isFlagInstalled_;

	if (isOpen_)
	{
		image_ = &ImageStatics::Numbers[countOfNeighbouringMines_];
	}
	else
	{
		image_ = isFlagInstalled_ ? &ImageStatics::Flag : &ImageStatics::Cell;
	}
}
