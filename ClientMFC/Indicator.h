#ifndef INDICATOR_H
#define INDICATOR_H

#include <vector>

#include "IRenderEngine.h"

class CIndicator
{
public:
	CIndicator(HWND hWnd);
	~CIndicator(void);

public:
	

	// ָ�괰������
	CRect m_rcArea;

	// �����Ļ�ͼ����
	CRect m_rcMemDC;

	// ָ���������
	CRect m_rcCaption;
	int m_nCaptionHeight;


	// �ұ߼۸������Ŀ��
	int m_nRightAxisWidth;

	// ָ���ͼ����
	CRect m_rcDraw;

	// �������ͣ�ֻ����ͼ��Ч
	int m_nLineType;

	// ָ������
	int m_nIndicatorType;

	// ���ھ��
	HWND m_hWnd;

	IRenderEngine* m_pRender;

	//�۸�����
	float m_fMaxPrice;
	float m_fMinPrice;
	long m_lMaxVolume;

	// �����
	double m_dVolume;

	// k��
	int m_nKLineWidth;

	// ����ͼ��֮��ķָ����
	int m_nSepWidth;

	// �ұ����ռ�������
	int m_nMarginCount;
	int m_nMarginWidth; // m_nMarginWidth = m_nKLineWidth + m_nSepWidth

	// ��Ϣ�״�߶�
	int m_nInfoRadarHeight;

	// �ײ�ȨϢ���ϸ߶�
	int m_nWeightHeight;
	
	// ���ݿ�ȣ��Լ�ȥ���ұ����գ����Ի�����k��
	int m_nDrawKLineCount;

	// �и�
	int m_nRowHeight;
	int m_nMinRows;

	float m_fScale;

public:
	//void SetWnd(HWND hWnd){m_hWnd = hWnd;}

	void InitWnd();
	void Render(std::vector<struct kline_struct>* pvData);

	void SetArea(CRect rcArea);

private:
	void KLine();
	void Vol();
	void MACD();

	void Compute();

	std::vector<struct kline_struct>* m_pvData;
	
};

#endif
