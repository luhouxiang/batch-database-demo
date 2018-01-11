# include "cfg.h"
# include <cstdlib>
# include <string.h>
# include "log.hpp"

JsonReader* JsonReader::m_json_reader = NULL;

bool JsonReader::initialize(string filePath) {
	if (!m_init) {
		m_init = true;
		ifstream ifs(filePath.c_str(), ifstream::in);
		if (not ifs.is_open()) {
			return false;
		}
		ifs >> m_cfg;
		ifs.close();
		return true;	
	}
	return true;
}

bool JsonReader::read_string(const string key, string& value) {
	if (m_cfg[key.c_str()].empty()) {
		return false;
	}	
	value = m_cfg[key.c_str()].asString();
	return true;
}

bool JsonReader::read_string(const string node, const string key, string& value) {
	Json::Value jsNode = m_cfg[node.c_str()];
	if (jsNode.empty() || jsNode[key.c_str()].empty()) {
		return false;
	}
	value = jsNode[key.c_str()].asString();
	return true;
}

bool JsonReader::read_integer(const string key, uint32_t& value) {
	if (m_cfg[key.c_str()].empty()) {
		return false;
	}
	string temp = m_cfg[key.c_str()].asString();
	value = strtol(temp.c_str(), NULL, 10);	
	return true;	
}

bool JsonReader::read_integer(const string node, const string key, uint32_t& value) {
	Json::Value jsNode = m_cfg[node.c_str()];
	if (jsNode.empty() || jsNode[key.c_str()].empty()) {
		return false;
	}
	string temp = jsNode[key.c_str()].asString();
	value = strtol(temp.c_str(), NULL, 10);	
	return true;
}

// 加载基础配置
bool JsonReader::loadBaseConfig(Options& options) {
	
	string logPath;
	if (not read_string("log_path", options._log_path)) {
		options._log_path = "../log/order-service.log";
	}
	if(not read_string("log_level", options._log_level))
	{
		options._log_level = "DEBUG";
	}
	initLog(options._log_path.c_str(), options._log_level.c_str());


	if (not read_string("mssql", "mssql_host", options._mssql_host)) {
		DBG_LOG("read mssql host failed");
		return false;
	}
	if (not read_string("mssql", "mssql_user", options._mssql_user)) {
		DBG_LOG("read mssql user failed");
		return false;
	}
	if (not read_string("mssql", "mssql_password", options._mssql_passwd)) {
		DBG_LOG("read mssql password failed");
		return false;
	}
	if (not read_string("mssql", "mssql_dbname", options._mssql_db_name)) {
		DBG_LOG("read mssql dbname failed");
		return false;
	}
	if (not read_integer("mssql", "mssql_poolsize", options._mssql_pool_size)) {
		DBG_LOG("read mssql pool size failed");
		return false;
	}
	if (not read_integer("mssql", "mssql_login_timeout", options._mssql_lg_timeout_s)) {
		DBG_LOG("read mssql login timeout failed");
		return false;
	}
	if (not read_integer("mssql", "mssql_readwrite_timeout", options._mssql_rw_timeout_s)) {
		DBG_LOG("read mssql read write time failed");
		return false;
	}
	return true;
}

bool JsonReader::loadExtBaseConfig(Options& options) {

	return true;
}

// 加载所有配置
bool JsonReader::loadConfig(Options& options) {

	if (not loadBaseConfig(options)) {
		return false;
	}

	if (not loadExtBaseConfig(options)) {
		return false;
	}
	
	return true;
}