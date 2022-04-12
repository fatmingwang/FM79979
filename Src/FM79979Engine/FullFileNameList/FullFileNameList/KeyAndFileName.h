#pragma once

using namespace System;

[Serializable]
ref class cKeyAndFileName
{
public:
	String^ Key;
	String^ FileName;
};

[Serializable]
ref class cBundleAndFileMap
{
public:
	String^ Name;
	System::Collections::ArrayList^ Array;
};