# ifndef __KDS_MOBILE_STOCK_FAST_CGI_SERVICE_CFG_H__
# define __KDS_MOBILE_STOCK_FAST_CGI_SERVICE_CFG_H__

# include <json/json.h>
# include <fstream>
# include <string>
# include "../Options.h"

using namespace std;

class JsonReader {
protected:
	JsonReader() {
		m_init = false;	
	};
	~JsonReader() {
		if (NULL != m_json_reader) {
			delete m_json_reader;
			m_json_reader = NULL;
		}
	}
private:
	JsonReader(const JsonReader &);
	JsonReader& operator=(const JsonReader &);		
private:
	static JsonReader* m_json_reader;
public:
	static JsonReader* instance() {
		if (NULL == m_json_reader) {
			m_json_reader = new JsonReader();
		}
		return m_json_reader;
	}
public:
	bool initialize(string filePath);
private:	
	bool read_string(const string key, string& value);
	bool read_integer(const string key, uint32_t& value);
	bool read_string(const string node, const string key, string& value);
	bool read_integer(const string node, const string key, uint32_t& value);
	bool loadBaseConfig(Options& options);
	bool loadExtBaseConfig(Options& options);
public:	
	bool loadConfig(Options& options);
private:
	Json::Value m_cfg;
	bool m_init;
};

# endif