#pragma once

#include <afxtempl.h>

template<class KeyType, class ValueType>
class CMapWithDeallocation : public CMap< KeyType, KeyType, ValueType*, ValueType* >
{
	// Destructor
	// ----------------
public:
	virtual ~CMapWithDeallocation()
	{
		DeleteMap();
	}

	// Methods
	// ----------------
public:
	///<summary> Изтрива мапа </summary>
	void DeleteMap()
	{
		POSITION pos = GetStartPosition();
		while (pos != NULL)
		{
			long lKey = 0;
			ValueType* pValue = nullptr;
			GetNextAssoc(pos, lKey, pValue);

			delete pValue;
		}
	};
};