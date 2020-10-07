	
// MinesweeperDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Minesweeper.h"
#include "MinesweeperDlg.h"
#include "afxdialogex.h"

#include <queue>
#include <fstream>

#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMinesweeperDlg dialog

CImage ImageStatics::Cell;
CImage ImageStatics::Flag;
CImage ImageStatics::Mine;
CImage ImageStatics::TriggeredMine;
CImage ImageStatics::Numbers[9];

CMinesweeperDlg::CMinesweeperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINESWEEPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CBitmap b;
	b.LoadBitmapW(IDB_BITMAP_MINE);
	


	std::ifstream inp("minesweeper.mcnf");
	field_ = new CField(inp);
	inp.close();
}

CMinesweeperDlg::~CMinesweeperDlg()
{
	std::ofstream out("minesweeper.mcnf", std::ios::out | std::ios::trunc);
	field_->Serialize(out);
	out.close();

	delete field_;
}

void CMinesweeperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMinesweeperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_NEWGAME, &CMinesweeperDlg::OnNewGame)
	ON_COMMAND(ID_SETTINGS, &CMinesweeperDlg::OnSettings)
END_MESSAGE_MAP()


// CMinesweeperDlg message handlers

BOOL CMinesweeperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	const HINSTANCE& hInstance = AfxGetInstanceHandle();

	ImageStatics::Cell.LoadFromResource(hInstance, IDB_BITMAP_CELL);
	ImageStatics::Flag.LoadFromResource(hInstance, IDB_BITMAP_IFLAG);

	ImageStatics::Mine.LoadFromResource(hInstance, IDB_BITMAP_MINE);
	ImageStatics::TriggeredMine.LoadFromResource(hInstance, IDB_BITMAP_TRIGGEREDMINE);

	ImageStatics::Numbers[0].LoadFromResource(hInstance, IDB_BITMAP_FREECELL);
	ImageStatics::Numbers[1].LoadFromResource(hInstance, IDB_BITMAP_NUMBER1);
	ImageStatics::Numbers[2].LoadFromResource(hInstance, IDB_BITMAP_NUMBER2);
	ImageStatics::Numbers[3].LoadFromResource(hInstance, IDB_BITMAP_NUMBER3);
	ImageStatics::Numbers[4].LoadFromResource(hInstance, IDB_BITMAP_NUMBER4);
	ImageStatics::Numbers[5].LoadFromResource(hInstance, IDB_BITMAP_NUMBER5);
	ImageStatics::Numbers[6].LoadFromResource(hInstance, IDB_BITMAP_NUMBER6);
	ImageStatics::Numbers[7].LoadFromResource(hInstance, IDB_BITMAP_NUMBER7);
	ImageStatics::Numbers[8].LoadFromResource(hInstance, IDB_BITMAP_NUMBER8);


	ResizeDialog();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMinesweeperDlg::ResizeDialog()
{
	CRect wndRect;
	GetDesktopWindow()->GetWindowRect(wndRect);

	int32_t fieldWidth = static_cast<int32_t>(field_->GetColumns() * field_->GetCageSize()) + 16;
	int32_t fieldHeight = static_cast<int32_t>(field_->GetRows() * field_->GetCageSize()) + 59;

	wndRect.left = (wndRect.right - fieldWidth) / 2;
	wndRect.top = (wndRect.bottom - fieldHeight) / 2;
	wndRect.right = wndRect.left + fieldWidth;
	wndRect.bottom = wndRect.top + fieldHeight;

	MoveWindow(wndRect);
}

void CMinesweeperDlg::RedrawCage(const int32_t& row, const int32_t& column)
{
	const CCage& cage		  = field_->GetData()[row][column];
	const int32_t cageSize    = static_cast<int32_t>(field_->GetCageSize());

	cage.GetImage()->Draw(*GetDC(), CRect(CPoint(column * cageSize, row * cageSize), CSize(cageSize, cageSize)));
}

void CMinesweeperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int32_t cxIcon = GetSystemMetrics(SM_CXICON);
		int32_t cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int32_t x = (rect.Width() - cxIcon + 1) / 2;
		int32_t y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		CDC* dc = GetDC();

		CDC dc_mem;
		dc_mem.CreateCompatibleDC(dc);

		CRect rectWindow;
		GetClientRect(&rectWindow);

		CRgn rgn;
		rgn.CreateRectRgn(0, 0, rectWindow.Width(), rectWindow.Height());
		dc->SelectClipRgn(&rgn);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(dc, rectWindow.Width(), rectWindow.Height());
		CBitmap* pOldBit = dc_mem.SelectObject(&bitmap);

		/*---------------------------------------------------------------------------------*/

		const auto& fieldData = field_->GetData();
		const int32_t cageSize = static_cast<int32_t>(field_->GetCageSize());

		for (int32_t i = 0; i < fieldData.size(); i++)
		{
			for (int32_t j = 0; j < fieldData.back().size(); j++)
			{
				const CCage& cage = fieldData[i][j];
				cage.GetImage()->Draw(dc_mem, CRect(CPoint(j * cageSize, i * cageSize), CSize(cageSize, cageSize)));
			}
		}

		/*---------------------------------------------------------------------------------*/

		dc_mem.SetStretchBltMode(HALFTONE);
		dc->BitBlt(0, 0, rectWindow.Width(), rectWindow.Height(), &dc_mem, 0, 0, SRCCOPY);


		dc->SelectClipRgn(nullptr);
		dc_mem.DeleteDC();
		bitmap.DeleteObject();
	}
}

void CMinesweeperDlg::CreateField(CField*&& newField)
{
	delete field_;
	field_ = newField;

	ResizeDialog();
	RedrawWindow();
}

void CMinesweeperDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!field_->IsGameOn())
	{
		return;
	}

	const uint16_t row    = static_cast<uint16_t>(point.y) / field_->GetCageSize();
	const uint16_t column = static_cast<uint16_t>(point.x) / field_->GetCageSize();
	const auto& fieldData = field_->GetData();

	if (const auto& cage = fieldData[row][column]; cage.IsOpen() || cage.IsFlagInstalled())
	{
		return;
	}

	if (!field_->IsMinesPlanted())
	{
		field_->PlantMines(row, column);
	}

	std::queue<std::pair<uint16_t, uint16_t>> needToOpen;
	needToOpen.push(std::make_pair(row, column));
	while (!needToOpen.empty())
	{
		field_->OpenCell(needToOpen);
		needToOpen.pop();
	}
	RedrawWindow();

	if (field_->CheckIsGameEnd())
	{
		field_->SetGameOn(false);
		field_->CalculatePlayingTime();
		MessageBox(field_->GetPlayingTime().Format("%H:%M:%S"), L"Win", MB_OK);
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMinesweeperDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!field_->IsGameOn())
	{
		return;
	}

	const uint16_t row    = static_cast<uint16_t>(point.y) / field_->GetCageSize();
	const uint16_t column = static_cast<uint16_t>(point.x) / field_->GetCageSize();
	CCage& cage = field_->GetData()[row][column];

	if (cage.IsOpen())
	{
		return;
	}

	cage.ToggleFlag();
	RedrawCage(row, column);

	if (field_->CheckIsGameEnd())
	{
		field_->SetGameOn(false);
		field_->CalculatePlayingTime();
		MessageBox(field_->GetPlayingTime().Format("Time: %H:%M:%S"), L"Win", MB_OK);
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CMinesweeperDlg::OnNewGame()
{
	CreateField(new CField(field_->GetRows(), field_->GetColumns(), field_->GetCountOfMines(), field_->GetCageSize(), CTimeSpan(0)));
}

void CMinesweeperDlg::OnSettings()
{
	CSettingsDlg settingsDialog(field_, this);
	settingsDialog.DoModal();
}