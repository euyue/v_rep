#include "vrepMainHeader.h"
#include "scintillaUserNonModalDlgContainer.h"
#include "v_rep_internal.h"
#include "app.h"

CScintillaUserNonModalDlgContainer::CScintillaUserNonModalDlgContainer()
{
    _nextHandle=0;
}

CScintillaUserNonModalDlgContainer::~CScintillaUserNonModalDlgContainer()
{
    for (size_t i=0;i<_allDlgs.size();i++)
        delete _allDlgs[i];
    _allDlgs.clear();
}

int CScintillaUserNonModalDlgContainer::addDlg(CScintillaUserNonModalDlg* c)
{
    c->setHandle(_nextHandle++);
    _allDlgs.push_back(c);
    return(c->getHandle());
}

void CScintillaUserNonModalDlgContainer::removeDlg(int handle)
{
    for (size_t i=0;i<_allDlgs.size();i++)
    {
        if (_allDlgs[i]->getHandle()==handle)
        {
            delete _allDlgs[i];
            _allDlgs.erase(_allDlgs.begin()+i);
            break;
        }
    }
}

CScintillaUserNonModalDlg* CScintillaUserNonModalDlgContainer::getDlg(int handle) const
{
    for (size_t i=0;i<_allDlgs.size();i++)
    {
        if (_allDlgs[i]->getHandle()==handle)
            return(_allDlgs[i]);
    }
    return(NULL);
}

void CScintillaUserNonModalDlgContainer::handleDlgRemoval()
{
    for (size_t i=0;i<_allDlgs.size();i++)
    {
        CScintillaUserNonModalDlg* dlg=_allDlgs[i];
        if (dlg->getSceneUniqueId()==App::ct->environment->getSceneUniqueID())
        {
            bool remove=( (!dlg->getIsOpen())&&(dlg->getCallbackFunc().size()==0) );
            if (!remove)
            {
                CLuaScriptObject* it=App::ct->luaScriptContainer->getScriptFromID_alsoAddOnsAndSandbox(dlg->getScriptId());
                if ( (it==NULL)||(!it->hasLuaState()) )
                    remove=true; // script was removed or has no state
                else
                {
                    if ( App::ct->simulation->isSimulationStopped()&&dlg->isAssociatedWithSimScript() )
                        remove=true;
                }
            }
            if (remove)
            { // dlg close button was pressed and callback called. We can remove that dlg
                delete dlg;
                _allDlgs.erase(_allDlgs.begin()+i);
                i--; // reprocess this position
            }
        }
    }
}

void CScintillaUserNonModalDlgContainer::handleCallbacks()
{
    for (size_t i=0;i<_allDlgs.size();i++)
    {
        CScintillaUserNonModalDlg* dlg=_allDlgs[i];
        if ( (!dlg->getIsOpen())&&(dlg->getCallbackFunc().size()!=0) )
            dlg->handleCallback(); // dlg was closed and callback not yet called
    }
}

void CScintillaUserNonModalDlgContainer::showOrHideAll(bool showState)
{
    if (App::ct->environment!=NULL)
    {
        int sceneId=App::ct->environment->getSceneUniqueID();
        for (size_t i=0;i<_allDlgs.size();i++)
        {
            CScintillaUserNonModalDlg* dlg=_allDlgs[i];
            if (dlg->getSceneUniqueId()==sceneId)
                dlg->showOrHideDlg(showState);
        }
    }
}
