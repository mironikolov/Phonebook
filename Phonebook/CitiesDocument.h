// CitiesDocument.h : interface of the CCitiesDocument class
//
#pragma once
#include "Structures.h"
#include "CitiesData.h"
#include "QSortContext.h"

class CCitiesDocument : public CDocument
{
protected: // create from serialization only
	CCitiesDocument() noexcept;
	DECLARE_DYNCREATE(CCitiesDocument)

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
	///<param name="recCity"> Ред от таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID( const long lID, CITIES& recCity );

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recCity"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID( const long lID, const CITIES& recCity);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recCity"> Запис за добавяне в таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert( CITIES& recCity );

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID( const long lID );

	///<returns> Връща масива от градове </returns>
	const CCitiesArray& GetCitiesArray() const;

	void SortArray(const CQSortContext& oQSortContext);

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
	CCitiesArray m_oCitiesArray;
	CPersonsArray m_oPersonsArray;

// Implementation
public:
	virtual ~CCitiesDocument();
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
