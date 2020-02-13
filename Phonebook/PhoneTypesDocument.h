// PhoneTypesDocument.h : interface of the CPhoneTypesDocument class
//
#pragma once
#include "Structures.h"
#include "PhoneTypesData.h"
#include "PhoneTypesUpdateHint.h"
#include "QSortContext.h"
class CPhoneTypesDocument : public CDocument
{
protected: // create from serialization only
	CPhoneTypesDocument() noexcept;
	DECLARE_DYNCREATE(CPhoneTypesDocument)

	// Attributes
public:

	// Operations
public:

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll();

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на града </param>
	///<param name="recPhoneType"> Ред от таблица PHONE_TYPES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID(const long lID, PHONE_TYPES& recPhoneType);

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recPhoneType"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, const PHONE_TYPES& recPhoneType);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recPhoneType"> Запис за добавяне в таблица PHONE_TYPES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert(PHONE_TYPES& recPhoneType);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	///<returns> Връща масива от градове </returns>
	const CPhoneTypesArray& GetPhoneTypesArray() const;

	void SortArray( const CQSortContext& oQSortContext);

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	void SetTitle(LPCTSTR lpszTitle) override;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Members
	// ----------------
private:
	CPhoneTypesArray m_oPhoneTypesArray;

	// Implementation
public:
	virtual ~CPhoneTypesDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
