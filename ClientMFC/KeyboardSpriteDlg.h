#pragma once


// CKeyboardSpriteDlg �Ի���

class CKeyboardSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyboardSpriteDlg)

public:
	CKeyboardSpriteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyboardSpriteDlg();

// �Ի�������
	enum { IDD = IDD_KEYBOARD_SPRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
