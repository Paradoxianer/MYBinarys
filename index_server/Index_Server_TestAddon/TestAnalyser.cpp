/*
 * Copyright 2016, 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Paradoxon
 */
#include "TestAnalyser.h"

#include <new>

#include <Directory.h>
#include <String.h>
#include <TranslatorFormats.h>
#include <TranslatorRoster.h>

#include "IndexServerPrivate.h"


#define DEBUG_TEST_ANALYSER
#ifdef DEBUG_TEST_ANALYSER
#include <stdio.h>
#	define STRACE(x...) ; printf("TestAnalyser: " x)
#else
#	define STRACE(x...) ;
#endif


TestAnalyser::TestAnalyser(BString name, const BVolume& volume)
	:
	FileAnalyser(name, volume),
	fNUncommited(0)
{
	STRACE("TestAnalyser(%s)\n",name.String());
	BDirectory	dir;
	volume.GetRootDirectory(&dir);
	
	logBasePath.SetTo(&dir);
	logBasePath.Append(kIndexServerDirectory);
	logBasePath.Append("TestAnalyser.log");
	printf("\t logging to %s \n",logBasePath.Path());
}


TestAnalyser::~TestAnalyser()
{
}


status_t
TestAnalyser::InitCheck()
{
	STRACE("InitCheck()\n");
	return B_OK;
}


void
TestAnalyser::AnalyseEntry(const entry_ref& ref)
{
	BPath path(&ref);
	BString line("");
	ssize_t writtenBytes;
	line << system_time();
	line << "\t";
	line << path.Path();
	line << "\t";
	line << ref.name;
	line << "\t";
	line << ref.device;
	line << "\t";
	line << ref.directory;
	line << "\n";
	STRACE("TestAnalyser AnalyseEntry: %s %s \t", ref.name, path.Path());
	BFile fileLog = BFile(logBasePath.Path(),B_READ_WRITE|B_CREATE_FILE|B_OPEN_AT_END);
	writtenBytes = fileLog.Write(line.String(),line.Length());
	STRACE("error %d \n",writtenBytes);
	fNUncommited++;
	if (fNUncommited > 100)
		LastEntry();
}


void
TestAnalyser::DeleteEntry(const entry_ref& ref)
{
	if (_IsInIndexDirectory(ref))
		return;
	STRACE("TestAnalyser DeleteEntry: %s\n", ref.name);
}


void
TestAnalyser::MoveEntry(const entry_ref& oldRef, const entry_ref& newRef)
{
	STRACE("TestAnalyser MoveEntry: %s to %s\n", oldRef.name, newRef.name);
	AnalyseEntry(newRef);
}


void
TestAnalyser::LastEntry()
{
	STRACE("TestAnalyser LastEntry():");

	fNUncommited = 0;
}



bool
TestAnalyser::_IsInIndexDirectory(const entry_ref& ref)
{
	BPath path(&ref);
	
	if (BString(path.Path()).FindFirst("/boot/system/cache/tmp") == 0)
		return true;

	return false;
}


TestAddOn::TestAddOn(image_id id, const char* name)
	:
	IndexServerAddOn(id, name)
{
	
}


FileAnalyser*
TestAddOn::CreateFileAnalyser(const BVolume& volume)
{
	return new (std::nothrow)TestAnalyser(Name(), volume);
}


extern "C" IndexServerAddOn* (instantiate_index_server_addon)(image_id id,
	const char* name)
{
	return new (std::nothrow)TestAddOn(id, name);
}
