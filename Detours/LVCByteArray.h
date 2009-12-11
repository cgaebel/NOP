#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Generic Sizable Byte Array
/// 
/// @author	LanceVorgin
////////////////////////////////////////////////////////////////////////////////////////////////////
class LVCByteArray
{
public:
	inline LVCByteArray()
	{
		m_pbBuffer = NULL;
		m_uiSize = 0;
	}

	inline ~LVCByteArray()
	{
		if(m_pbBuffer)
			free(m_pbBuffer);
	}

	inline void Clear()
	{
		if(m_pbBuffer)
		{
			free(m_pbBuffer);
			m_pbBuffer = NULL;
		}

		m_uiSize = 0;
	}

	inline void CopyTo(BYTE* pbDest)
	{
		if(m_pbBuffer)
			memcpy(pbDest, m_pbBuffer, m_uiSize);
	}

	inline void CopyFrom(BYTE* pbSrc, unsigned int uiSize)
	{
		Clear();

		Add(pbSrc, uiSize);
	}

	inline BYTE* Copy()
	{
		if(m_pbBuffer == NULL)
			return NULL;

		BYTE* pbBuffer = (BYTE*)calloc(m_uiSize, sizeof(BYTE));

		CopyTo(pbBuffer);

		return pbBuffer;		
	}

	inline void Grow(unsigned int uiSize)
	{
		BYTE* pbNewBuffer = (BYTE*)realloc(m_pbBuffer, (m_uiSize + uiSize) * sizeof(BYTE));

		if(pbNewBuffer == NULL)
			throw std::exception("Out of memory!");

		m_pbBuffer = pbNewBuffer;

		m_uiSize += uiSize;
	}

	inline int SetEntry(int iIndex, void* pValue, unsigned int uiSize)
	{
		if((iIndex < 0) || ((iIndex + uiSize) > m_uiSize))
			return -1;

		memcpy(&m_pbBuffer[iIndex], pValue, uiSize);

		return iIndex;
	}

	inline int Add(void* pValue, unsigned int uiSize)
	{
		Grow(uiSize);

		return SetEntry(m_uiSize - uiSize, pValue, uiSize);
	}

	inline BYTE& operator[](int iIndex) const
	{
		return m_pbBuffer[iIndex];
	}

	inline BYTE* GetBuffer()
	{
		return m_pbBuffer;
	}

	inline unsigned int Size()
	{
		return m_uiSize;
	}

	inline int Peek()
	{
		return (int)m_uiSize;
	}

	inline int AddBYTE(BYTE bVal){			return Add(&bVal, 1);		}
	inline int AddWORD(WORD wVal){			return Add(&wVal, 2);		}
	inline int AddDWORD(DWORD dwVal){		return Add(&dwVal, 4);		}

	inline void operator+=(BYTE bVal){		AddBYTE(bVal);				}
	inline void operator+=(WORD wVal){		AddWORD(wVal);				}
	inline void operator+=(DWORD dwVal){	AddDWORD(dwVal);			}

	inline int operator+(BYTE bVal){		return AddBYTE(bVal);		}
	inline int operator+(WORD wVal){		return AddWORD(wVal);		}
	inline int operator+(DWORD dwVal){		return AddDWORD(dwVal);		}

	inline BYTE& BYTEAt(int iIndex){		return (BYTE&)m_pbBuffer[iIndex];	}
	inline WORD& WORDAt(int iIndex){		return (WORD&)m_pbBuffer[iIndex];	}
	inline DWORD& DWORDAt(int iIndex){		return (DWORD&)m_pbBuffer[iIndex];	}

private:
	BYTE* m_pbBuffer;

	unsigned int m_uiSize;
};
