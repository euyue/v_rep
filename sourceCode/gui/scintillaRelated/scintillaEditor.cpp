
#include "vrepMainHeader.h"
#include "funcDebug.h"
#include "scintillaEditor.h"
#include "scintillaDlg.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscistyle.h>

#include "vMessageBox.h"
#include "v_rep_internal.h"
#include "v_repStrings.h"
#include "app.h"

CScintillaEditor::CScintillaEditor()
{
    _dlg=nullptr;
    _scriptID=-1;
    _closeRequest=false;
    _currentWindowTitle="";
    _hideLevel=0;
}

CScintillaEditor::~CScintillaEditor() 
{
    _closeDlgWindow(true);
}

int CScintillaEditor::getScriptID()
{
    return(_scriptID);
}

void CScintillaEditor::showEditorButDontOpenIt()
{
    if (_hideLevel>0)
        _hideLevel--;
    if (_hideLevel==0)
    {
        if (_dlg!=nullptr)
            _dlg->setShowWindow(true);
    }
}

void CScintillaEditor::hideEditorButDontCloseIt()
{
    _hideLevel++;
    if (_hideLevel==1)
    {
        if (_dlg!=nullptr)
            _dlg->setShowWindow(false);
    }
}


bool CScintillaEditor::initialize(int scriptID)
{
    if ((App::operationalUIParts&sim_gui_scripteditor)==0)
        return(false);
    // Make sure that scripts cannot be viewed/edited with locked scenes:
    if (App::ct->environment->getSceneLocked())
    {
        App::uiThread->messageBox_warning(App::mainWindow,strTranslate("Script"),strTranslate(IDS_SCENE_IS_LOCKED_WARNING_NO_SCRIPT_VIEWING),VMESSAGEBOX_OKELI);
        return(false);
    }

    int scriptType=-1;
    bool scriptIsThreaded=false;
    CLuaScriptObject* it=App::ct->luaScriptContainer->getScriptFromID_alsoAddOnsAndSandbox(scriptID);
    if (it!=nullptr)
    {
        scriptType=it->getScriptType();
        scriptIsThreaded=it->getThreadedExecution();
    }
    _dlg=new CScintillaDlg(scriptType,scriptIsThreaded,App::mainWindow);
    if (_dlg==nullptr)
        return(false);
    if (it==nullptr)
    {
        _closeDlgWindow(true);
        return(false);
    }
    _dlg->setScriptID(scriptID);
    if (it->getScriptType()==sim_scripttype_mainscript)
    { 
        if (it->isDefaultMainScript())
        { // Display warning
            if (VMESSAGEBOX_REPLY_YES!=App::uiThread->messageBox_warning(App::mainWindow,strTranslate("Main script"),strTranslate(IDS_MAINSCRIPT_EDITION_WARNING),VMESSAGEBOX_YES_NO))
            {
                _closeDlgWindow(false);
                return(false);
            }
            // Have have to make the main script a default main script!
            it->setCustomizedMainScript(true);
            POST_SCENE_CHANGED_ANNOUNCEMENT(""); // **************** UNDO THINGY ****************
        }
    }
    int posAndSize[4];
    it->getPreviousEditionWindowPosAndSize(posAndSize);
    _dlg->initPhase1(posAndSize);
    _scriptID=scriptID;
    updateWindowExceptContentText();
    if (it->getScriptType()==sim_scripttype_mainscript)
        _dlg->setColorsAndMainStyles(0);
    if (it->getScriptType()==sim_scripttype_childscript)
    {
        if (it->getThreadedExecution())
            _dlg->setColorsAndMainStyles(2);
        else
            _dlg->setColorsAndMainStyles(1);
    }
    if (it->getScriptType()==sim_scripttype_jointctrlcallback)
        _dlg->setColorsAndMainStyles(3);
    if (it->getScriptType()==sim_scripttype_generalcallback)
        _dlg->setColorsAndMainStyles(5);
    _dlg->initPhase2(it->getScriptText());
    return(true);
}

void CScintillaEditor::getWindowPosAndSize(int posAndSize[4])
{
    FUNCTION_DEBUG;
    _dlg->getWindowPosAndSize(posAndSize);
}

bool CScintillaEditor::updateWindowExceptContentText()
{ // return value means true means we need to close the editor
    FUNCTION_DEBUG;
    if (_dlg==nullptr)
        return(true);
    CLuaScriptObject* it=App::ct->luaScriptContainer->getScriptFromID_alsoAddOnsAndSandbox(_scriptID);
    if ( (it==nullptr)||_closeRequest )
        return(true);

    std::string tmp(it->getDescriptiveName());

    if (tmp.compare(_currentWindowTitle)!=0)
    {
        _dlg->setWindowTitleText(tmp);
        _currentWindowTitle=tmp;
    }
    return(false);
}

void CScintillaEditor::newSceneProcedure()
{

}

void CScintillaEditor::requestClosing(void* dialogPointer)
{
    FUNCTION_DEBUG;
    if (dialogPointer==(void*)_dlg)
        _closeRequest=true;
}

void CScintillaEditor::_closeDlgWindow(bool announceChange)
{ // announceChange is true by default!
    if (_dlg==nullptr)
        return;
    bool changed=applyChanges();
    delete _dlg;
    _dlg=nullptr;
    _scriptID=-1;
    _currentWindowTitle="";
    if (announceChange&&changed)
    {
        POST_SCENE_CHANGED_ANNOUNCEMENT(""); // ************************** UNDO thingy **************************
    }
}

bool CScintillaEditor::applyChanges()
{
    FUNCTION_DEBUG;
    if (_dlg==nullptr)
        return(false);
    CLuaScriptObject* it=App::ct->luaScriptContainer->getScriptFromID_alsoAddOnsAndSandbox(_scriptID);
    if (it==nullptr)
        return(false);
    int l=_dlg->_scintillaObject->SendScintilla(QsciScintillaBase::SCI_GETLENGTH);
    char* buff=new char[l+1];
    _dlg->_scintillaObject->SendScintilla(QsciScintillaBase::SCI_GETTEXT,(unsigned long)l+1,buff);

    bool changed=false;
    const char* originalBuff=it->getScriptText();
    if (int(strlen(originalBuff))==l)
    {
        for (int i=0;i<l;i++)
        {
            if (originalBuff[i]!=buff[i])
            {
                changed=true;
                break;
            }
        }
    }
    else
        changed=true;

    it->setScriptText(buff);
    delete[] buff;
    return(changed);
}
