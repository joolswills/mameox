/**
* \file			smbhandler.cpp
* \brief		Implementation of the SMBHandler class,
*           includes both directory listing handling
*           and reading file data over a smb share
*/

//= I N C L U D E S ====================================================
#include "smbhandler.h"
#include "xbox_Network.h"

//= G L O B A L = V A R S ==============================================
static char szPassWd[128];

//= C L A S S E S / S T R U C T U R E S ================================
class MyCallback : public SmbAnswerCallback
{
protected:
  char buf[200];
public:
  char *getAnswer(int type, const char *optmessage) 
  {
    switch (type) 
    {
    case ANSWER_USER_NAME:
      strcpy(buf, szPassWd);
      break;
    case ANSWER_USER_PASSWORD:
      strcpy(buf, szPassWd);
      break;
    case ANSWER_SERVICE_PASSWORD:
      strcpy(buf, szPassWd);
      break;
    }
    
    return buf;
  }
} cb;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSMBHandler::CSMBHandler(void)
{
  m_pSMB = new SMB();

  if( g_NetworkConfig.m_NameServer[0] != 0x00 )
    m_pSMB->setNBNSAddress(g_NetworkConfig.m_NameServer);

  m_pSMB->setPasswordCallback(&cb);
  m_fd = -1;
}

CSMBHandler::~CSMBHandler(void)
{
  Close();
}

//----------------------------------------------------------------------
// GetDirectory
//
// Parameters:
//  strDirectory: Directory to get file list from
//  strExtFilter: Extension to filter for (like .zip)
//  vecFiles    : Vector where found filenames are returned in
//  bChopExt    : TRUE if you want the extension removed from the 
//                filename
//----------------------------------------------------------------------
bool CSMBHandler::GetDirectory(const CStdString& strDirectory, 
                               const CStdString& strExtFilter, 
                               std::vector<CStdString>& vecFiles,
                               BOOL bChopExt)
{  
  ParsePasswd(strDirectory);

  SMBdirent* dirEnt;

  int fd = m_pSMB->opendir(strDirectory);
  if (fd < 0) 
  {
    return false;
  }

  while ((dirEnt = m_pSMB->readdir(fd))) 
  {
    if (dirEnt->d_name && strcmp(dirEnt->d_name,".") && strcmp(dirEnt->d_name,".."))
    {
      CStdString strName = dirEnt->d_name;

      if ((strName.Find(strExtFilter) != -1) || (strExtFilter.IsEmpty()))
      {
        // Chop off the extension if we need to
        if (bChopExt)
        {
          strName = strName.Left(strName.Find(strExtFilter));
        }

        // Add the fielname to the vector
        vecFiles.push_back(strName);
      }
    }
  };

  m_pSMB->closedir(fd);

  return true;
}

//----------------------------------------------------------------------
// CopyFile
//
// Parameters:
//  strSrcFile:  Source SMB file to copy from
//  strDestFile: Destination local file to copy to
//----------------------------------------------------------------------
bool CSMBHandler::CopyFile(const CStdString& strSrcFile, const CStdString& strDestFile)
{
  if (!Open(strSrcFile))
  {
    return false;
  }

  // Open the output file for write
  HANDLE hFile;
  hFile = CreateFile(	strDestFile,
                      GENERIC_WRITE,
                      0,
                      NULL,
                      CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL );

  if (hFile == INVALID_HANDLE_VALUE)
  {
    return false;
  }

  // Read data in 65KB chunks
  byte* buff = new byte[65535];

  // Loop reading in 65KB chunks and writing to the output file
  // until the file is completely read
  while (1)
  {
    // Read in 65K bytes from the samba file
    int iReadSize = Read(buff, 65535);

    // do the write
    DWORD result;
    WriteFile(hFile, buff, iReadSize, &result, NULL);

    if (iReadSize != 65535)
    {
      break;
    }
  }
  
  delete [] buff;
  CloseHandle(hFile);

  return true;
}

//----------------------------------------------------------------------
// ParsePasswd
//
// Parameters:
//  strSMBPath:  SMB Path string to parse passwd from
//----------------------------------------------------------------------
void CSMBHandler::ParsePasswd(const CStdString& strSMBPath)
{
  // Parse the password from the path and copy into the global
  int iPassStartIdx = strSMBPath.Find(':', 5);
  if (iPassStartIdx != -1)
  {
    int iPassEndIdx = strSMBPath.Find('@', iPassStartIdx);
    if (iPassEndIdx == -1)
    {
      iPassEndIdx = strSMBPath.GetLength();
    }

    // Copy the string back to the global
    strcpy(szPassWd, strSMBPath.Mid(iPassEndIdx, iPassEndIdx-iPassStartIdx));
  }
  else
  {
    strcpy(szPassWd, "");
  }
}

bool CSMBHandler::Open(const char* szFileName)
{
  ParsePasswd(szFileName);

  m_fd = m_pSMB->open(szFileName, O_RDONLY);

  if( m_fd < 0 )
  {
    return false;
  }

  INT64 ret = m_pSMB->lseek(m_fd, 0, SEEK_END);

  if( ret < 0 )
  {
    m_pSMB->close(m_fd);
    m_fd = -1;
    return false;
  }

  m_fileSize = ret;
  ret = m_pSMB->lseek(m_fd, 0, SEEK_SET);

  if( ret < 0 )
  {
    m_pSMB->close(m_fd);
    m_fd = -1;
    return false;
  }

  // We've successfully opened the file!
  return true;
}

unsigned int CSMBHandler::Read(void *lpBuf, offset_t uiBufSize)
{
  int bytesRead = m_pSMB->read(m_fd, lpBuf, (int)uiBufSize);

  if( bytesRead <= 0 )
  {
    return 0;
  }

  return (unsigned int)bytesRead;
}

offset_t CSMBHandler::Seek(offset_t iFilePosition, int iWhence)
{
  INT64 pos = m_pSMB->lseek(m_fd, (int)iFilePosition, iWhence);

  if( pos < 0 )
    return 0;

  return (offset_t)pos;
}

void CSMBHandler::Close()
{
  if (m_pSMB)
  {
    if (m_fd>=0)
      m_pSMB->close(m_fd);
    delete m_pSMB;
  }

  m_pSMB=NULL;
  m_fd = -1;
}

offset_t CSMBHandler::GetPosition()
{
  int pos = m_pSMB->lseek(m_fd, 0, SEEK_CUR);

  if( pos < 0 )
    return 0;

  return (offset_t)pos;
}

offset_t CSMBHandler::GetLength()
{
  return m_fileSize;
}