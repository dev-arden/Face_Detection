
// MFCApplication_skin.h : MFCApplication_skin ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFCApplication_skinApp:
// �� Ŭ������ ������ ���ؼ��� MFCApplication_skin.cpp�� �����Ͻʽÿ�.
//

class CMFCApplication_skinApp : public CWinAppEx
{
public:
	CMFCApplication_skinApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication_skinApp theApp;
