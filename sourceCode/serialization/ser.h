#pragma once

#include "serBase.h"
#include "vrepMainHeader.h"
#include "vArchive.h"
#include "tinyxml2.h"

#define SER_VREP_HEADER "VREP" // The file header since 03/07/2009
#define SER_END_OF_OBJECT "EOO"
#define SER_NEXT_STEP "NXT"
#define SER_END_OF_FILE "EOF"
typedef tinyxml2::XMLElement xmlNode;

class CSer : public CSerBase
{
public:
    CSer(VArchive& ar);
    CSer(std::vector<char>& bufferArchive);
    CSer(const char* xmlFilename);
    virtual ~CSer();

    void writeOpen(bool compress,char filetype);
    void writeClose();

    int readOpen(int& serializationVersion,unsigned short& vrepVersionThatWroteThis,int& licenseTypeThatWroteThis,char& revNumber,bool ignoreTooOldSerializationVersion);
    void readClose();

    CSer& operator<< (const int& v);
    CSer& operator<< (const float& v);
    CSer& operator<< (const double& v);
    CSer& operator<< (const unsigned short& v);
    CSer& operator<< (const unsigned int& v);
    CSer& operator<< (const quint64& v);
    CSer& operator<< (const long& v);
    CSer& operator<< (const unsigned char& v);
    CSer& operator<< (const char& v);
    CSer& operator<< (const std::string& v);

    CSer& operator>> (int& v);
    CSer& operator>> (float& v);
    CSer& operator>> (double& v);
    CSer& operator>> (unsigned short& v);
    CSer& operator>> (unsigned int& v);
    CSer& operator>> (quint64& v);
    CSer& operator>> (long& v);
    CSer& operator>> (unsigned char& v);
    CSer& operator>> (char& v);
    CSer& operator>> (std::string& v);

    void flush(bool writeNbOfBytes=true);
    
    bool isStoring();
    bool isBinary();
    void setCountingMode(bool force=false);
    bool setWritingMode(bool force=false);
    void disableCountingModeExceptForExceptions();
    std::vector<unsigned char>* getBufferPointer();
    std::vector<unsigned char>* getFileBuffer();
    int getFileBufferReadPointer();
    void addOffsetToFileBufferReadPointer(int off);
    int getCounter();
    void loadUnknownData();
    bool getFoundUnknownCommands();

    unsigned short getVrepVersionThatWroteThisFile();
    int getLicenseTypeThatWroteThisFile();
    int getSerializationVersionThatWroteThisFile();
    
    void storeDataName(const char* name);
    std::string readDataName();
    int readBytesButKeepPointerUnchanged(unsigned char* buffer,int desiredCount);
    VArchive& getArchive();

    xmlNode* xmlGetCurrentNode();
    void xmlSetCurrentNode(xmlNode* node);
    xmlNode* xmlCreateNode(const char* name);
    xmlNode* xmlCreateNode(const char* name,const char* nameAttribute);
    xmlNode* xmlCreateNode(const char* name,int idAttribute);
    void xmlAddNode(xmlNode* parentNode,xmlNode* node);
    void xmlAddNode_string(xmlNode* parentNode,const char* name,const char* str);
    void xmlAddNode_int(xmlNode* parentNode,const char* name,int val);
    void xmlAddNode_2int(xmlNode* parentNode,const char* name,int val1,int val2);
    void xmlAddNode_3int(xmlNode* parentNode,const char* name,int val1,int val2,int val3);

    xmlNode* xmlGetNode_fromParent(xmlNode* parentNode,const char* name);
    xmlNode* xmlGetNode_fromSibling(xmlNode* siblingNode,const char* name);
    bool xmlGetNode_nameAttribute(xmlNode* node,std::string& val);
    bool xmlGetNode_idAttribute(xmlNode* node,int& val);
    bool xmlGetNode_string(xmlNode* parentNode,const char* name,std::string& val);
    bool xmlGetNode_int(xmlNode* parentNode,const char* name,int& val);
    bool xmlGetNode_2int(xmlNode* parentNode,const char* name,int& val1,int& val2);
    bool xmlGetNode_3int(xmlNode* parentNode,const char* name,int& val1,int& val2,int& val3);

    static char getFileTypeFromName(const char* filename);

    static int SER_SERIALIZATION_VERSION;
    static int SER_MIN_SERIALIZATION_VERSION_THAT_CAN_READ_THIS;
    static int SER_MIN_SERIALIZATION_VERSION_THAT_THIS_CAN_READ;
    static int XML_SERIALIZATION_VERSION;

private:
    void _commonInit();
    void _writeBinaryHeader();
    void _writeXmlHeader();
    void _writeXmlFooter();
    int _readXmlHeader(int& serializationVersion,unsigned short& vrepVersionThatWroteThis,char& revNumber);
    void _readXmlHeader();
    VArchive* theArchive;
    std::vector<char>* _bufferArchive;
    tinyxml2::XMLDocument _xmlDocument;
    xmlNode* _xmlCurrentNode;
    bool _compress;
    char _filetype;
    std::string _xmlFilename;
    int counter;
    int countingMode;
    bool _coutingModeDisabledExceptForExceptions;
    std::vector<unsigned char> buffer;
    std::vector<unsigned char> _fileBuffer;
    int _fileBufferReadPointer;
    bool _foundUnknownCommands;

    unsigned short _vrepVersionThatWroteThis;
    int _licenseTypeThatWroteThis;
    int _serializationVersionThatWroteThisFile;
};
