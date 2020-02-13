#pragma once
enum UseAccessor
{
	AccessorZero = 0,
	AccessorOne = 1
};

enum DialogModes
{
	DialogModeModify = 0,
	DialogModeView = 1,
	DialogModeInsert = 2
};

enum ConstextMenuOptions {
	ID_MENU_MODIFY = 32772,
	ID_MENU_INSERT = 32771,
	ID_MENU_DELETE = 32773,
	ID_MENU_VIEW = 32776
};

enum MainfraimMenuOptions {
	ID_MENU_PERSONS = 32777,
	ID_MENU_CITIES = 32778,
	ID_MENU_PHONE_TYPES = 32779
};

enum UpdateViewHint {
	ViewModify = 0,
	ViewInsert = 1,
	ViewDelete = 2
};