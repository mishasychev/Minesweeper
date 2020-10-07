#pragma once

#include <cstdint>

#include "Field.h"

// CSettingsDlg dialog

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CField* field, CWnd* pParent = nullptr);   // standard constructor

	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnEnUpdateEditCountOfmines();


	__forceinline constexpr uint32_t GetRows() const { return rows_; }
	__forceinline constexpr uint32_t GetColumns() const { return columns_; }
	__forceinline constexpr uint32_t GetCountOfMines() const { return countOfMines_; }

	__forceinline void SetCageSize(const uint32_t& newCageSize) { cageSize_ = newCageSize; }
	__forceinline constexpr uint32_t GetCageSize() const { return cageSize_; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	uint32_t rows_;
	uint32_t columns_;
	uint32_t countOfMines_;
	uint32_t cageSize_;

	CField* currentField_;
public:
	afx_msg void OnBnClickedOk();
};