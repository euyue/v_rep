// Some parts of this code were inspired from Robert Umbehant's personal c compiler
// (http://www.codeproject.com/KB/cpp/Personal_C___Compiler.aspx)

#pragma once

#include "vrepMainHeader.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscistyle.h>
#include <QDialog>

struct SScintillaNMUserKeyword {
    std::string keyword;
    std::string callTip;
    bool autocomplete;
};

class CScintillaUserNonModalDlg : public QDialog
{
    Q_OBJECT

public:
    CScintillaUserNonModalDlg(const std::string& xmlInfo,int scriptId,const char* callbackFunc,bool simScript,QWidget* pParent = NULL, Qt::WindowFlags f= 0);
    virtual ~CScintillaUserNonModalDlg();

    void closeEvent(QCloseEvent *event);

    // UI thread only:
    void initialize(const char* text);
    void forceClose(std::string* txt,int pos[2],int size[2]);
    void showOrHideDlg(bool showState);

    // sim thread only:
    void handleCallback();

    void setHandle(int h);
    int getHandle() const;
    int getScriptId() const;
    bool getIsOpen() const;
    bool isAssociatedWithSimScript() const;
    std::string getCallbackFunc() const;

    static Qt::WindowFlags dialogStyle;

protected:
    void _getColorFromString(const char* txt,unsigned int& col) const;
    void _findText(const char* txt,bool caseSensitive);
    void _setColorsAndMainStyles();

    void _setAStyle(int style,unsigned int fore,unsigned int back=VRGB(0,0,0),int size=-1,const char *face=0);
    void _prepAutoCompletionList(const std::string& txt);
    std::string _getCallTip(const char* txt) const;

    std::vector<SScintillaNMUserKeyword> _keywords1;
    std::vector<SScintillaNMUserKeyword> _keywords2;
    std::string _autoCompletionList;
    std::string _allKeywords1;
    std::string _allKeywords2;
    bool _editable;
    bool _searchable;
    bool _open;
    int _handle;
    int _scriptId;
    bool _simScript;
    std::string _callbackFunc;
    std::string _textAtClosing;
    int _sizeAtClosing[2];
    int _posAtClosing[2];
    int _tabWidth;
    int _size[2];
    int _position[2];
    std::string _title;
    bool _isLua;
    bool _useVrepKeywords;
    unsigned int _textColor;
    unsigned int _backgroundColor;
    unsigned int _selectionColor;
    unsigned int _keywords1Color;
    unsigned int _keywords2Color;

    unsigned int _commentColor;
    unsigned int _numberColor;
    unsigned int _stringColor;
    unsigned int _characterColor;
    unsigned int _operatorColor;
    unsigned int _preprocessorColor;
    unsigned int _identifierColor;
    unsigned int _wordColor;
    unsigned int _word4Color;

    QsciScintilla* _scintillaObject;


public slots:
    void _onFind();
    void _charAdded(int charAdded);
    void _updateUi(int updated);
};

