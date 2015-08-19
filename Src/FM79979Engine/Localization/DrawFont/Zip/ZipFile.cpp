//--------------------------------------------------------------------------------------
// File: ZipFile.cpp
//
// Desc: Implementation of a class for loading uncompressed zip files. This code deals
//       with byte ordering and should work on platforms with any endianness.
//       This file exists primarily as an example of how to do byte swapping to correct
//       the endianness of data.
//
// Hist: 04.10.04 - Created
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <stdafx.h>
//#include <xtl.h>
#include <assert.h>
//#include <AtgUtil.h>
#include "ZipFile.h"
#include "EndianSwitch.h"


//--------------------------------------------------------------------------------------
// Name: LocalFileHeader
// Desc: Each file in a .zip file is preceded by a LocalFileHeader
//       struct, stored in little-endian format. We completely
//       ignore this structure, using only its size. Instead we get all
//       of the information about the files in the .zip file from the
//       central directory stored at the end of the .zip file.
//--------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct LocalFileHeader
{
    DWORD    signature;     // Must be 0x04034b50
    USHORT   versionNeeded;
    USHORT   bitFlags;
    USHORT   compressionMethod;
    USHORT   lastModificationTime;
    USHORT   lastModificationDate;
    DWORD    crc32;
    DWORD    compressedSize;
    DWORD    uncompressedSize;
    USHORT   filenameLength;
    USHORT   extraFieldLength;
    // filename (variable size, not null terminated)
    // extra field (variable size)

    // Not part of the struct - required signature value
    static const DWORD    SIGNATURE_VALUE = 0x04034b50;
};
#pragma pack(pop)


//--------------------------------------------------------------------------------------
// Name: CentralDirFileHeader
// Desc: The CentralDirfileHeader struct is used in the central directory at
//       end of the zip file to record information about the files in
//       a centralized location. The data in this struct is stored in little
//       endian format.
//       For more information search for information on the zip file format.
//--------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct CentralDirFileHeader
{
    DWORD    signature;     // Must be 0x02014b50
    USHORT   versionMadeBy;
    USHORT   versionNeeded;
    USHORT   bitFlags;
    USHORT   compressionMethod;
    USHORT   lastModificationTime;
    USHORT   lastModificationDate;
    DWORD    crc32;
    DWORD    compressedSize;
    DWORD    uncompressedSize;
    USHORT   filenameLength;
    USHORT   extraFieldLength;
    USHORT   fileCommentLength;
    USHORT   diskNumberStart;
    USHORT   internalFileAttributes;
    DWORD    externalFileAttributes;
    DWORD    localHeaderOffset;
    // filename (variable size, not null terminated)
    // extra field (variable size)
    // file comment (variable size)

    // Not part of the struct - required signature value
    static const DWORD    SIGNATURE_VALUE = 0x02014b50;
};
#pragma pack(pop)


//--------------------------------------------------------------------------------------
// Name: EndOfCentralDir
// Desc: At the end of each zip file is an EndOfCentralDir structure.
//       By reading this we can find the start of the central directory,
//       and find out about all the files in the .zip file in one place.
//       The optional variable length comment complicates this somewhat,
//       but we deal with this complication by not supporting .zip files
//       with comments. Adding this support would not be tricky if it is
//       desired. The data in this struct is stored in little endian format.
//       For more information search for information on the zip file format.
//--------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct EndOfCentralDir
{
    DWORD    signature;             // Must be 0x06054b50
    USHORT   thisDiskNumber;
    USHORT   centralDirDiskNumber;
    USHORT   centralDirEntriesThisDisk;
    USHORT   centralDirEntriesTotal;
    DWORD    centralDirSize;
    DWORD    centralDirStart;       // Location of central directory
    USHORT   fileCommentLength;
    // File comment - variable length

    // Not part of the struct - required signature value
    static const DWORD    SIGNATURE_VALUE = 0x06054b50;
};
#pragma pack(pop)


//--------------------------------------------------------------------------------------
// Name: LoadZipFile
// Desc: Loads a zip file entirely into memory and then calls MakeDirectory.
//       Returns false if the load fails.
//--------------------------------------------------------------------------------------
bool CZipFile::LoadZipFile(const WCHAR* filename)
{
    // First free any previously loaded files.
    Free();

    // Then, open the specified file.
    HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
		assert(0 &&"Error opening file \n");
//        ATG::FatalError( "Error opening file '%s'.\n", filename );
        return false;
    }

    // Find the file size.
    DWORD fileSizeHigh;
    DWORD fileSizeLow = GetFileSize(file, &fileSizeHigh);
    if (fileSizeLow == INVALID_FILE_SIZE && GetLastError() != NO_ERROR)
    {
        CloseHandle(file);
		assert(0 && "Error getting size for filename .\n");
        //ATG::FatalError( "Error getting size for '%s'.\n", filename );
        return false;
    }

    // Allocate the necessary memory and read the data.
    assert(fileSizeHigh == 0);
    m_data = new WCHAR[fileSizeLow];
    DWORD bytesRead = 0;
    ReadFile(file, m_data, fileSizeLow, &bytesRead, 0);

    CloseHandle(file);

    // Make sure the requested amount of data was read.
    if (bytesRead != fileSizeLow)
    {
        Free();
		assert(0&&"Error reading file filename \n");
        //ATG::FatalError( "Error reading file '%s'.\n", filename );
        return false;
    }

    // Now record information about the file we've loaded.
    m_zipFilename = _wcsdup(filename);
    m_dataSize = fileSizeLow;

    // Create a directory listing of the files in the zip file.
    // This also checks the zip file for validity.
    if (!MakeDirectory())
    {
        Free();
        return false;
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Name: MakeDirectory
// Desc: This function scans the zip file central directory at the end of the
//       file, looking for any errors and recording the directory location of
//       each file. This makes it simple to go to a file by number, if
//       desired. This function carefully avoids touching the LocalFileHeader
//       (in the data section of the zip file), to avoid bringing in cache
//       lines that aren't really needed.
//       No other information - like file name or file location - is stored,
//       because the central directory already serves as a repository for
//       that information.
//       MakeDirectory is automatically called after a zip file is opened.
//--------------------------------------------------------------------------------------
bool CZipFile::MakeDirectory()
{
    assert(m_centralDirOffsets.empty());

    // First find and validate the central directory
    if (m_dataSize < sizeof(EndOfCentralDir))
    {
		assert(0&&"Corrupt or unsupported zip file.\n");
        //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
        return false;
    }
    // Look at the end of the file for the EndOfCentralDir struct
	// for data exchange make data move by byte not WCHAR so change data type to bye not original WCHAR
    EndOfCentralDir* pEndOfCentralDir = (EndOfCentralDir*)
                ((BYTE*)m_data + (m_dataSize - sizeof(EndOfCentralDir)));
    // This format string means an int, then four shorts, then two ints, then a short.
    LittleEndianToNative(pEndOfCentralDir, "i4siis");
    if (pEndOfCentralDir->signature != EndOfCentralDir::SIGNATURE_VALUE)
    {		
        // Zip files can contain a file comment, which goes at the end of
        // the file. If there is a file comment then the EndOfCentralDir
        // structure will not be at the end of the file. The only way to find
        // the EndOfCentralDir structure in this case is to scan backwards
        // through the last 64K of the file looking for the signature (which
						// for 64 is because 
						// UINT uSrcXInc = (m_uSrcWidth  << 16) / m_uDstWidth;
						//UINT uSrcYInc = (m_uSrcHeight << 16) / m_uDstHeight;
        // had better not show up in the file comment.)
        // For the sake of simplicity this zip file reader does not support zip
        // files with comments - it treats them as being corrupt and unreadable.
        //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
		assert(0&&"Corrupt or unsupported zip file.\n");
        return false;
    }

    m_numEntries = pEndOfCentralDir->centralDirEntriesTotal;

    // Allocate enough space to store the offsets of the central directory entries
    // for all of the files.
    m_centralDirOffsets.resize(m_numEntries);
    DWORD    currentOffset = pEndOfCentralDir->centralDirStart;
    for (DWORD i = 0; i < m_numEntries; ++i)
    {
        // Record the location of this directory entry.
        m_centralDirOffsets[i] = currentOffset;

        // Make sure the specified central dir location is valid
        if (!IsValidLocation(currentOffset, sizeof(CentralDirFileHeader)))
        {
			assert(0&&"Corrupt or unsupported zip file.\n");
            //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
            return false;
        }
		// for data exchange make data move by byte not WCHAR so change data type to bye not original WCHAR
        CentralDirFileHeader* pFileHeader = (CentralDirFileHeader*)((BYTE*)m_data + currentOffset);
        // This format string means an int, then six shorts, then three ints, then five
        // shorts, then two ints.
        LittleEndianToNative(pFileHeader, "i6s3i5s2i");
        if (pFileHeader->signature != CentralDirFileHeader::SIGNATURE_VALUE)
        {
			assert(0&&"Corrupt or unsupported zip file.\n");
            //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
            return false;
        }

        if (pFileHeader->filenameLength == 0)
        {
			assert(0&&"Corrupt or unsupported zip file.\n");
            //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
            return false;
        }

        // This simple zip library doesn't handle compressed files.
        if (pFileHeader->compressedSize != pFileHeader->uncompressedSize
                    || pFileHeader->compressionMethod != 0)
            return false;

        // Calculate the location of the file data. This is done by taking the
        // offset to the local file header (which precedes) the file data, then
        // adding the size of the local file header, then adding the size of the
        // two variable length fields which go immediately after the local
        // file header. This offset is calculated in order to find out now
        // whether the location is correct, to detect corrupt zip files when they
        // are opened.
        DWORD    dataOffset = pFileHeader->localHeaderOffset + sizeof(LocalFileHeader) +
                    pFileHeader->filenameLength + pFileHeader->extraFieldLength;

        // Make sure the file data location is valid.
        // If this check fails it means the .zip file is corrupt.
        if (!IsValidLocation(dataOffset, pFileHeader->compressedSize))
        {
			assert(0&&"Corrupt or unsupported zip file.\n");
            //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
            return false;
        }

        // Calculate the location of the next central directory record - we can use
        // that to make sure that the current central directory record fits entirely
        // in the zip file.

        // Since these are 16-bit fields we don't need to worry about integer overflow,
        // unless our currentOffset is impossibly big.
        currentOffset += sizeof(*pFileHeader) + pFileHeader->filenameLength +
                    pFileHeader->extraFieldLength + pFileHeader->fileCommentLength;

        // Make sure the entire record is valid, including the variable length fields,
        // using the updated offset. Without this check the filename might be extending
        // past the end of the file.
        if (!IsValidLocation(currentOffset, 0))
        {
			assert(0&&"Corrupt or unsupported zip file.\n");
            //ATG::FatalError( "Corrupt or unsupported zip file.\n" );
            return false;
        }

        // Now we know we can safely access any part of the current central directory
        // record, but for now we don't need to.
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Name: GetFilename
// Desc: Get the filename for the specified file in the zip file.
//       The index must be in range.
//--------------------------------------------------------------------------------------
void CZipFile::GetFilename(DWORD index, WCHAR* fileBuffer, DWORD bufSize) const
{
    assert(index >= 0 && index < m_numEntries);
    const CentralDirFileHeader* pFileHeader =
                (const CentralDirFileHeader*)(((BYTE*)m_data + m_centralDirOffsets[index]));

    const char* filename = (const char*)(pFileHeader + 1);
    // Warn with an assert if the string will not fit. It will
    // be truncated if it doesn't fit.
    assert(pFileHeader->filenameLength < bufSize);
    // Adjust bufSize to the size of the filename (including NULL) or the
    // actual buffer size, whichever is smaller.
    if (pFileHeader->filenameLength + 1U < bufSize)
        bufSize = pFileHeader->filenameLength + 1;

    CHAR  strAnsiName[512] = {0};
    WCHAR strName[512];
    SIZE_T dwNameLength = 512;
    MultiByteToWideChar( CP_ACP, 0, filename, -1, strName, 512 );

    strName[511] = 0;
    wcsncpy_s( fileBuffer, bufSize, strName, bufSize - 1 );
}


//--------------------------------------------------------------------------------------
// Name: GetFile
// Desc: Get the location and data size for the file specified by the index.
//       The index must be in range.
//--------------------------------------------------------------------------------------
void CZipFile::GetFile(DWORD index, WCHAR** pData, DWORD* pSize) const
{
    assert(index >= 0 && index < m_numEntries);
    const CentralDirFileHeader* pFileHeader =
                (const CentralDirFileHeader*)((BYTE*)m_data + m_centralDirOffsets[index]);

    // Calculate the offset of the data. The validity of this location is
    // checked when the directory is made.
    DWORD    dataOffset = pFileHeader->localHeaderOffset + sizeof(LocalFileHeader) +
                pFileHeader->filenameLength + pFileHeader->extraFieldLength;

    *pData = (WCHAR*)((BYTE*)m_data + dataOffset);
    *pSize = pFileHeader->compressedSize;
}


//--------------------------------------------------------------------------------------
// Name: GetFile
// Desc: Looks for the specified file name in the zip file - doing a case
//       sensitive match - and returns the location and size.
//       If the file does not exist it returns false.
//--------------------------------------------------------------------------------------
bool CZipFile::GetFile(const WCHAR* filename, WCHAR** pData, DWORD* pSize) const
{
    DWORD numEntries = GetCount();
    // Scan through all the directory entries in the zip file
    for (DWORD i = 0; i < numEntries; ++i)
    {
        // Get the name of this file
        WCHAR buffer[250];
        GetFilename(i, buffer, sizeof(buffer));

        // If the name matches, return the location and size of that file.
        if (wcscmp(buffer, filename) == 0)
        {
            GetFile(i, pData, pSize);
            return true;
        }
    }

    return false;
}


//--------------------------------------------------------------------------------------
// Name: IsValidLocation
// Desc: Helper function to check whether the range from offset to offset+size
//       is within the zip file - returns true if the range is valid.
//--------------------------------------------------------------------------------------
bool CZipFile::IsValidLocation(DWORD offset, DWORD size) const
{
    assert(offset >= 0 && size >= 0);
    DWORD    endOffset = offset + size;
    // Check for integer overflow - can be used to cause buffer overflows
    if (endOffset < 0 || endOffset < offset)
        return false;

    if (endOffset > m_dataSize)
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Name: Free
// Desc: Free all the data associated with the zip file. This function is
//       called automatically by the ZipFile destructor.
//--------------------------------------------------------------------------------------
void CZipFile::Free()
{
    delete [] m_data;
    m_data = 0;

    free(const_cast<WCHAR*>(m_zipFilename));
    m_zipFilename = 0;

    m_dataSize = 0;
    m_centralDirOffsets.clear();
}
