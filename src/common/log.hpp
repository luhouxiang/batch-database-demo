# ifndef __KDS_MOBILE_STOCK_AUTH_SERVICE_LOG_H__
# define __KDS_MOBILE_STOCK_AUTH_SERVICE_LOG_H__

# include <string>
# include <map>
# include <pthread.h>

using namespace std;

bool initLog( const char* logPath, const char* strLevel );

typedef enum tagLogType {
    ERROR = 1,
    INFO,
    DEBUG,
    TEST,
}LOG_TYPE;
    
void logMsg( LOG_TYPE type, const char *fmt, ... );
void logBuffer( LOG_TYPE type, const char *module, const char *buffer, size_t size );

# ifndef LOG_MODULE
# define LOG_MODULE "LOG "
# endif

# define ERR_LOG( fmt, args... ) \
    logMsg( ERROR, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )

# define DBG_LOG( fmt, args... ) \
    logMsg( DEBUG, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )

# define RUN_LOG( fmt, args... ) \
    logMsg(INFO, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args)

# define LOG_BUFFER( buffer, len ) \
    logBuffer( INFO, LOG_MODULE, (buffer), (len) )

class TimeMx{
public:
	TimeMx() {
		pthread_mutex_lock(&s_time_mutex);
		bLocked = true;
	}
	~TimeMx(){
		if(bLocked)
			pthread_mutex_unlock(&s_time_mutex);
	}
	void unlock() {
		if(bLocked) {
			bLocked = false;
			pthread_mutex_unlock(&s_time_mutex);
		}
	}
private:
	static pthread_mutex_t s_time_mutex;
	int bLocked;
};


# endif
