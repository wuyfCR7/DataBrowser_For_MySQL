#include "UIConnectorInfo.h"
#include <iostream>
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libboost_thread-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_date_time-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_system-vc110-mt-1_58.lib")
#pragma comment(lib, "fltk.lib")
#pragma comment(lib, "fltkforms.lib")
int main()
{
	UIConnectorInfo::GetInstance().OnInitizeDialog();
	UIConnectorInfo::GetInstance().show();

	Fl::run();

	std::string host_ = UIConnectorInfo::GetInstance().host_;
	std::string user_ = UIConnectorInfo::GetInstance().user_;
	std::string password_ = UIConnectorInfo::GetInstance().password_;
	unsigned int port_ = UIConnectorInfo::GetInstance().port_;	
	bool res_ = UIConnectorInfo::GetInstance().is_info_prepared_;

	return NULL;
}