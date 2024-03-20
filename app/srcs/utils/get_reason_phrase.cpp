#include "utils.hpp"
#include <algorithm>
#include <cctype>

std::string getReasonPhrase(int statusCode) {
	std::string reasonPhrase;
	switch (statusCode) {
		// 1xx Informational
		case 100: reasonPhrase = "continue"; break;
		case 101: reasonPhrase = "switching protocols"; break;
		case 102: reasonPhrase = "processing"; break;
		case 103: reasonPhrase = "early hints"; break;
		// 2xx Success
		case 200: reasonPhrase = "ok"; break;
		case 201: reasonPhrase = "created"; break;
		case 202: reasonPhrase = "accepted"; break;
		case 203: reasonPhrase = "non authoritative information"; break;
		case 204: reasonPhrase = "no content"; break;
		case 205: reasonPhrase = "reset content"; break;
		case 206: reasonPhrase = "partial content"; break;
		case 207: reasonPhrase = "multi status"; break;
		case 208: reasonPhrase = "already reported"; break;
		case 226: reasonPhrase = "im used"; break;
		// 3xx Redirection
		case 300: reasonPhrase = "multiple choices"; break;
		case 301: reasonPhrase = "moved permanently"; break;
		case 302: reasonPhrase = "found"; break;
		case 303: reasonPhrase = "see other"; break;
		case 304: reasonPhrase = "not modified"; break;
		case 305: reasonPhrase = "use proxy"; break;
		case 307: reasonPhrase = "temporary redirect"; break;
		case 308: reasonPhrase = "permanent redirect"; break;
		// 4xx Client Errors
		case 400: reasonPhrase = "bad request"; break;
		case 401: reasonPhrase = "unauthorized"; break;
		case 402: reasonPhrase = "payment required"; break;
		case 403: reasonPhrase = "forbidden"; break;
		case 404: reasonPhrase = "not found"; break;
		case 405: reasonPhrase = "method not allowed"; break;
		case 406: reasonPhrase = "not acceptable"; break;
		case 407: reasonPhrase = "proxy authentication required"; break;
		case 408: reasonPhrase = "request timeout"; break;
		case 409: reasonPhrase = "conflict"; break;
		case 410: reasonPhrase = "gone"; break;
		case 411: reasonPhrase = "length required"; break;
		case 412: reasonPhrase = "precondition failed"; break;
		case 413: reasonPhrase = "payload too large"; break;
		case 414: reasonPhrase = "uri too long"; break;
		case 415: reasonPhrase = "unsupported media type"; break;
		case 416: reasonPhrase = "range not satisfiable"; break;
		case 417: reasonPhrase = "expectation failed"; break;
		case 418: reasonPhrase = "im a teapot"; break;
		case 421: reasonPhrase = "misdirected request"; break;
		case 422: reasonPhrase = "unprocessable entity"; break;
		case 423: reasonPhrase = "locked"; break;
		case 424: reasonPhrase = "failed dependency"; break;
		case 425: reasonPhrase = "too early"; break;
		case 426: reasonPhrase = "upgrade required"; break;
		case 428: reasonPhrase = "precondition required"; break;
		case 429: reasonPhrase = "too many requests"; break;
		case 431: reasonPhrase = "request header fields too large"; break;
		case 451: reasonPhrase = "unavailable for legal reasons"; break;
		// 5xx Server Errors
		case 500: reasonPhrase = "internal server error"; break;
		case 501: reasonPhrase = "not implemented"; break;
		case 502: reasonPhrase = "bad gateway"; break;
		case 503: reasonPhrase = "service unavailable"; break;
		case 504: reasonPhrase = "gateway timeout"; break;
		case 505: reasonPhrase = "http version not supported"; break;
		case 506: reasonPhrase = "variant also negotiates"; break;
		case 507: reasonPhrase = "insufficient storage"; break;
		case 508: reasonPhrase = "loop detected"; break;
		case 510: reasonPhrase = "not extended"; break;
		case 511: reasonPhrase = "network authentication required"; break;
		default: reasonPhrase = "unknown status code"; break;
	}
	for (std::string::size_type i = 0; i < reasonPhrase.length(); ++i)
		reasonPhrase[i] = std::toupper(static_cast<unsigned char>(reasonPhrase[i]));
	return reasonPhrase;
}


std::string read_htlm_file(std::string file_path)
{
	std::ifstream file(file_path.c_str());
	std::string str;
	std::string file_content;
	while (std::getline(file, str))
	{
		file_content += str;
	}
	return file_content;
}