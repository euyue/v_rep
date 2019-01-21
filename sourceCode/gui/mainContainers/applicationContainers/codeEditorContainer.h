#pragma once

#include "vrepMainHeader.h"
#include <QString>

struct SCodeEditor {
    int handle;
    int scriptHandle;
    int callingScriptHandle;
    int sceneUniqueId;
    bool openAcrossScenes;
    bool closeAtSimulationEnd;
    bool systemVisibility;
    bool userVisibility;
    bool closeAfterCallbackCalled;
    bool restartScriptWhenClosing;
    std::string callbackFunction;
};


class CCodeEditorContainer
{
public:
    CCodeEditorContainer();
    virtual ~CCodeEditorContainer();

    // From UI thread:
    void showOrHideAll(bool showState);

    // From sim thread:
    int open(const char* initText,const char* xml,int callingScriptHandle);
    int openSimulationScript(int scriptHandle,int callingScriptHandle);
    int openCustomizationScript(int scriptHandle,int callingScriptHandle);
    int openConsole(const char* title,int maxLines,int mode,const int position[2],const int size[2],const int textColor[3],const int backColor[3],int callingScriptHandle);
    std::string openModalTextEditor(const char* initText,const char* xml,int windowSizeAndPos[4]) const;
    int openTextEditor(const char* initText,const char* xml,const char* callback,int callingScriptHandle,bool isSimulationScript);
    bool close(int handle,int posAndSize[4],std::string* txt,std::string* callback);
    bool closeFromScriptHandle(int scriptHandle,int posAndSize[4],bool ignoreChange);
    void restartScript(int handle) const;
    int getCallingScriptHandle(int handle) const;
    bool getCloseAfterCallbackCalled(int handle) const;
    void simulationAboutToStart() const;
    void simulationAboutToEnd();
    bool setText(int handle,const char* txt) const;
    std::string getText(int handle,int posAndSize[4]) const;
    bool isHandleValid(int handle) const;
    int showOrHide(int handle,bool showState);
    int getShowState(int handle) const;
    bool appendText(int handle,const char* txt) const;

    // From any thread:
    bool areSceneEditorsOpen() const;
    void sceneClosed(int sceneUniqueId);
    static QString getXmlColorString(const char* colTxt,const int rgbCol[3]);
    static QString getXmlColorString(const char* colTxt,int r,int g,int b);
    static QString getKeywords(int scriptType,bool threaded);
    static QString getFuncKeywords(int scriptType,bool threaded);
    static QString getVarKeywords(int scriptType,bool threaded);
    static QString translateXml(const char* oldXml,const char* callback);

protected:
    std::vector<SCodeEditor> _allEditors;
};
