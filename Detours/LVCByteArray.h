#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Generic Sizable Byte Array
/// 
/// @author	LanceVorgin
////////////////////////////////////////////////////////////////////////////////////////////////////
class LVCByteArray
{
private:
	BYTE* m_pbBuffer;
	unsigned int m_uiSize;

public:
	LVCByteArray();
	~LVCByteArray();

	void Clear();
	void CopyTo(BYTE* pbDest);
	void CopyFrom(BYTE* pbSrc, unsigned int uiSize);
	BYTE* Copy();
	void Grow(unsigned int uiSize);
	int SetEntry(int iIndex, void* pValue, unsigned int uiSize);
	int Add(void* pValue, unsigned int uiSize);
	BYTE& operator[](int iIndex) const;
	BYTE* GetBuffer();
	unsigned int Size();
	int Peek();

	inline int AddBYTE(BYTE bVal)		{	return Add(&bVal, 1);				}
	inline int AddWORD(WORD wVal)		{	return Add(&wVal, 2);				}
	inline int AddDWORD(DWORD dwVal)	{	return Add(&dwVal, 4);				}

	inline void operator+=(BYTE bVal)	{	AddBYTE(bVal);						}
	inline void operator+=(WORD wVal)	{	AddWORD(wVal);						}
	inline void operator+=(DWORD dwVal)	{	AddDWORD(dwVal);					}

	inline int operator+(BYTE bVal)		{	return AddBYTE(bVal);				}
	inline int operator+(WORD wVal)		{	return AddWORD(wVal);				}
	inline int operator+(DWORD dwVal)	{	return AddDWORD(dwVal);				}

	inline BYTE& BYTEAt(int iIndex)		{	return (BYTE&)m_pbBuffer[iIndex];	}
	inline WORD& WORDAt(int iIndex)		{	return (WORD&)m_pbBuffer[iIndex];	}
	inline DWORD& DWORDAt(int iIndex)	{	return (DWORD&)m_pbBuffer[iIndex];	}
};
