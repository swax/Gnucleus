#pragma once

// {70CA4938-6A0F-4641-A9A9-C936E4C1E7DE}
static const IID IID_IUpdateEvent =
{ 0x70ca4938, 0x6a0f, 0x4641, { 0xa9, 0xa9, 0xc9, 0x36, 0xe4, 0xc1, 0xe7, 0xde } };

class CGnucleusDoc;

class CAutUpdateSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutUpdateSink)

public:
	CAutUpdateSink(CGnucleusDoc* pDoc);
	virtual ~CAutUpdateSink();

	virtual void OnFinalRelease();

	void OnUpdateFound(LPCTSTR Version);
	void OnUpdateFailed(LPCTSTR Reason);
	void OnVersionCurrent();
	void OnUpdateComplete();

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


