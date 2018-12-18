#pragma once

#include "vrepMainHeader.h"
#include "scintillaUserNonModalDlg.h"

class CScintillaUserNonModalDlgContainer
{
public:
    CScintillaUserNonModalDlgContainer();
    virtual ~CScintillaUserNonModalDlgContainer();

    // From UI thread:
    void removeDlg(int handle);
    void handleDlgRemoval();
    void showOrHideAll(bool showState);

    // From sim thread:
    void handleCallbacks();

    // From any thread;
    int addDlg(CScintillaUserNonModalDlg* c);
    CScintillaUserNonModalDlg* getDlg(int handle) const;

protected:
    int _nextHandle;
    std::vector<CScintillaUserNonModalDlg*> _allDlgs;
};
