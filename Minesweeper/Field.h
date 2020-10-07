#pragma once

#include <unordered_set>
#include <vector>

#include <queue>

#include "constants.h"
#include "Cage.h"

class CField
{
public:
	CField(const uint16_t& rows, const uint16_t& columns, const uint32_t& countOfMines, const uint16_t& cageSize, const CTimeSpan&& time);
	CField(std::istream& is);

	void PlantMines(const uint16_t& startRow, const uint16_t& startColumn);

	void OpenCell(std::queue<std::pair<uint16_t, uint16_t>>& needToOpen);

	__forceinline constexpr uint16_t GetRows() const { return rows_; }
	__forceinline constexpr uint16_t GetColumns() const { return columns_; }
	__forceinline constexpr uint32_t GetCountOfMines() const { return countOfMines_; }

	//Cage size
	__forceinline void SetCageSize(const uint16_t& newCageSize) { cageSize_ = newCageSize; }
	__forceinline constexpr uint16_t GetCageSize() const { return cageSize_; }

	//@returns reference of field
	__forceinline auto& GetData() { return data_; }

	__forceinline constexpr bool IsMinesPlanted() const { return isMinesPlanted_; }

	__forceinline void SetGameOn(const bool&& newGameOn) { isGameOn_ = newGameOn; }
	__forceinline constexpr bool IsGameOn() const { return isGameOn_; }

	bool CheckIsGameEnd() const;

	__forceinline constexpr CTimeSpan& GetPlayingTime() { return playingTime_; }
	__forceinline constexpr CTime& GetStartTime() { return startTime_; }

	__forceinline void CalculatePlayingTime() { playingTime_ += CTime::GetCurrentTime() - startTime_; };

	void Serialize(std::ostream& os);
	void Deserialize(std::istream& is);

private:
	uint16_t rows_;
	uint16_t columns_;
	uint32_t countOfMines_;
	uint16_t cageSize_;

	bool isGameOn_;

	std::vector<std::vector<CCage>> data_;

	bool isMinesPlanted_;
	std::unordered_set<uint32_t> minesData_;

	uint32_t countOfClosedCages_;

	CTime startTime_;
	CTimeSpan playingTime_;
};