


#include <stdio.h>
#include <NodeInfo.h>
#include <Query.h>
#include <Volume.h>
#include <VolumeRoster.h>

const bigtime_t kSecond = 1000000;
	

int
main(int argc, char* argv[])
{
	bigtime_t	startBig	= real_time_clock_usecs();
	time_t		fStart		= startBig / kSecond;
	BVolume		bootVolume;
	status_t	err			= B_OK;
	BQuery		query;
	size_t		qLength		= 0;
	uint32		refsCount	= 0;
	BVolumeRoster roster;
	roster.GetBootVolume(&bootVolume);
		
	query.SetVolume(&bootVolume);
	query.PushAttr("last_modified");
	query.PushInt32(fStart);
	query.PushOp(B_LE);
	qLength=query.PredicateLength();
	char *queryString = new char[qLength+1];
	query.GetPredicate(queryString,qLength);
	printf("query %s\n", queryString);
	delete[] queryString;

	err = query.Fetch();
	entry_ref ref;
	while (query.GetNextRef(&ref) == B_OK) {
		printf("file %ld:\t %s\n",refsCount,ref.name);
		refsCount++;
	}
}
