#pragma once

#include "constants.h"

class CCage
{
public:
	__forceinline CCage(const bool&& isMine = false) : isMine_(isMine) {}

	void Open();
	__forceinline constexpr bool IsOpen() const { return isOpen_; }

	void ToggleFlag();
	__forceinline constexpr bool IsFlagInstalled() const { return isFlagInstalled_; }

	__forceinline constexpr bool IsMine() const { return isMine_; }

	__forceinline void IncreaseCountOfNearbyMines() { countOfNeighbouringMines_++; }
	__forceinline constexpr int8_t GetCountOfNearbyMines() const { return countOfNeighbouringMines_; }

	__forceinline void SetImage(CImage& newImage) { image_ = &newImage; }
	__forceinline constexpr CImage* GetImage() const { return image_; }

	void Serialize(std::ostream& os) const;
	void Deserialize(std::istream& is);

private:
	int8_t countOfNeighbouringMines_ = 0;

	bool isOpen_ = false;
	bool isMine_;
	bool isFlagInstalled_ = false;

	CImage* image_ = &ImageStatics::Cell;
};