#pragma once
#include <UI/ViewPane/ViewPane.h>

namespace viewpane
{
	class CheckPane : public ViewPane
	{
	public:
		CheckPane() : m_bCheckValue(false), m_bCommitted(false) {}

		static CheckPane* Create(UINT uidLabel, bool bVal, bool bReadOnly);
		bool GetCheck() const;
		static void Draw(_In_ HWND hWnd, _In_ HDC hDC, _In_ const RECT& rc, UINT itemState);

	private:
		void Initialize(int iControl, _In_ CWnd* pParent, _In_ HDC hdc) override;
		void DeferWindowPos(_In_ HDWP hWinPosInfo, _In_ int x, _In_ int y, _In_ int width, _In_ int height) override;
		void CommitUIValues() override;
		int GetMinWidth(_In_ HDC hdc) override;
		int GetFixedHeight() override;

		CButton m_Check;
		bool m_bCheckValue;
		bool m_bCommitted;
	};
} // namespace viewpane