#pragma once

// {CB971AC0-6408-40da-A540-92F9F256F51F}
static const IID IID_IUploadEvent =
{ 0xcb971ac0, 0x6408, 0x40da, { 0xa5, 0x40, 0x92, 0xf9, 0xf2, 0x56, 0xf5, 0x1f } };


class CGnucleusDoc;

class CAutUploadSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutUploadSink)

public:
	CAutUploadSink(CGnucleusDoc* pDoc);
	virtual ~CAutUploadSink();

	virtual void OnFinalRelease();

	void OnUpdate(LONG UploadID);

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


