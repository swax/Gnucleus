#pragma once


// {284477E4-A7CB-4055-9E1B-0EA7CBA28945}
static const IID IID_IShareEvent =
{ 0x284477e4, 0xa7cb, 0x4055, { 0x9e, 0x1b, 0xe, 0xa7, 0xcb, 0xa2, 0x89, 0x45 } };


class CGnucleusDoc;

class CAutShareSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutShareSink)

public:
	CAutShareSink(CGnucleusDoc* pDoc);
	virtual ~CAutShareSink();

	virtual void OnFinalRelease();

	void OnUpdate(int FileID);
	void OnReload();

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


