#include "Config.h"

#include "ParseSection.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

#define CONFIG_FILE_NAME				SYSTEM_NAME "d.cfg"	// newcoind.cfg
#define SECTION_PEER_IP					"peer_ip"
#define SECTION_PEER_PORT				"peer_port"
#define SECTION_RPC_IP					"rpc_ip"
#define SECTION_RPC_PORT				"rpc_port"
#define SECTION_VALIDATION_PASSWORD		"validation_password"
#define SECTION_VALIDATION_KEY			"validation_key"
#define SECTION_PEER_SSL_CIPHER_LIST	"peer_ssl_cipher_list"
#define SECTION_PEER_SCAN_INTERVAL_MIN	"peer_scan_interval_min"

Config theConfig;

Config::Config()
{
	VERSION					= 1;

	NETWORK_START_TIME		= 1319844908;

	PEER_PORT				= SYSTEM_PEER_PORT;
	RPC_PORT				= 5001;
	NUMBER_CONNECTIONS		= 30;

	// a new ledger every 30 min
	LEDGER_SECONDS			= (60*30);

	RPC_USER				= "admin";
	RPC_PASSWORD			= "pass";

	DATA_DIR				= "db/";

	PEER_SSL_CIPHER_LIST	= DEFAULT_PEER_SSL_CIPHER_LIST;
	PEER_SCAN_INTERVAL_MIN	= DEFAULT_PEER_SCAN_INTERVAL_MIN;

	TRANSACTION_FEE_BASE	= 1000;
}

void Config::load()
{
	std::ifstream	ifsConfig(CONFIG_FILE_NAME, std::ios::in);

	if (!ifsConfig)
	{
		std::cerr << "Failed to open '" CONFIG_FILE_NAME "'." << std::endl;
	}
	else
	{
		std::string	strConfigFile;

		strConfigFile.assign((std::istreambuf_iterator<char>(ifsConfig)),
			std::istreambuf_iterator<char>());

		if (ifsConfig.bad())
		{
			std::cerr << "Failed to read '" CONFIG_FILE_NAME "'." << std::endl;
		}
		else
		{
			section		secConfig	= ParseSection(strConfigFile, true);
			std::string	strTemp;

			(void) sectionSingleB(secConfig, SECTION_PEER_IP, PEER_IP);

			if (sectionSingleB(secConfig, SECTION_PEER_PORT, strTemp))
				PEER_PORT=boost::lexical_cast<int>(strTemp);

			(void) sectionSingleB(secConfig, SECTION_RPC_IP, RPC_IP);

			if (sectionSingleB(secConfig, SECTION_RPC_PORT, strTemp))
				RPC_PORT=boost::lexical_cast<int>(strTemp);

			(void) sectionSingleB(secConfig, SECTION_VALIDATION_PASSWORD, VALIDATION_PASSWORD);
			(void) sectionSingleB(secConfig, SECTION_VALIDATION_KEY, VALIDATION_KEY);

			(void) sectionSingleB(secConfig, SECTION_PEER_SSL_CIPHER_LIST, PEER_SSL_CIPHER_LIST);
			(void) sectionSingleB(secConfig, SECTION_PEER_SCAN_INTERVAL_MIN, strTemp);
				PEER_SCAN_INTERVAL_MIN=MAX(60, boost::lexical_cast<int>(strTemp));
		}
	}

	/*
	node=root.child("DB_TYPE");
	if(!node.empty())
	{
		if( stricmp(node.child_value(),"mysql")==0 ) theApp->setDB(Database::newMysqlDatabase("host","user","pass"));
		else theApp->setSerializer(new DiskSerializer());
	}else */

}

// vim:ts=4
