#include <sstream>
#include "lspcgi.h"

void lsp::output_error_page(FCGX_Request* r, int err, const char* info)
{
	if (!info)
	{
		switch (err)
		{
			case 400:
				info = "Bad Request";
				break;
			case 500:
				info = "Internal Server Error";
				break;
			default:
				info = "Unkown Error";
				break;
		}
	}
	std::stringstream ss;
	ss << err << " " << info;
	std::string str = "Content-type: text/html\r\n" + ss.str();
	str += "\r\n\r\n";
	str += "<html><head><title>" + ss.str();
	str += "</title></head> <body> HTTP " + ss.str();
	str += "</body></html>";

	FCGX_PutS(str.c_str(), r->out);
}
