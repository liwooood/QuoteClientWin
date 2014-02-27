#ifndef BASE_WND_H
#define BASE_WND_H

#include "IRenderEngine.h"
#include "GDIRenderEngine.h"
#include "Direct2DRenderEngine.h"


#define WND_UNKNOWN -1
#define CONTAINER_VERTICAL 0
#define CONTAINER_HORIZONTAL 1
#define VIEW_KLINE 2
#define VIEW_TICK 3
#define VIEW_TABLE 4
#define VIEW_REALTIME 5
#define VIEW_DETAIL 6

#define WND_POSITION_FIRST 1
#define WND_POSITION_SECOND 2

class CBaseWnd : public CWnd
{
	DECLARE_DYNAMIC(CBaseWnd)

public:
	CBaseWnd();
	virtual ~CBaseWnd();

protected:
	DECLARE_MESSAGE_MAP()

	int m_nId;
	int m_nType;
	int m_nPosition; 
	float m_fWidth;
	float m_fHeight;

	// Õº–Œ‰÷»æ“˝«Ê
	IRenderEngine * m_pRender;

	CRect m_rcClient;

	CBaseWnd * m_pParent;
	int m_nParentId;
	
public:
	void SetId(int nId) {m_nId = nId;};
	int GetId() { return m_nId; };

	void SetPosition(int nPos) {m_nPosition = nPos;};
	int GetPosition() { return m_nPosition;};

	void SetType(int nType) {m_nType = nType;};
	int GetType() { return m_nType;};

	void SetWidth(float fWidth) {m_fWidth = fWidth;};
	float GetWidth() { return m_fWidth;};

	void SetHeight(float fHeight) {m_fHeight = fHeight;};
	float GetHeight() { return m_fHeight;};

	void SetParentWnd(CBaseWnd* pParent) {m_pParent = pParent;};
	CBaseWnd* GetParentWnd() {return m_pParent;};

	void SetParentWndId(int nParentId) {m_nParentId = nParentId;};
	int GetParentWndId() {return m_nParentId;};

	virtual void PrintWindowInfo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};


#endif
