#pragma once

#include <afxtempl.h>
#include <algorithm>

template<class TYPE>
class CTypedPtrArrayWithDeallocation : public CTypedPtrArray<CPtrArray, TYPE*>
{
	// Destructor
	// ----------------
public:
	virtual ~CTypedPtrArrayWithDeallocation()
	{
		DeleteArray();
	}

	// Methods
	// ----------------
public:
	///<summary> Изтрива масива </summary>
	void DeleteArray()
	{
		for (int i = 0; i < GetCount(); i++)
		{
			delete GetAt(i);
		}

		RemoveAll();
	};

	void DeleteElementAt( int nIndex ) 
	{
		if ( nIndex < 0 || nIndex >= GetCount() )
			return;

		delete GetAt( nIndex );
		RemoveAt( nIndex );
	}
};