//***************************************************************************
//
//  ExpMenu.c
//
//***************************************************************************
#include "windows.h"
#include <commctrl.h>
#include <shlobj.h>

#define TPM_RECURSE         0x0001L

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       GetNextItem
//
//  DESCRIPTION:    Finds the next item in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Pointer to the next item.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST GetNextItem (LPITEMIDLIST pidl)
{
    USHORT nLen;

    if ((nLen = pidl->mkid.cb) == 0)
        return NULL;
    
    return (LPITEMIDLIST) (((LPBYTE) pidl) + nLen);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       GetItemCount
//
//  DESCRIPTION:    Computes the number of item IDs in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Number of item IDs in the list.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

UINT GetItemCount (LPITEMIDLIST pidl)
{
    USHORT nLen;
    UINT nCount;

    nCount = 0;
    while ((nLen = pidl->mkid.cb) != 0) {
        pidl = GetNextItem (pidl);
        nCount++;
    }
    return nCount;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       DuplicateItem
//
//  DESCRIPTION:    Makes a copy of the next item in an item ID list.
//
//  INPUT:          pMalloc = Pointer to an IMalloc interface.
//                  pidl    = Pointer to an item ID list.
//
//  RETURNS:        Pointer to an ITEMIDLIST containing the copied item ID.
//
//  NOTES:          It is the caller's responsibility to free the memory
//                  allocated by this function when the item ID is no longer
//                  needed. Example:
//
//                    pidlItem = DuplicateItem (pMalloc, pidl);
//                      .
//                      .
//                      .
//                    pMalloc->lpVtbl->Free (pMalloc, pidlItem);
//
//                  Failure to free the ITEMIDLIST will result in memory
//                  leaks.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST DuplicateItem (LPMALLOC pMalloc, LPITEMIDLIST pidl)
{
    USHORT nLen;
    LPITEMIDLIST pidlNew;

    nLen = pidl->mkid.cb;
    if (nLen == 0)
        return NULL;

    pidlNew = (LPITEMIDLIST) pMalloc->lpVtbl->Alloc (pMalloc,
        nLen + sizeof (USHORT));
    if (pidlNew == NULL)
        return NULL;

    CopyMemory (pidlNew, pidl, nLen);
    *((USHORT*) (((LPBYTE) pidlNew) + nLen)) = 0;

    return pidlNew;
}

/////////////////////////////////////////////////////////////////////////////
// Context menu functions

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       DoExplorerMenu
//
//  DESCRIPTION:    Given a path name to a file or folder object, displays
//                  the shell's context menu for that object and executes
//                  the menu command (if any) selected by the user.
//
//  INPUT:          hwnd    = Handle of the window in which the menu will be
//                            displayed.
//
//                  pszPath = Pointer to an ANSI or Unicode string
//                            specifying the path to the object.
//
//                  point   = x and y coordinates of the point where the
//                            menu's upper left corner should be located, in
//                            client coordinates relative to hwnd.
//  
//  RETURNS:        TRUE if successful, FALSE if not.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

UINT DoExplorerMenu (HWND hwnd, LPCTSTR pszPath, POINT point,BOOL bEx)
{
    LPMALLOC pMalloc;
    LPSHELLFOLDER psfFolder, psfNextFolder;
    LPITEMIDLIST pidlMain, pidlItem, pidlNextItem, *ppidl;
    LPCONTEXTMENU pContextMenu;
    CMINVOKECOMMANDINFO ici;
    ULONG ulCount, ulAttr;
    TCHAR tchPath[MAX_PATH];
    WCHAR wchPath[MAX_PATH];    
    UINT nCount, nCmd;
    HMENU hMenu;

    //
    // Make sure the file name is fully qualified and in Unicode format.
    //
    GetFullPathName (pszPath, sizeof (tchPath) / sizeof (TCHAR), tchPath,
        NULL);

    if (IsTextUnicode (tchPath, lstrlen (tchPath), NULL))
        lstrcpy ((char *) wchPath, tchPath);
    else
        MultiByteToWideChar (CP_ACP, 0, pszPath, -1, wchPath, sizeof (wchPath) / sizeof (WCHAR));

    //
    // Get pointers to the shell's IMalloc interface and the desktop's
    // IShellFolder interface.
    //
    nCmd = 0;

    if (!SUCCEEDED (SHGetMalloc (&pMalloc)))
        return nCmd;

    if (!SUCCEEDED (SHGetDesktopFolder (&psfFolder)))
	{
        pMalloc->lpVtbl->Release (pMalloc);
        return nCmd;
    }

    //
    // Convert the path name into a pointer to an item ID list (pidl).
    //
    if (SUCCEEDED (psfFolder->lpVtbl->ParseDisplayName (psfFolder, hwnd,
        NULL, wchPath, &ulCount, &pidlMain, &ulAttr)) && (pidlMain != NULL))
	{
        if (nCount = GetItemCount (pidlMain)) { // nCount must be > 0
            //
            // Initialize psfFolder with a pointer to the IShellFolder
            // interface of the folder that contains the item whose context
            // menu we're after, and initialize pidlItem with a pointer to
            // the item's item ID. If nCount > 1, this requires us to walk
            // the list of item IDs stored in pidlMain and bind to each
            // subfolder referenced in the list.
            //
            pidlItem = pidlMain;

            while (--nCount)
			{
                //
                // Create a 1-item item ID list for the next item in pidlMain.
                //
                pidlNextItem = DuplicateItem (pMalloc, pidlItem);
                if (pidlNextItem == NULL)
				{
                    pMalloc->lpVtbl->Free (pMalloc, pidlMain);
                    psfFolder->lpVtbl->Release (psfFolder);
                    pMalloc->lpVtbl->Release (pMalloc);
                    return nCmd;
                }

                //
                // Bind to the folder specified in the new item ID list.
                //
                if (!SUCCEEDED (psfFolder->lpVtbl->BindToObject (psfFolder,
                    pidlNextItem, NULL, &IID_IShellFolder, &psfNextFolder)))
				{
                    pMalloc->lpVtbl->Free (pMalloc, pidlNextItem);
                    pMalloc->lpVtbl->Free (pMalloc, pidlMain);
                    psfFolder->lpVtbl->Release (psfFolder);
                    pMalloc->lpVtbl->Release (pMalloc);
                    return nCmd;
                }

                //
                // Release the IShellFolder pointer to the parent folder
                // and set psfFolder equal to the IShellFolder pointer for
                // the current folder.
                //
                psfFolder->lpVtbl->Release (psfFolder);
                psfFolder = psfNextFolder;

                //
                // Release the storage for the 1-item item ID list we created
                // just a moment ago and initialize pidlItem so that it points
                // to the next item in pidlMain.
                //
                pMalloc->lpVtbl->Free (pMalloc, pidlNextItem);
                pidlItem = GetNextItem (pidlItem);
            }

            //
            // Get a pointer to the item's IContextMenu interface and call
            // IContextMenu::QueryContextMenu to initialize a context menu.
            //
            ppidl = &pidlItem;
            if (SUCCEEDED (psfFolder->lpVtbl->GetUIObjectOf (psfFolder,
                hwnd, 1, ppidl, &IID_IContextMenu, NULL, &pContextMenu)))
			{
                hMenu = CreatePopupMenu ();
                if (SUCCEEDED (pContextMenu->lpVtbl->QueryContextMenu (
                    pContextMenu, hMenu, 0, 1, 0x7FFF, CMF_EXPLORE /*| CMF_CANRENAME*/ )))
				{

                    ClientToScreen (hwnd, &point);

                    //
                    // Display the context menu.
                    //
                    if(!bEx)
						nCmd = TrackPopupMenu (hMenu, TPM_LEFTALIGN |
							TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
							point.x, point.y, 0, hwnd, NULL);
					else
						nCmd = TrackPopupMenuEx (hMenu, TPM_LEFTALIGN |
							TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD| TPM_RECURSE,
							point.x, point.y, hwnd, NULL);
                    //
                    // If a command was selected from the menu, execute it.
                    //
                    if (nCmd)
					{
                        ici.cbSize          = sizeof (CMINVOKECOMMANDINFO);
                        ici.fMask           = 0;
                        ici.hwnd            = hwnd;
                        ici.lpVerb          = MAKEINTRESOURCE (nCmd - 1);
                        ici.lpParameters    = NULL;
                        ici.lpDirectory     = NULL;
                        ici.nShow           = SW_SHOWNORMAL;
                        ici.dwHotKey        = 0;
                        ici.hIcon           = NULL;

                        if (!SUCCEEDED (pContextMenu->lpVtbl->InvokeCommand (pContextMenu, &ici)))
							nCmd = 0;
                    }
                }
                DestroyMenu (hMenu);
                pContextMenu->lpVtbl->Release (pContextMenu);
            }
        }
        pMalloc->lpVtbl->Free (pMalloc, pidlMain);
    }

    //
    // Clean up and return.
    //
    psfFolder->lpVtbl->Release (psfFolder);
    pMalloc->lpVtbl->Release (pMalloc);

    return nCmd;//nResult;
}
