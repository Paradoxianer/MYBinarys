#ifndef TEST_ANALYSER_H
#define TEST_ANALYSER_H


#include "IndexServerAddOn.h"

#include <Path.h>
#include <File.h>

const char* kTestDirectory = "TestAnalyser";


class TestAnalyser : public FileAnalyser {
public:
								TestAnalyser(BString name,
									const BVolume& volume);
								~TestAnalyser();

			status_t			InitCheck();

			void				AnalyseEntry(const entry_ref& ref);
			void				DeleteEntry(const entry_ref& ref);
			void				MoveEntry(const entry_ref& oldRef,
									const entry_ref& newRef);
			void				LastEntry();

private:
	inline	bool				_IsInIndexDirectory(const entry_ref& ref);
			uint32				fNUncommited;
			BPath				logBasePath;
};


class TestAddOn : public IndexServerAddOn {
public:
								TestAddOn(image_id id, const char* name);

			FileAnalyser*		CreateFileAnalyser(const BVolume& volume);
};

#endif
