#pragma once

// {E2032EC2-A9AC-4ed7-9BDB-EBECACF076F2}
static const IID IID_ISearchEvent =
{ 0xe2032ec2, 0xa9ac, 0x4ed7, { 0x9b, 0xdb, 0xeb, 0xec, 0xac, 0xf0, 0x76, 0xf2 } };


class CGnucleusDoc;

class CAutSearchSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutSearchSink)

public:
	CAutSearchSink(CGnucleusDoc* pDoc);
	virtual ~CAutSearchSink();

	virtual void OnFinalRelease();

	void OnUpdate(int SearchID, int ResultID);
	void OnResult(int SearchID, int ResultID);
	void OnRefresh(int SearchID);
	void OnBrowseUpdate(int SearchID, int State, int Progress);

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


