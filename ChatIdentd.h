#pragma once

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
