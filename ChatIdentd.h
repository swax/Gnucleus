#if !defined(AFX_CHATIDENTD_H__18331C82_2611_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_CHATIDENTD_H__18331C82_2611_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChatControl;


class CChatIdentd : public CAsyncSocket
{
public:
	CChatIdentd(CChatControl*);
	virtual ~CChatIdentd();

	int m_SecsAlive;

	//{{AFX_VIRTUAL(CChatIdentd)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnSend(int nErrorCode);
	virtual void Close();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatIdentd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


protected:
	CChatControl* m_pChat;

	bool m_DataSending;

	byte m_pBuff[6000];
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATIDENTD_H__18331C82_2611_11D5_ACF2_00A0CC533D52__INCLUDED_)
