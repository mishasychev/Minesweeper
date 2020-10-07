
// MinesweeperDlg.h : header file
//

#pragma once


#include "Field.h"
#include "constants.h"

// CMinesweeperDlg dialog
class CMinesweeperDlg : public CDialogEx
{
// Construction
public:
	CMinesweeperDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CMinesweeperDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINESWEEPER_DIALOG };
#endif

	void CreateField(CField*&& field);

	void ResizeDialog();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnNewGame();
	afx_msg void OnSettings();

	void RedrawCage(const int32_t& row, const int32_t& column);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg __forceinline constexpr BOOL OnEraseBkgnd(CDC* pDC) { return TRUE; }
	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	afx_msg __forceinline constexpr HCURSOR OnQueryDragIcon() { return m_hIcon; }
	DECLARE_MESSAGE_MAP()

private:
	CField* field_;
};
