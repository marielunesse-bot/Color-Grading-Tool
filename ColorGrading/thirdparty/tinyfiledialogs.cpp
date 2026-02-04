/*
 * tinyfiledialogs.cpp - Simplified implementation for Windows
 */

#ifdef _WIN32

#include "tinyfiledialogs.h"
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <string>
#include <vector>

static char tinyfd_response[1024];

char const * tinyfd_openFileDialog(
    char const * aTitle,
    char const * aDefaultPathAndFile,
    int aNumOfFilterPatterns,
    char const * const * aFilterPatterns,
    char const * aSingleFilterDescription,
    int aAllowMultipleSelects)
{
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = "";
    
    if (aDefaultPathAndFile && *aDefaultPathAndFile) {
        strncpy(szFile, aDefaultPathAndFile, MAX_PATH - 1);
    }
    
    // Build filter string
    std::string filter;
    if (aSingleFilterDescription && aNumOfFilterPatterns > 0) {
        filter = aSingleFilterDescription;
        filter += '\0';
        for (int i = 0; i < aNumOfFilterPatterns; i++) {
            if (i > 0) filter += ';';
            filter += aFilterPatterns[i];
        }
        filter += '\0';
    }
    filter += "All Files\0*.*\0\0";
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = aTitle;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    
    if (GetOpenFileNameA(&ofn)) {
        strncpy(tinyfd_response, szFile, sizeof(tinyfd_response) - 1);
        return tinyfd_response;
    }
    
    return NULL;
}

char const * tinyfd_saveFileDialog(
    char const * aTitle,
    char const * aDefaultPathAndFile,
    int aNumOfFilterPatterns,
    char const * const * aFilterPatterns,
    char const * aSingleFilterDescription)
{
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = "";
    
    if (aDefaultPathAndFile && *aDefaultPathAndFile) {
        strncpy(szFile, aDefaultPathAndFile, MAX_PATH - 1);
    }
    
    // Build filter string
    std::string filter;
    if (aSingleFilterDescription && aNumOfFilterPatterns > 0) {
        filter = aSingleFilterDescription;
        filter += '\0';
        for (int i = 0; i < aNumOfFilterPatterns; i++) {
            if (i > 0) filter += ';';
            filter += aFilterPatterns[i];
        }
        filter += '\0';
    }
    filter += "All Files\0*.*\0\0";
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = aTitle;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    
    if (GetSaveFileNameA(&ofn)) {
        strncpy(tinyfd_response, szFile, sizeof(tinyfd_response) - 1);
        return tinyfd_response;
    }
    
    return NULL;
}

char const * tinyfd_selectFolderDialog(
    char const * aTitle,
    char const * aDefaultPath)
{
    BROWSEINFOA bi;
    char szDir[MAX_PATH];
    LPITEMIDLIST pidl;
    
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = NULL;
    bi.pszDisplayName = szDir;
    bi.lpszTitle = aTitle;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    
    pidl = SHBrowseForFolderA(&bi);
    if (pidl != NULL) {
        if (SHGetPathFromIDListA(pidl, szDir)) {
            CoTaskMemFree(pidl);
            strncpy(tinyfd_response, szDir, sizeof(tinyfd_response) - 1);
            return tinyfd_response;
        }
        CoTaskMemFree(pidl);
    }
    
    return NULL;
}

int tinyfd_messageBox(
    char const * aTitle,
    char const * aMessage,
    char const * aDialogType,
    char const * aIconType,
    int aDefaultButton)
{
    UINT uType = MB_OK;
    
    if (aDialogType) {
        if (strcmp(aDialogType, "ok") == 0) uType = MB_OK;
        else if (strcmp(aDialogType, "okcancel") == 0) uType = MB_OKCANCEL;
        else if (strcmp(aDialogType, "yesno") == 0) uType = MB_YESNO;
        else if (strcmp(aDialogType, "yesnocancel") == 0) uType = MB_YESNOCANCEL;
    }
    
    if (aIconType) {
        if (strcmp(aIconType, "info") == 0) uType |= MB_ICONINFORMATION;
        else if (strcmp(aIconType, "warning") == 0) uType |= MB_ICONWARNING;
        else if (strcmp(aIconType, "error") == 0) uType |= MB_ICONERROR;
        else if (strcmp(aIconType, "question") == 0) uType |= MB_ICONQUESTION;
    }
    
    int result = MessageBoxA(NULL, aMessage, aTitle, uType);
    
    if (result == IDOK || result == IDYES) return 1;
    if (result == IDNO) return 0;
    return 0;
}

int tinyfd_notifyPopup(
    char const * aTitle,
    char const * aMessage,
    char const * aIconType)
{
    return tinyfd_messageBox(aTitle, aMessage, "ok", aIconType, 1);
}

char const * tinyfd_inputBox(
    char const * aTitle,
    char const * aMessage,
    char const * aDefaultInput)
{
    // Simple implementation - just return default
    if (aDefaultInput) {
        strncpy(tinyfd_response, aDefaultInput, sizeof(tinyfd_response) - 1);
        return tinyfd_response;
    }
    return NULL;
}

char const * tinyfd_colorChooser(
    char const * aTitle,
    char const * aDefaultHexRGB,
    unsigned char const aDefaultRGB[3],
    unsigned char aoResultRGB[3])
{
    CHOOSECOLORA cc;
    static COLORREF acrCustClr[16];
    COLORREF rgbCurrent = RGB(
        aDefaultRGB ? aDefaultRGB[0] : 0,
        aDefaultRGB ? aDefaultRGB[1] : 0,
        aDefaultRGB ? aDefaultRGB[2] : 0
    );
    
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = NULL;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    
    if (ChooseColorA(&cc)) {
        if (aoResultRGB) {
            aoResultRGB[0] = GetRValue(cc.rgbResult);
            aoResultRGB[1] = GetGValue(cc.rgbResult);
            aoResultRGB[2] = GetBValue(cc.rgbResult);
        }
        sprintf(tinyfd_response, "#%02X%02X%02X",
                GetRValue(cc.rgbResult),
                GetGValue(cc.rgbResult),
                GetBValue(cc.rgbResult));
        return tinyfd_response;
    }
    
    return NULL;
}

#endif /* _WIN32 */