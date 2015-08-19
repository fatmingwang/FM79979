//------- -------------------------------------------------------------------------------
// File: ZipFile.cpp
//
// Desc: Declaration of a class for loading uncompressed zip files. This code deals
//       byte ordering and should work on platforms with any endianness.
//
// Hist: 04.10.04 - Created
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef ZIP_HANDLING_H
#define ZIP_HANDLING_H

// Disable warning about C++ exception hander used when exceptions are disabled.
// This can be removed once disabling of STL thrown exceptions is enabled.
#pragma warning(disable : 4530)

// Disable warning about unreachable code in vector.
#pragma warning(disable : 4702)
#include <vector>
#include <assert.h>


//--------------------------------------------------------------------------------------
// Name: class CZipFile
// Desc: This class handles the loading, searching, and resource handling of
//       a zip file. Zip files are loaded entirely into memory and the
//       individual files can then be accessed.
//       Only uncompressed zip files are supported - the intent is to use zip
//       files as a way of packaging many files into one, not as a data
//       compression technique.
//       In order to avoid resource management issues this class does not
//       allow itself to be copied or assigned.
//--------------------------------------------------------------------------------------
class CZipFile
{
public:
    // Constructor - sets all resource pointers to zero.
    CZipFile()
        : m_zipFilename(0), m_data(0), m_dataSize(0)
    {
    }
    // Destructor - automatically frees resources.
    ~CZipFile()
    {
        Free();
    }

    // Free all the memory associated with this zip file
    void Free();

    // Load a new zip file into memory (freeing the previous zip file if there
    // was one). Returns true if the load succeeds.
    bool LoadZipFile(const WCHAR* filename);

    // Returns the name of the loaded zip file, or 0 if no zip file is loaded.
    const WCHAR* GetName() const {return m_zipFilename;}

    // Get the location and size of the specified file. Returns false if no
    // file of that name is found. Name matching is case sensitive.
    bool GetFile(const WCHAR* filename, WCHAR** pData, DWORD* pSize) const;

    // Get the filename for the specified file. Index must be less than GetCount().
    void GetFilename(DWORD index, WCHAR* fileBuffer, DWORD bufSize) const;

    // Get the file data for the specified file. Index must be less than GetCount().
    void GetFile(DWORD index, WCHAR** pData, DWORD* pSize) const;

    // Get the count of how many files there are in the loaded zip file, or zero
    // if no zip file is loaded.
    DWORD GetCount() const
    {
        assert(m_numEntries == m_centralDirOffsets.size());
        return m_numEntries;
    }

private:
    // This function scans the central directory, records the location of each file's
    // record, and does some error checking to make sure the file is not corrupt.
    bool MakeDirectory();

    // This function checks to see whether the specified offset and offset plus size
    // are within the .zip file.
    bool IsValidLocation(DWORD offset, DWORD size) const;

    // The name of the zip file loaded, or zero if no file is loaded.
    const WCHAR* m_zipFilename;

    // The location and size of the loaded zip file data, or zero.
    WCHAR*       m_data;
    DWORD       m_dataSize;

    // The number of files in the zip file. This must match the number of elements
    // in m_centralDirOffsets.
    DWORD       m_numEntries;

    // Stores the location of the central directory records for each file.
    // This lets us efficiently find the filename and other information about
    // each file.
    std::vector<size_t> m_centralDirOffsets;

    // Make these private and inaccessible so this class can't accidentally be
    // copied or assigned.
    CZipFile& operator=(const CZipFile& rhs);
    CZipFile(const CZipFile& rhs);
};

#endif
