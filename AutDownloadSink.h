#pragma once

// {0BE385A3-85A5-4722-B677-68DAE891FF21}
static const IID IID_IDownloadEvent =
{ 0xbe385a3, 0x85a5, 0x4722, { 0xb6, 0x77, 0x68, 0xda, 0xe8, 0x91, 0xff, 0x21 } };


class CGnucleusDoc;

class CAutDownloadSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutDownloadSink)

public:
	CAutDownloadSink(CGnucleusDoc* pDoc);
	virtual ~CAutDownloadSink();

	virtual void OnFinalRelease();

	void OnUpdate(LONG DownloadID);

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


