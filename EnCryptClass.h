#pragma once

#if !defined(AFX_CEnCryptClass_H__F753DF0D_1705_42E8_AAB7_69852C663F96__INCLUDED_)
#define AFX_CEnCryptClass_H__F753DF0D_1705_42E8_AAB7_69852C663F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Singleton.h"

class CEnCryptClass : public Singleton<CEnCryptClass>
{
public:
	explicit CEnCryptClass(void);
	virtual ~CEnCryptClass(void);

private:
	//�ǽð� XOR Encrypt Key Make
	size_t		m_nIdx_PM;				//packmemory idx
	char* m_pPMTarget;			//��,���� ���

	const int	m_nMAX_Cnt_Mark;		//max mark value
	char* m_pMark;				//mark key��

private:
	//��Ȱ������ �ΰ��� ����
	inline void PackMemory(char* pSource, const size_t nSourceSize)
	{
		for (m_nIdx_PM = 0; m_nIdx_PM < nSourceSize; ++m_nIdx_PM)
		{
			pSource[m_nIdx_PM] ^= m_pMark[m_nIdx_PM % m_nMAX_Cnt_Mark];
		}
	}
	inline void UnPackMemory(char* pSource, const size_t nSourceSize)
	{
		for (m_nIdx_PM = 0; m_nIdx_PM < nSourceSize; ++m_nIdx_PM)
		{
			pSource[m_nIdx_PM] ^= m_pMark[m_nIdx_PM % m_nMAX_Cnt_Mark];
		}
	}

public:
	template<typename T>
	const T GetValue(const T& Target)
	{
		//0. �� ����
		T Result = Target;

		//1. ��ȣȭ
		m_pPMTarget = (char*)&Result;
		UnPackMemory(m_pPMTarget, sizeof(Result));

		return Result;
	}
	template<typename T>
	void SetValue(T& Target, const T& Value)
	{
		//0.���ʿ� ���� ����.
		Target = Value;

		//1.����Ȱ� ��ȣȭ
		m_pPMTarget = (char*)&Target;
		PackMemory(m_pPMTarget, sizeof(Target));
	}
	template<typename T>
	void IncreaseValue(T& Target)
	{
		//0. ���� ����
		T Result = Target;

		//1. ��ȣȭ
		m_pPMTarget = (char*)&Result;
		UnPackMemory(m_pPMTarget, sizeof(Result));
		//2. ����
		++Result;

		//3. ��ȣȭ
		m_pPMTarget = (char*)&Result;
		PackMemory(m_pPMTarget, sizeof(Result));

		//4. ��ȣȭ�� �����͸� ����
		Target = Result;
	}
	template<typename T>
	void GetValueArray(T* Target, const T* Source, const size_t nSize)
	{
		_ASSERT(FALSE == IsBadReadPtr(Source, nSize));
		_ASSERT(FALSE == IsBadReadPtr(Target, nSize));

		//0. �� ����
		CopyMemory(Target, Source, nSize);

		//1. ��ȣȭ
		m_pPMTarget = (char*)&Target;
		UnPackMemory(m_pPMTarget, sizeof(Target));
	}
	template<typename T>
	void SetValueArray(T* Target, const T* Value, const size_t nSize)
	{
		_ASSERT(FALSE == IsBadReadPtr(Target, nSize));
		_ASSERT(FALSE == IsBadReadPtr(Value, nSize));

		//0.���ʿ� ���� ����.
		CopyMemory(Target, Value, nSize);

		//1.����Ȱ� ��ȣȭ
		m_pPMTarget = (char*)&Target;
		PackMemory(m_pPMTarget, sizeof(Target));
	}
	template<typename T>
	void InitValueArray(T* Target, const size_t nSize)
	{
		_ASSERT(FALSE == IsBadReadPtr(Target, nSize));

		//0.���ʿ� 0���� �ʱ�ȭ
		ZeroMemory(Target, nSize);

		//1.����Ȱ� ��ȣȭ
		m_pPMTarget = (char*)&Target;
		PackMemory(m_pPMTarget, sizeof(Target));
	}

};

#define g_pPack CEnCryptClass::Pointer()
#endif // !defined(AFX_CEnCryptClass_H__F753DF0D_1705_42E8_AAB7_69852C663F96__INCLUDED_)