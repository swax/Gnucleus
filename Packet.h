#if !defined(AFX_PACK_H__A2D4CDEA_22B4_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PACK_H__A2D4CDEA_22B4_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma pack (push, 1)

union IP										// Size 4 	
{
	struct { u_char s_b1,s_b2,s_b3,s_b4; };		// S_un_b
	struct { u_short s_w1,s_w2; };				// S_un_w
	struct {BYTE a, b, c, d;};					// old_IP
	u_long S_addr;

	inline int operator > (union IP& first)
	{	
		bool result = false;

		if(a > first.a)
			result = true;
		else if(a == first.a)
		{
			if(b > first.b)
				result = true;
			else if(b == first.b)
			{
				if(c > first.c)
					result = true;
				else if(c == first.c)
				{
					if(d > first.d)
						result = true;
				}
			}
		}
	
		return result;
	};
};

struct ExIP
{
	int  a, b, c, d; // 0 - 255 is ip num, -1 is wildcard
	bool mode;       // false for deny, true for allow
};


/////////////////////////////////////////////////////////////////////////////
// Gnutella packets

struct packet_Header		// Size 23
{
	GUID  Guid;					// 0  - 15							
	BYTE  Function;				// 16
	BYTE  TTL;					// 17
	BYTE  Hops;					// 18							
	UINT  Payload;				// 19 - 22
};


struct packet_Ping			// Size 23
{
	packet_Header Header;		// 0  - 22	
};


struct packet_Pong			// Size 37
{
	packet_Header Header;		// 0  - 22							
	WORD Port;					// 23 - 24
	IP Host;					// 25 - 28
	UINT FileCount;			// 29 - 32
	UINT FileSize;				// 33 - 36
};


struct packet_Push			// Size 49
{
	packet_Header Header;		// 0  - 22;							
	GUID ServerID;				// 23 - 38
	UINT Index;				    // 39 - 42
	IP Host;					// 43 - 46
	WORD Port;					// 47 - 48
};


struct packet_Query			// Size 26+
{		
	packet_Header Header;		// 0  - 22						
	WORD Speed;					// 23 - 24
	// Search					// 25+
};


struct packet_QueryHit		// Size 35+
{
	packet_Header Header;		// 0  - 22
	byte TotalHits;				// 23
	WORD Port;					// 24 - 25
	IP   Host;					// 26 - 29
	UINT Speed;					// 30 - 33
	// QueryHitItems			// 34+
	
	// QueryHit Descriptor

	// ClientGuid				// Last 16 bytes
};

struct packet_QueryHitItem	// Size 9+
{
	UINT Index;					// 0  - 3
	UINT Size;					// 4  - 7
	
	// FileName					// 8+	
};

struct packet_QueryHitEx	    // Size 6+
{
	byte VendorID[4];			// 0  - 3
	byte Length;				// 4

	// Public Sector
	unsigned	Push		 : 1; // 5
	unsigned	FlagBad 	 : 1;
	unsigned	FlagBusy	 : 1;
	unsigned	FlagStable   : 1;
	unsigned	FlagSpeed	 : 1;
	unsigned    FlagTrash    : 3;

	unsigned    FlagPush	 : 1; // 6
	unsigned	Bad			 : 1;
	unsigned	Busy		 : 1;
	unsigned	Stable  	 : 1;
	unsigned	Speed		 : 1;
	unsigned    Trash		 : 3;

	// Private Sector

};

struct packet_RouteTableReset	// Size 29
{
	packet_Header Header;		// 0  - 22

	byte PacketType;			// 23
	UINT TableLength;			// 24 - 27
	byte Infinity;				// 28
};

struct packet_RouteTablePatch	// Size 29+
{
	packet_Header Header;		// 0  - 22

	byte PacketType;			// 23
	byte SeqNum;				// 24
	byte SeqSize;				// 25

	byte Compression;		    // 26
	byte EntryBits;				// 27

	// Patch Table...			// 28+
};

#pragma pack (pop)

#endif // !defined(AFX_PACK_H__A2D4CDEA_22B4_11D4_ACF2_00A0CC533D52__INCLUDED_)


