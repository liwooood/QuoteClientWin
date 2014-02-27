#pragma once


// CKeyboardSpriteDlg 对话框

class CKeyboardSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyboardSpriteDlg)

public:
	CKeyboardSpriteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyboardSpriteDlg();

// 对话框数据
	enum { IDD = IDD_KEYBOARD_SPRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
