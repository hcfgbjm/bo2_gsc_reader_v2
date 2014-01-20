#pragma 

__inline BYTE* GET_ALIGNED_WORD		(BYTE* x) { return (BYTE*)(((DWORD)x + 1) & 0xFFFFFFFE); }
__inline BYTE* GET_ALIGNED_DWORD	(BYTE* x) { return (BYTE*)(((DWORD)x + 3) & 0xFFFFFFFC); }

int GetFormattedLength(char *format, ...);
char* MallocAndSprintf(char* format, ...);

class Crc32
{
public:
	const static DWORD kCrc32Table[];

	Crc32()
	{
		Reset();
	}

	~Crc32() throw()
	{
	}

	void Reset()
	{
		_crc = (DWORD)~0;
	}

	void AddData(const BYTE* pData, const DWORD length)
	{
		BYTE* pCur = (BYTE*)pData;
		DWORD remaining = length;
		for (; remaining--; ++pCur)
			_crc = ( _crc >> 8 ) ^ kCrc32Table[(_crc ^ *pCur) & 0xff];
	}

	const DWORD GetCrc32()
	{
		return ~_crc;
	}

private:
	DWORD _crc;
};