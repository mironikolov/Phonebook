#pragma once

#include <afxtempl.h>
#include "TypedPtrArrayWithDeallocation.h"
#include "MapWithDeallocation.h"

#define CITIES_NAME_SIZE 64
#define CITIES_DISTRICT_SIZE 64
#define PHONE_TYPES_TYPE_SIZE 16
#define PERSONS_NAME_SIZE 64
#define PERSONS_SURNAME_SIZE 64
#define PERSONS_LASTNAME_SIZE 64
#define PERSONS_UCN_SIZE 16
#define PERSONS_ADDRESS_SIZE 256
#define PHONE_NUMBERS_NUMBER_SIZE 16

///<summary> Структура на таблицата CITIES</summary>
struct CITIES
{
	///<summary> Уникален идентификатор за ред </summary>
	long lID;

	///<summary> Versioning </summary>
	long lUpdateCounter;

	///<summary> Име на град </summary>
	TCHAR szName[CITIES_NAME_SIZE + 1];

	///<summary> Име на област </summary>
	TCHAR szDistrict[CITIES_DISTRICT_SIZE + 1];

	CITIES() 
	{
		SecureZeroMemory(this, sizeof(*this));
	}
	CITIES( const long lID )
	{
		SecureZeroMemory(this, sizeof(*this));
		this->lID = lID;
	}
};

///<summary> Структура на таблицата PHONE_TYPES </summary>
struct PHONE_TYPES
{
	///<summary> Уникален идентификатор за ред </summary>
	long lID;

	///<summary> Versioning </summary>
	long lUpdateCounter;

	///<summary> Тип на телефон </summary>
	TCHAR szType[PHONE_NUMBERS_NUMBER_SIZE + 1];

	PHONE_TYPES() 
	{
		SecureZeroMemory(this, sizeof(*this));
	}
	PHONE_TYPES( const long lID )
	{
		SecureZeroMemory(this, sizeof(*this));
		this->lID = lID;
	}
};

///<summary> Структура на таблицата PERSONS </summary>
struct PERSONS
{
	///<summary>Уникален идентификатор за ред </summary>
	long lID;

	///<summary>Versioning </summary>
	long lUpdateCounter;

	///<summary> Име на клиент </summary>
	TCHAR szName[PERSONS_NAME_SIZE + 1];

	///<summary> Презиме на клиент </summary>
	TCHAR szSurname[PERSONS_SURNAME_SIZE + 1];

	///<summary> Фамилия на клиент </summary>
	TCHAR szLastName[PERSONS_LASTNAME_SIZE + 1];

	///<summary> ЕГН на клиент </summary>
	TCHAR szUCN[PERSONS_UCN_SIZE + 1];

	///<summary> ID на града на клиент </summary>
	long lCityID;

	///<summary> Адрес на клиент </summary>
	TCHAR szAddress[PERSONS_ADDRESS_SIZE + 1];

	PERSONS() 
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

///<summary> Структура на таблицата PHONE_NUMBERS </summary>
struct PHONE_NUMBERS
{
	///<summary> Уникален идентификатор за ред </summary>
	long lID;
	///<summary> Versioning </summary>
	long lUpdateCounter;
	///<summary> ID на клиент с номер</summary>
	long lPersonID;
	///<summary> ID на тип на номер</summary>
	long lPhoneTypeID;
	///<summary> Номер</summary>
	TCHAR szNumber[PHONE_NUMBERS_NUMBER_SIZE + 1];

	PHONE_NUMBERS() 
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

///<summary> typedef-ове на CTypedPtrArray за таблиците </summary>
typedef CTypedPtrArrayWithDeallocation<CITIES> CCitiesArray;
typedef CTypedPtrArrayWithDeallocation<PHONE_TYPES> CPhoneTypesArray;
typedef CTypedPtrArrayWithDeallocation<PERSONS> CPersonsArray;
typedef CTypedPtrArrayWithDeallocation<PHONE_NUMBERS> CPhoneNumbersArray;
typedef CMapWithDeallocation< long, PHONE_TYPES> CPhoneTypesMap;
typedef CMapWithDeallocation< long, CITIES > CCitiesMap;
typedef CMapWithDeallocation< long, PHONE_NUMBERS > CPhoneNumbersMap;
