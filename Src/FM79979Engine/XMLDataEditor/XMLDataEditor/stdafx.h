// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的
// 專案專用 Include 檔案
#pragma once

// TODO: 在此參考您的程式所需要的其他標頭
#include "../../Core/AllCoreInclude.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <list>
#include <xstring>
#include "windows.h"
#include "assert.h"
#include "NodeFile.h"
#include <time.h>
#include "../../DotNetCommon/DotNetCommonTools.h"
#include "../../Core/Common/BinaryFile/StringCompress.h"

long				GetFileSize(FILE*e_pFile);