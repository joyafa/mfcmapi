#pragma once
#include <UI/ViewPane/ViewPane.h>
#include <UI/Controls/SortList/SortListCtrl.h>
#include <functional>

namespace viewpane
{
	typedef std::function<bool(ULONG, int, controls::sortlistdata::SortListData*)> DoListEditCallback;

	class ListPane : public ViewPane
	{
	public:
		static ListPane* Create(UINT uidLabel, bool bAllowSort, bool bReadOnly, DoListEditCallback callback);
		static ListPane*
		CreateCollapsibleListPane(UINT uidLabel, bool bAllowSort, bool bReadOnly, DoListEditCallback callback);

		ULONG HandleChange(UINT nID) override;
		void SetListString(ULONG iListRow, ULONG iListCol, const std::wstring& szListString);
		_Check_return_ controls::sortlistdata::SortListData* InsertRow(int iRow, const std::wstring& szText) const;
		void ClearList();
		void ResizeList(bool bSort);
		_Check_return_ ULONG GetItemCount() const;
		_Check_return_ controls::sortlistdata::SortListData* GetItemData(int iRow) const;
		_Check_return_ controls::sortlistdata::SortListData* GetSelectedListRowData() const;
		void InsertColumn(int nCol, UINT uidText);
		void SetColumnType(int nCol, ULONG ulPropType) const;
		_Check_return_ bool OnEditListEntry();
		std::wstring GetItemText(_In_ int nItem, _In_ int nSubItem) const;
		bool IsDirty() override;

	private:
		ListPane();
		void Setup(bool bAllowSort, DoListEditCallback callback);

		void UpdateButtons() override;

		void Initialize(int iControl, _In_ CWnd* pParent, _In_ HDC hdc) override;
		void DeferWindowPos(_In_ HDWP hWinPosInfo, _In_ int x, _In_ int y, _In_ int width, _In_ int height) override;
		void CommitUIValues() override;
		int GetMinWidth(_In_ HDC hdc) override;
		int GetFixedHeight() override;
		int GetLines() override;

		void SwapListItems(ULONG ulFirstItem, ULONG ulSecondItem);
		void OnMoveListEntryUp();
		void OnMoveListEntryDown();
		void OnMoveListEntryToTop();
		void OnMoveListEntryToBottom();
		void OnAddListEntry();
		void OnDeleteListEntry(bool bDoDirty);

		bool m_bDirty;
		bool m_bAllowSort;

		DoListEditCallback m_callback;
		controls::sortlistctrl::CSortListCtrl m_List;
		static const int LINES_LIST = 6;
		static const int NUMLISTBUTTONS = 7;
		CButton m_ButtonArray[NUMLISTBUTTONS];
		const UINT ListButtons[NUMLISTBUTTONS] = {
			IDD_LISTMOVEDOWN,
			IDD_LISTMOVETOBOTTOM,
			IDD_LISTADD,
			IDD_LISTEDIT,
			IDD_LISTDELETE,
			IDD_LISTMOVETOTOP,
			IDD_LISTMOVEUP,
		};

		int m_iButtonWidth;
	};
}