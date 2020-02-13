
// Phonebook.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Phonebook.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CitiesDocument.h"
#include "CitiesView.h"
#include "CitiesTable.h"
#include "DatabaseConnection.h"
#include "CitiesData.h"
#include "PersonsDocument.h"
#include "PersonsView.h"
#include "PhoneTypesDocument.h"
#include "PhoneTypesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhonebookApp

BEGIN_MESSAGE_MAP(CPhonebookApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPhonebookApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_MENU_PERSONS, &CPhonebookApp::OpenPersonsDocument)
	ON_COMMAND(ID_MENU_CITIES, &CPhonebookApp::OpenCitiesDocument)
	ON_COMMAND(ID_MENU_PHONE_TYPES, &CPhonebookApp::OpenPhoneTypesDocument)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CPhonebookApp construction

CPhonebookApp::CPhonebookApp() noexcept
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Phonebook.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CPhonebookApp::OpenPersonsDocument()
{
	CMultiDocTemplate* pDocPersonsTemplate;
	pDocPersonsTemplate = new CMultiDocTemplate(IDS_PERSONS,
		RUNTIME_CLASS(CPersonsDocument),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CPersonsView));

	if (!pDocPersonsTemplate)
		return;

	CDocument* pDocument = pDocPersonsTemplate->CreateNewDocument();
	pDocument->SetTitle(_T("Абонати"));
	pDocument->OnNewDocument();

	CFrameWnd* pFrame = pDocPersonsTemplate->CreateNewFrame(pDocument, NULL);
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);

	pDocPersonsTemplate->InitialUpdateFrame(pFrame, pDocument, TRUE);
	AddDocTemplate(pDocPersonsTemplate);
}

void CPhonebookApp::OpenCitiesDocument()
{
	CMultiDocTemplate* pDocCitiesTemplate;
	pDocCitiesTemplate = new CMultiDocTemplate(IDS_CITIES,
		RUNTIME_CLASS(CCitiesDocument),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCitiesView));

	if (!pDocCitiesTemplate)
		return;

	CDocument* pDocument = pDocCitiesTemplate->CreateNewDocument();
	pDocument->SetTitle(_T("Градове"));
	pDocument->OnNewDocument();
	CFrameWnd* pFrame = pDocCitiesTemplate->CreateNewFrame(pDocument, NULL);
	pDocCitiesTemplate->InitialUpdateFrame(pFrame, pDocument, TRUE);
	AddDocTemplate(pDocCitiesTemplate);
}

void CPhonebookApp::OpenPhoneTypesDocument()
{
	CMultiDocTemplate* pDocPhoneTypesTemplate;
	pDocPhoneTypesTemplate = new CMultiDocTemplate(IDS_PHONE_TYPES,
		RUNTIME_CLASS(CPhoneTypesDocument),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CPhoneTypesView));
	if (!pDocPhoneTypesTemplate)
		return;

	CDocument* pDocument = pDocPhoneTypesTemplate->CreateNewDocument();
	pDocument->SetTitle(_T("Телефонни типове"));
	pDocument->OnNewDocument();
	CFrameWnd* pFrame = pDocPhoneTypesTemplate->CreateNewFrame(pDocument, NULL);
	pDocPhoneTypesTemplate->InitialUpdateFrame(pFrame, pDocument, TRUE);
	AddDocTemplate(pDocPhoneTypesTemplate);
}

// The one and only CPhonebookApp object

CPhonebookApp theApp;

// CPhonebookApp initialization

BOOL CPhonebookApp::InitInstance()
{
	CoInitialize(NULL);

	CDatabaseConnection::GetInstance()->Connect();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	OpenPersonsDocument();

	return TRUE;
}

int CPhonebookApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CPhonebookApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CPhonebookApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CPhonebookApp message handlers



