#include "Statistics.hxx"
#include "Logging.hxx"

#include <stdio.h>

int main() {
	Logging::SetLogLevel(Logging::LEVEL_INFO);
	Punisher::Statistics stats;

	printf("Freshly initialized stats object\n");
	stats.Print();

	printf("New stats object\n");
	Punisher::Statistics statsnew;
	statsnew.SetTotalRequests(100);
	statsnew.AddTotalRequests(50);
	statsnew.Print();

	printf("Another new stats object\n");
	Punisher::Statistics statsnewnew;
	statsnewnew.SetTotalRequests(25);
	statsnewnew.Print();

	printf("Adding statsnewnew and statsnew\n");
	statsnew += statsnewnew;
	statsnew.Print();

	printf("Adding statsnew to original\n");
	stats += statsnew;
	stats.Print();

	printf("Assign stats to statsnew\n");
	statsnew = stats;
	statsnew.Print();

	return 0;
}
