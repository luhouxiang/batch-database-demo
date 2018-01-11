/*
 * log.cpp -- 通用日志处理模块
 *
 * 版权所有(c)，2014 - 2015 深圳市金慧盈通数据服务有限公司
 * 核心技术，如需分发请在法律许可范围。
 *
 * 本文件是通用日志处理模块
 */

# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <time.h>
# include <sys/errno.h>
# include <ctype.h>
#include <unistd.h>
# include <sys/syscall.h>
# include "log.hpp"

#define KDS_LOG_NAME "batch"

static FILE *gLogFp = NULL;
static LOG_TYPE gLogLevel = INFO;

pthread_mutex_t TimeMx::s_time_mutex = PTHREAD_MUTEX_INITIALIZER;

pid_t gettid()
{
    return syscall(SYS_gettid);
}

int strIcmp(const char* p1, const char* p2) 
{ 
    while (*p1) 
    { 
        char c1 = *p1; 
        char c2 = *p2; 

        if (c1 >= 'A' && c1 <= 'Z') 
        { 
            c1 += 'a' - 'A'; 
        } 
        if (c2 >= 'A' && c2 <= 'Z') 
        { 
            c2 += 'a' - 'A'; 
        } 
        if (c1 != c2) 
        { 
            return c1 - c2; 
        } 
        p1++; 
        p2++; 
    } 
    return *p1 - *p2; 
}

bool initLog( const char* logPath, const char* strLevel )
{
	gLogFp = fopen( logPath, "ab" );
	if ( gLogFp == NULL ){
		ERR_LOG( "日志模块初始化失败，因为打开日志文件 %s 时遇到错误: [%d] %s。",
			logPath, errno, strerror(errno) );
		return false;
	}

    string logLevel = strLevel;

    if ( !strIcmp("ERROR",logLevel.c_str()) ){
        gLogLevel = ERROR;
    }
    else if ( !strIcmp("INFO",logLevel.c_str())){
        gLogLevel = INFO;
    }
    else if ( !strIcmp("DEBUG",logLevel.c_str())){
        gLogLevel = DEBUG;
    }
    else if (!strIcmp("test",logLevel.c_str()) ){
        gLogLevel = TEST;
    }
    else{
        gLogLevel = INFO;
    }
    return true;
}

void logMsg( LOG_TYPE type, const char *fmt, ... )
{
    va_list args;
    time_t now;
    struct tm *buf;

    if ( gLogFp == NULL ){
        gLogFp = stderr;
        //return;
    }

    if ( type > gLogLevel ){
        return;
    }

	struct tm tbuf;
    now = time( NULL );
	{
		TimeMx mx;
		buf = localtime_r( &now, &tbuf);
	}


    fprintf( gLogFp, KDS_LOG_NAME " " "%s %04d-%02d-%02d %02d:%02d:%02d %d %d ",
            ( type == ERROR ? "ERR" : type == DEBUG ? "DBG" : type == INFO ? "RUN" : "---" ),
            buf->tm_year + 1900,
            buf->tm_mon + 1,
            buf->tm_mday,
            buf->tm_hour,
            buf->tm_min,
            buf->tm_sec, 
			getpid(),
            gettid());

    va_start( args, fmt );
    vfprintf( gLogFp, fmt, args );
    va_end( args );

    fflush( gLogFp );

    return;
}

void logBuffer( LOG_TYPE type, const char *module, const char *buffer, size_t size )
{
    static char spaceLine[49] = "                                                ";
    int lineWidth = 16;
    time_t now;
    struct tm *buf;
	struct tm tbuf;

    if ( gLogFp == NULL ){
        return;
    }

    if ( type > gLogLevel ){
        return;
    }

    now = time( NULL );
	{
		TimeMx mx;
		buf = localtime_r( &now, &tbuf);
	}

    size_t i=0;
    while(1){
        fprintf( gLogFp, KDS_LOG_NAME " " "%s %04d-%02d-%02d %02d:%02d:%02d %d %d ",
                ( type == ERROR ? "ERR" : type == DEBUG ? "DBG" : type == INFO ? "RUN" : "---" ),
                buf->tm_year + 1900,
                buf->tm_mon + 1,
                buf->tm_mday,
                buf->tm_hour,
                buf->tm_min,
                buf->tm_sec,
                getpid(),
                gettid());

        fprintf( gLogFp, "%s %06lx ", module, i );

        int j=0;
        for( j=0; i<size && j<lineWidth; j++, i++ ){
            fprintf( gLogFp, "%02x ", (unsigned char)buffer[i] );
        }

        fwrite( spaceLine, (lineWidth - j) * 3, 1, gLogFp );
        fprintf( gLogFp, "  " );

        i -= j;
        for( j=0; i<size && j<lineWidth; j++, i++ ){
            fprintf( gLogFp, "%c", isprint( buffer[i] ) ? buffer[i] : '.' );
        }

        fprintf( gLogFp, "\n" );

        if ( i>=size ){
            break;
        }
    }
}
