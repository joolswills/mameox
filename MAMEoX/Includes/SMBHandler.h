#pragma once

#include "MAMEoX.h"
#include "smb++.h"
#include "StdString.h"
#include <Vector>
#include <StdIO.h>

typedef INT64 offset_t;

class CSMBHandler
{
public:
  CSMBHandler(void);
  virtual ~CSMBHandler(void);
  bool GetDirectory(const CStdString& strDirectory, const CStdString& strExtFilter, std::vector<CStdString>& vecFiles, BOOL  = FALSE);
  bool CopyFile(const CStdString& strSrcFile, const CStdString& strDestFile);

  virtual void Close();
  virtual offset_t Seek(offset_t iFilePosition, int iWhence=SEEK_SET);
  virtual unsigned int Read(void* lpBuf, offset_t uiBufSize);
  virtual bool Open(const char* strFullPath);
  virtual offset_t GetLength();
  virtual offset_t GetPosition();

protected:
  void ParsePasswd(const CStdString& strSMBPath);

private:
  SMB* m_pSMB;
  offset_t m_fileSize;
  int m_fd;
};
