# include <sstream>
# include <stdio.h>
# include <stdlib.h>
# include "version.h"
# include "common/version_utils.h"
# include "BusinessServer.h"

#define EXIT_ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
    fprintf(stderr, "(%s:%d)(%s) %d != %d\n", \
    __FILE__, __LINE__, __FUNCTION__, (expected), (actual)); \
    exit(1); \
    }

int main(int argc, char** argv) 
{

	int ch = 0;

	while ((ch = getopt(argc, argv, "v")) != -1)
	{
		switch (ch)
		{
		case 'v':
			ShowVersion(SERVICE_VERSION_STRING, KDS_PROGRAM_NAME_CN);
			return 0;
		default:
			printf("usage:\n\tgm-service -v\n");
			return 0;
		}
	}


 	BusinessServer server;
	int ret = server.LoadOptionsFromIni("./../etc/config.json");
	EXIT_ASSERT_EQ(0, ret);

	ret = server.Init();
	EXIT_ASSERT_EQ(0, ret);
	
	ret = server.start();
	EXIT_ASSERT_EQ(0, ret);

	server.Join();
			
	return 0;
}
