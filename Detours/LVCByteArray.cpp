#include "LVCByteArray.h"

LVCByteArray::LVCByteArray()
{
	m_pbBuffer = NULL;
	m_uiSize = 0;
}

LVCByteArray::~LVCByteArray()
{
	if(m_pbBuffer)
		free(m_pbBuffer);
}

void LVCByteArray::Clear()
{
	if(m_pbBuffer)
	{
		free(m_pbBuffer);
		m_pbBuffer = NULL;
	}

	m_uiSize = 0;
}

void LVCByteArray::CopyTo(BYTE* pbDest)
{
	if(m_pbBuffer)
		memcpy(pbDest, m_pbBuffer, m_uiSize);
}

void LVCByteArray::CopyFrom(BYTE* pbSrc, unsigned int uiSize)
{
	Clear();

	Add(pbSrc, uiSize);
}

BYTE* LVCByteArray::Copy()
{
	if(m_pbBuffer == NULL)
		return NULL;

	BYTE* pbBuffer = (BYTE*)calloc(m_uiSize, sizeof(BYTE));

	CopyTo(pbBuffer);

	return pbBuffer;		
}

void LVCByteArray::Grow(unsigned int uiSize)
{
	auto pbNewBuffer = (BYTE*)realloc(m_pbBuffer, (m_uiSize + uiSize) * sizeof(BYTE));

	if(pbNewBuffer == NULL)
		throw std::exception("Out of memory!");

	m_pbBuffer = pbNewBuffer;

	m_uiSize += uiSize;
}

int LVCByteArray::SetEntry(int iIndex, void* pValue, unsigned int uiSize)
{
	if((iIndex < 0) || ((iIndex + uiSize) > m_uiSize))
		return -1;

	memcpy(&m_pbBuffer[iIndex], pValue, uiSize);

	return iIndex;
}

int LVCByteArray::Add(void* pValue, unsigned int uiSize)
{
	Grow(uiSize);

	return SetEntry(m_uiSize - uiSize, pValue, uiSize);
}

BYTE& LVCByteArray::operator[](int iIndex) const
{
	return m_pbBuffer[iIndex];
}

BYTE* LVCByteArray::GetBuffer()
{
	return m_pbBuffer;
}

unsigned int LVCByteArray::Size()
{
	return m_uiSize;
}

int LVCByteArray::Peek()
{
	return (int)m_uiSize;
}
