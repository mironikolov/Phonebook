// PersonsDocument.h : interface of the CPersonsDocument class
//
#pragma once
#include "Structures.h"
#include "PersonsData.h"
#include "QSortContext.h"

class CPersonsDocument : public CDocument
{
protected: // create from serialization only
	CPersonsDocument() noexcept;
	DECLARE_DYNCREATE(CPersonsDocument)

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
	///<param name="lID"> ID на човека </param>
	///<param name="oPersonWithNumbers"> Човек и неговите номера </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID( const long lID, CPersonWithNumbers& oPersonWithNumbers );

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recPerson"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, CPersonWithNumbers& oPersonWithNumbers);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recCities"> Запис за добавяне в таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert( CPersonWithNumbers& oPersonWithNumbers );

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	///<returns> Връща масива от хора </returns>
	const CPersonsArray& GetPersonsArray() const;

	///<returns> Връща мап от градове </returns>
	const CCitiesMap& GetCitiesMap() const;

	///<returns> Връща мап от телефонни типове</returns>
	const CPhoneTypesMap& GetPhoneTypesMap() const;

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
	CPersonsArray m_oPersonsArray;
	CCitiesMap m_oCitiesMap;
	CPhoneTypesMap m_oPhoneTypesMap;

	// Implementation
public:
	virtual ~CPersonsDocument();
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

