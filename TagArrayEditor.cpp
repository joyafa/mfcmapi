// TagArrayEditor.cpp : implementation file
//

#include "stdafx.h"
#include "TagArrayEditor.h"
#include "InterpretProp.h"
#include "InterpretProp2.h"
#include "MAPIFunctions.h"
#include "PropertyTagEditor.h"

enum __TagArrayEditorTypes
{
	EDITOR_RTF,
	EDITOR_STREAM,
};

static TCHAR* CLASS = _T("CTagArrayEditor");

// Takes LPMAPIPROP and ulPropTag as input - will pull SPropValue from the LPMAPIPROP
CTagArrayEditor::CTagArrayEditor(
								 CWnd* pParentWnd,
								 UINT uidTitle,
								 UINT uidPrompt,
								 LPSPropTagArray lpTagArray,
								 BOOL bIsAB,
								 LPMAPIPROP	lpMAPIProp):
CEditor(pParentWnd,uidTitle,uidPrompt,0,CEDITOR_BUTTON_OK|CEDITOR_BUTTON_CANCEL)
{
	TRACE_CONSTRUCTOR(CLASS);

	m_lpTagArray = lpTagArray;
	m_bIsAB = bIsAB;
	m_lpOutputTagArray = NULL;
	m_lpMAPIProp = lpMAPIProp;
	if (m_lpMAPIProp) m_lpMAPIProp->AddRef();

	CreateControls(1);
	InitList(0,IDS_PROPTAGARRAY,false,false);
}

CTagArrayEditor::~CTagArrayEditor()
{
	TRACE_DESTRUCTOR(CLASS);
	MAPIFreeBuffer(m_lpOutputTagArray);
	if (m_lpMAPIProp) m_lpMAPIProp->Release();
}

// Used to call functions which need to be called AFTER controls are created
BOOL CTagArrayEditor::OnInitDialog()
{
	BOOL bRet = CEditor::OnInitDialog();

	ReadTagArrayToList(0);

	UpdateListButtons();
	return bRet;
}

void CTagArrayEditor::OnOK()
{
	WriteListToTagArray(0);
	CDialog::OnOK(); // don't need to call CEditor::OnOK
}

BOOL CTagArrayEditor::DoListEdit(ULONG ulListNum, int iItem, SortListData* lpData)
{
	if (!IsValidList(ulListNum)) return false;
	if (!lpData) return false;

	HRESULT	hRes = S_OK;
	ULONG	ulOrigPropTag = lpData->data.Tag.ulPropTag;
	ULONG	ulNewPropTag = ulOrigPropTag;

	CPropertyTagEditor MyPropertyTag(
			NULL, // title
			NULL, // prompt
			ulOrigPropTag,
			m_bIsAB,
			m_lpMAPIProp,
			this);

	WC_H(MyPropertyTag.DisplayDialog());
	if (S_OK != hRes) return false;
	ulNewPropTag = MyPropertyTag.GetPropertyTag();

	if (ulNewPropTag != ulOrigPropTag)
	{
		CString szTmp;
		lpData->data.Tag.ulPropTag = ulNewPropTag;

		szTmp.Format(_T("0x%08X"),ulNewPropTag); // STRING_OK
		SetListString(ulListNum,iItem,1,szTmp);

		CString PropTag;
		CString PropType;
		LPTSTR szExactMatch = NULL;
		LPTSTR szPartialMatch = NULL;
		LPTSTR szNamedPropName = NULL;
		LPTSTR szNamedPropGUID = NULL;

		InterpretProp(
			NULL,
			ulNewPropTag,
			NULL,
			NULL,
			NULL,
			m_bIsAB,
			&szExactMatch, // Built from ulPropTag & bIsAB
			&szPartialMatch, // Built from ulPropTag & bIsAB
			&PropType,
			&PropTag,
			NULL,
			NULL,
			&szNamedPropName, // Built from lpProp & lpMAPIProp
			&szNamedPropGUID, // Built from lpProp & lpMAPIProp
			NULL);
		SetListString(ulListNum,iItem,2,szExactMatch);
		SetListString(ulListNum,iItem,3,szPartialMatch);
		SetListString(ulListNum,iItem,4,PropType);
		SetListString(ulListNum,iItem,5,szNamedPropName);
		SetListString(ulListNum,iItem,6,szNamedPropGUID);

		delete[] szPartialMatch;
		delete[] szExactMatch;
		FreeNameIDStrings(szNamedPropName, szNamedPropGUID, NULL);

		return true;
	}
	return false;
} // CTagArrayEditor::DoListEdit

void CTagArrayEditor::ReadTagArrayToList(ULONG ulListNum)
{
	if (!IsValidList(ulListNum)) return;

	InsertColumn(ulListNum,0,IDS_SHARP);
	InsertColumn(ulListNum,1,IDS_TAG);
	InsertColumn(ulListNum,2,IDS_PROPERTYNAMES);
	InsertColumn(ulListNum,3,IDS_OTHERNAMES);
	InsertColumn(ulListNum,4,IDS_TYPE);
	InsertColumn(ulListNum,5,IDS_NAMEDPROPNAME);
	InsertColumn(ulListNum,6,IDS_NAMEDPROPGUID);

	if (m_lpTagArray)
	{
		CString szTmp;
		ULONG iTagCount = 0;
		ULONG cValues = m_lpTagArray->cValues;

		for (iTagCount = 0; iTagCount < cValues; iTagCount++)
		{
			ULONG ulPropTag = m_lpTagArray->aulPropTag[iTagCount];
			SortListData* lpData = NULL;
			szTmp.Format(_T("%d"),iTagCount); // STRING_OK
			lpData = InsertListRow(ulListNum,iTagCount,szTmp);
			if (lpData)
			{
				lpData->ulSortDataType = SORTLIST_TAGARRAY;
				lpData->data.Tag.ulPropTag = ulPropTag;
				lpData->bItemFullyLoaded = true;
			}

			CString PropTag;
			CString PropType;
			LPTSTR szExactMatch = NULL;
			LPTSTR szPartialMatch = NULL;
			LPTSTR szNamedPropName = NULL;
			LPTSTR szNamedPropGUID = NULL;

			InterpretProp(
				NULL,
				ulPropTag,
				m_lpMAPIProp,
				NULL,
				NULL,
				m_bIsAB,
				&szExactMatch, // Built from ulPropTag & bIsAB
				&szPartialMatch, // Built from ulPropTag & bIsAB
				&PropType,
				&PropTag,
				NULL,
				NULL,
				&szNamedPropName, // Built from lpProp & lpMAPIProp
				&szNamedPropGUID, // Built from lpProp & lpMAPIProp
				NULL);
			SetListString(ulListNum,iTagCount,1,PropTag);
			SetListString(ulListNum,iTagCount,2,szExactMatch);
			SetListString(ulListNum,iTagCount,3,szPartialMatch);
			SetListString(ulListNum,iTagCount,4,PropType);
			SetListString(ulListNum,iTagCount,5,szNamedPropName);
			SetListString(ulListNum,iTagCount,6,szNamedPropGUID);
			delete[] szPartialMatch;
			delete[] szExactMatch;
			FreeNameIDStrings(szNamedPropName, szNamedPropGUID, NULL);
		}
	}
	ResizeList(ulListNum,false);
} // CTagArrayEditor::ReadTagArrayToList

void CTagArrayEditor::WriteListToTagArray(ULONG ulListNum)
{
	if (!IsValidList(ulListNum)) return;

	// If we're not dirty, don't write
	if (!ListDirty(ulListNum)) return;

	HRESULT hRes = S_OK;
	ULONG ulListCount = GetListCount(ulListNum);
	EC_H(MAPIAllocateBuffer(
		CbNewSPropTagArray(ulListCount),
		(LPVOID*) &m_lpOutputTagArray));
	if (m_lpOutputTagArray)
	{
		m_lpOutputTagArray->cValues = ulListCount;

		ULONG iTagCount = 0;
		for (iTagCount = 0; iTagCount < m_lpOutputTagArray->cValues; iTagCount++)
		{
			SortListData* lpData = GetListRowData(ulListNum,iTagCount);
			if (lpData)
				m_lpOutputTagArray->aulPropTag[iTagCount] = lpData->data.Tag.ulPropTag;
		}

	}
} // CTagArrayEditor::WriteListToTagArray

LPSPropTagArray CTagArrayEditor::DetachModifiedTagArray()
{
	LPSPropTagArray lpRetArray = m_lpOutputTagArray;
	m_lpOutputTagArray = NULL;
	return lpRetArray;
} // CTagArrayEditor::DetachModifiedTagArray
