#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using std::string;
using std::vector;
class Global
{
public:
    Global();
    ~Global();
    static string gameSessionId;
    static long score;
	static string outputheader;
	static string outputbody;
    static string toString(vector<char> *buffer);

    static string getSessionIdFromHeader(string header);
	static string getHeader(string header);
	static string getbody(string header);
};

#endif
