// KeyboardSpriteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "KeyboardSpriteDlg.h"
#include "afxdialogex.h"


// CKeyboardSpriteDlg 对话框

IMPLEMENT_DYNAMIC(CKeyboardSpriteDlg, CDialogEx)

CKeyboardSpriteDlg::CKeyboardSpriteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyboardSpriteDlg::IDD, pParent)
{

}

CKeyboardSpriteDlg::~CKeyboardSpriteDlg()
{
}

void CKeyboardSpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeyboardSpriteDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyboardSpriteDlg 消息处理程序
