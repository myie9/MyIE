// SortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct CCompareListStruct
{
	CSortListCtrl* m_pList;
	int m_nSortCol;
	BOOL m_bSortAsc;
};

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
	m_bLastSortAsc = FALSE;
	m_nLastSortCol = 0 ;
}

CSortListCtrl::~CSortListCtrl()
{
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

void CSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWaitCursor wait;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;

	if( m_nLastSortCol == pNMListView->iSubItem )
	{
		m_bLastSortAsc = !m_bLastSortAsc;
	}
	else
	{
		m_nLastSortCol = pNMListView->iSubItem;
		m_bLastSortAsc = TRUE;
	}

	CCompareListStruct s;
	
	s.m_pList = this;
	s.m_nSortCol = m_nLastSortCol;
	s.m_bSortAsc = m_bLastSortAsc;
		
	SortItems( (PFNLVCOMPARE) CompareEntries, (LPARAM) &s );
	
	*pResult = 0;
}

int CALLBACK CSortListCtrl::CompareEntries( LPARAM lp1, LPARAM lp2, LPARAM lData )
{
	CCompareListStruct* ps = (CCompareListStruct*) lData;
	ASSERT( NULL != ps );

	int nResult = 0;

	CString s1 = ps->m_pList->GetItemText( ps->m_pList->FindIndex( lp1 ), ps->m_nSortCol );
	CString s2 = ps->m_pList->GetItemText( ps->m_pList->FindIndex( lp2 ), ps->m_nSortCol );

	nResult = s1.CompareNoCase( s2 );

	return( ps->m_bSortAsc ? nResult : -nResult );
}

int CSortListCtrl::FindIndex( DWORD dw )
{
	int nCount = GetItemCount();
	for( int idx = 0; idx < nCount; idx++ )
	{
		if( dw == GetItemData( idx ) )
		{
			return( idx );
		}
	}

	return( -1 );
}
void CSortListCtrl::AddSortIndexData()
{
	int nCount = GetItemCount();
	for( int idx = 0; idx < nCount; idx++ )
		SetItemData( idx, idx );
}