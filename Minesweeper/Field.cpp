#include "pch.h"
#include "Field.h"

#include <random>

#include "MinesweeperDlg.h"

CField::CField(const uint16_t& rows, const uint16_t& columns, const uint32_t& countOfMines, const uint16_t& cageSize, const CTimeSpan&& time)
	: columns_(columns)
	, rows_(rows)
	, countOfMines_(countOfMines)
	, cageSize_(cageSize)
	, countOfClosedCages_(rows * columns)
	, isGameOn_(true)
	, isMinesPlanted_(false)
	, playingTime_(time)
	, startTime_(CTime::GetCurrentTime())

{
	data_.assign(rows_, std::vector<CCage>(columns_, CCage()));
}

CField::CField(std::istream& is)
{
	Deserialize(is);
}

void CField::PlantMines(const uint16_t& startRow, const uint16_t& startColumn)
{
	//Plant mines
	std::default_random_engine randomEngine(static_cast<uint32_t>(CTime::GetCurrentTime().GetTime()));
	std::uniform_int_distribution<uint32_t> distribution(0, static_cast<uint32_t>(columns_) * rows_ - 1);
	minesData_.emplace(startRow * columns_ + startColumn);
	while (minesData_.size() != static_cast<size_t>(countOfMines_) + 1)
	{
		minesData_.emplace(distribution(randomEngine));
	}
	minesData_.erase(static_cast<uint32_t>(startRow) * columns_ + startColumn);

	for (const auto& index : minesData_)
	{
		data_[index / columns_][index % columns_] = CCage(true);
	}

	//Set numbers
	for (const auto& index : minesData_)
	{
		const int32_t row = index / columns_;
		const int32_t column = index % columns_;

		for (int32_t i = row - 1; i < row + 2; i++)
		{
			if (i == -1 || i == rows_)
			{
				continue;
			}

			for (int32_t j = column - 1; j < column + 2; j++)
			{
				if (j == -1 || j == columns_)
				{
					continue;
				}

				if (!data_[i][j].IsMine())
				{
					data_[i][j].IncreaseCountOfNearbyMines();
				}
			}
		}
	}

	isMinesPlanted_ = true;
}

void CField::OpenCell(std::queue<std::pair<uint16_t, uint16_t>>& needToOpen)
{
	const auto& [row, column] = needToOpen.front();
	CCage* cage = &data_[row][column];

	if (cage->IsOpen())
	{
		return;
	}

	cage->Open();
	countOfClosedCages_--;

	if (cage->IsMine())
	{
		for (const auto& index : minesData_)
		{
			data_[index / columns_][index % columns_].SetImage(ImageStatics::Mine);
		}
		cage->SetImage(ImageStatics::TriggeredMine);
		isGameOn_ = false;
	}
	else if (cage->GetCountOfNearbyMines() == 0)
	{
		for (int32_t i = row - 1; i < row + 2; i++)
		{
			if (i == -1 || i == rows_)
			{
				continue;
			}

			for (int32_t j = column - 1; j < column + 2; j++)
			{
				if (j == -1 || j == columns_)
				{
					continue;
				}

				needToOpen.push(std::make_pair(i, j));
			}
		}
	}
}

bool CField::CheckIsGameEnd() const
{
	if (countOfClosedCages_ != minesData_.size())
	{
		return false;
	}

	for (const auto& index : minesData_)
	{
		if (!data_[index / columns_][index % columns_].IsFlagInstalled())
		{
			return false;
		}
	}

	return true;
}

void CField::Serialize(std::ostream& os)
{
	//Settings
	os << rows_ << ' ' << columns_ << ' ' << countOfMines_ << ' ' << cageSize_ << ' ' << isGameOn_ << '\n';

	//Vector of field
	for (const auto& i : data_)
	{
		for (const auto& j : i)
		{
			j.Serialize(os);
			os << ' ';
		}
		os << '\n';
	}

	os << isMinesPlanted_ << '\n';

	//Mine indexes
	for (const auto& i : minesData_)
	{
		os << i << ' ';
	}
	os << '\n';

	playingTime_ += CTime::GetCurrentTime() - startTime_;

	os << ' ' << countOfClosedCages_ << ' '
	   << playingTime_.GetTotalSeconds();
}

void CField::Deserialize(std::istream& is)
{
	//Settings
	is >> rows_ >> columns_ >> countOfMines_ >> cageSize_ >> isGameOn_;

	//Vector of field
	data_.assign(rows_, std::vector<CCage>(columns_, CCage()));

	if (!isGameOn_)
	{
		isGameOn_ = true;
		isMinesPlanted_ = false;
		countOfClosedCages_ = rows_ * columns_;
		startTime_ = CTime::GetCurrentTime();
		playingTime_ = 0;
		return;
	}


	for (auto&& i : data_)
	{
		for (auto&& j : i)
		{
			j.Deserialize(is);
		}
	}

	is >> isMinesPlanted_;

	//Mine indexes
	for (uint32_t i = 0; i < countOfMines_ && isMinesPlanted_; i++)
	{
		uint32_t index;
		is >> index;
		minesData_.insert(index);
	}

	time_t playing;
	is >> countOfClosedCages_ >> playing;

	startTime_ = CTime::GetCurrentTime();
	playingTime_ = playing;
}