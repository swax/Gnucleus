#pragma once

// {A916AF3C-976D-4358-8736-95BEA0B5FD2C}
static const IID IID_IChatEvent =
{ 0xA916AF3C, 0x976D, 0x4358, { 0x87, 0x36, 0x95, 0xBE, 0xA0, 0xB5, 0xFD, 0x2C } };


class CGnucleusDoc;

class CAutChatSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutChatSink)

public:
	CAutChatSink(CGnucleusDoc* pDoc);
	virtual ~CAutChatSink();

	virtual void OnFinalRelease();

	void OnRecvPrivateMessage(LPCTSTR Address, LPCTSTR Message);

	CGnucleusDoc* m_pDoc;


protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


