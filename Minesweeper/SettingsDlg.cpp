// SettingsDlg.cpp : implementation file
//

#include "pch.h"
#include "Minesweeper.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
#include "MinesweeperDlg.h"


// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CField* field, CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, currentField_(field)
	, rows_(field->GetRows())
	, columns_(field->GetColumns())
	, countOfMines_(field->GetCountOfMines())
	, cageSize_(field->GetCageSize())
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROWS, rows_);
	DDX_Text(pDX, IDC_EDIT_COLUMNS, columns_);
	DDX_Text(pDX, IDC_EDIT_COUNTOFMINES, countOfMines_);
	DDX_Text(pDX, IDC_EDIT_CAGESIZE, cageSize_);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSettingsDlg::OnBnClickedButtonDefault)
	ON_EN_UPDATE(IDC_EDIT_COUNTOFMINES, &CSettingsDlg::OnEnUpdateEditCountOfmines)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CSettingsDlg::OnBnClickedButtonDefault()
{
	rows_ = 10;
	columns_ = 10;
	countOfMines_ = 25;
	cageSize_ = 30;
	UpdateData(FALSE);
}

void CSettingsDlg::OnEnUpdateEditCountOfmines()
{
	UpdateData(TRUE);
	uint32_t maxAllowedCountOfMines = static_cast<uint32_t>(static_cast<double>(rows_) * columns_ * 50 / 100);

	if (countOfMines_ > maxAllowedCountOfMines)
	{
		countOfMines_ = maxAllowedCountOfMines;
		UpdateData(FALSE);
		MessageBox(TEXT("New value over the limit"), TEXT("Error"), MB_ICONERROR | MB_OK);
	}
}

void CSettingsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CMinesweeperDlg* dlg = static_cast<CMinesweeperDlg*>(AfxGetMainWnd());

	if (rows_ != currentField_->GetRows()
		|| columns_ != currentField_->GetColumns()
		|| countOfMines_ != currentField_->GetCountOfMines())
	{
		if (MessageBox(TEXT("If you change these settings, current game will be lost.\nAre you sure?"), TEXT("Warning"), MB_ICONWARNING | MB_OKCANCEL) == IDOK)
		{
			dlg->CreateField(new CField(rows_, columns_, countOfMines_, cageSize_, CTimeSpan(0)));
		}
		else
		{
			return;
		}
	}
	else if (cageSize_ != currentField_->GetCageSize())
	{
		currentField_->SetCageSize(cageSize_);
		dlg->ResizeDialog();
		dlg->RedrawWindow();
	}

	CDialogEx::OnOK();
}
