// Description : This is a simple web crawler in C++
//               Uses boost_regex and boost_algorithm
//				## Needs "Boost" development headers to be installed
//				## e.g. 'lib-boost' package in debian-based systems 
//				## and 'boost' package in arch-linux based systems
//============================================================================

/* To compile run : <g++ simpleWebCrawler.cpp -lboost_regex> */
 
#include <iostream>
#include <string>
#include <typeinfo>
#include <cstdarg>
#include <fstream>
 
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
 
using namespace std;
using namespace boost;
 
const int DELAY = 12;
const int MAXRECV = 140 * 1024;
const std::string WRITE_DIR_PATH = "~/Documents";
 
class WebPage {
public:
    std::string hostname;
    std::string page;
 
    WebPage() {
        hostname = "";
        page = "";
    }
 
    std::string parseHttp(const std::string str) {
        const boost::regex re("(?i)http://(.*)/?(.*)");
        boost::smatch what;
        if (boost::regex_match(str, what, re)) {
            std::string hst = what[1];
            boost::algorithm::to_lower(hst);
            return hst;
        }
        return "";
    } // End of method //
 
    void parseHref(const std::string orig_host, const std::string str) {
        const boost::regex re("(?i)http://(.*)/(.*)");
        boost::smatch what;
        if (boost::regex_match(str, what, re)) {
            // We found a full URL, parse out the 'hostname'
            // Then parse out the page
            hostname = what[1];
            boost::algorithm::to_lower(hostname);
            page = what[2];
        } else {
            // We could not find the 'page' but we can build the hostname
            hostname = orig_host;
            page = "";
        } // End of the if - else //
    } // End of method //
 
    void parse(const std::string orig_host, const std::string hrf) {
        const std::string hst = parseHttp(hrf);
        if (!hst.empty()) {
            // If we have a HTTP prefix
            // We could end up with a 'hostname' and page
            parseHref(hst, hrf);
        } else {
            hostname = orig_host;
            page = hrf;
        }
        // hostname and page are constructed,
        // perform post analysis
        if (page.length() == 0) {
            page = "/";
        } // End of the if //
    } // End of the method
}; // End of the class
 
std::string string_format(const std::string &fmt, ...) {
    int size = 255;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *) str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    } // End of the while //
    return str;
} // End of the function //
 
std::string request(std::string host, std::string path) {
    std::string request = "GET ";
    request.append(path);
    request.append(" HTTP/1.1\r\n");
    request.append("Host: ");
    request.append(host);
    request.append("\r\n");
    request.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.81\r\n");
    request.append("User-Agent: Mozilla/5.0 (compatible; octanebot/1.0; http://code.google.com/p/octane-crawler/)\r\n");
    request.append("Connection: close\r\n");
    request.append("\r\n");
    return request;
} // End of the function //
 
std::string clean_href(const std::string host, const std::string path) {
    // Clean the href to save to file //
    std::string full_url = host;
    full_url.append("/");
    full_url.append(path);
    const boost::regex rmv_all("[^a-zA-Z0-9]");
    const std::string s2 = boost::regex_replace(full_url, rmv_all, "_");
    cout << s2 << endl;
    return s2;
}
 
int connect(const std::string host, const std::string path) {
    const int port = 80;
 
    // Setup the msock
    int m_sock;
    sockaddr_in m_addr;
    memset(&m_addr, 0, sizeof(m_addr));
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
 
    int on = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1) {
        return false;
    }
 
    // Connect //
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);
 
    if (errno == EAFNOSUPPORT) {
        return false;
    }
    status = ::connect(m_sock, (sockaddr *) &m_addr, sizeof(m_addr));
 
    // HTTP/1.1 defines the "close" connection option for
    // the sender to signal that the connection will be closed
    // after completion of the response.
    std::string req = request(host, path);
    // End of building the request //
 
    status = ::send(m_sock, req.c_str(), req.size(), MSG_NOSIGNAL);
    char buf[MAXRECV];
 
    cout << "Request: " << req << endl;
    cout << "=========================" << endl;
 
    std::string recv = "";
    while (status != 0) {
        memset(buf, 0, MAXRECV);
        status = ::recv(m_sock, buf, MAXRECV, 0);
        recv.append(buf);
    } // End of the while //
    cout << "Response:" << recv << endl;
    cout << "---------------------------" << endl;
 
    // Attempt to write to file //
    const std::string html_file_write = string_format("%s/%s", WRITE_DIR_PATH.c_str(), clean_href(host, path).c_str());
    cout << "Writing to file : " << html_file_write << endl;
    ofstream outfile(html_file_write.c_str());
    outfile << recv << endl;
    outfile.close();
 
    // Parse the data //
    try {
        const boost::regex rmv_all("[\\r|\\n]");
        const std::string s2 = boost::regex_replace(recv, rmv_all, "");
        const std::string s = s2;
        // Use this regex expression, allow for mixed-case
        // Search for the anchor tag but not the '>'
        // Where (.+?) match anything
        //const boost::regex re("<a([^>]+) href='(.+?)'>");
        const boost::regex re("<a\\s+href\\s*=\\s*(\"([^\"]*)\")|('([^']*)')\\s*>");
        boost::cmatch matches;
        // Using token iterator with sub-matches
        const int subs[] = { 2, 4 };
        boost::sregex_token_iterator i(s.begin(), s.end(), re, subs);
        boost::sregex_token_iterator j;
        for (; i != j; i++) {
            // Iterate through the listed HREFs and
            // move to next request //
            const std::string href = *i;
            if (href.length() != 0) {
                WebPage* page = new WebPage();
                page->parse(host, href);
                const char* hrefc = page->page.c_str();
                cout << "Connecting to HTTP server with : " << page->hostname << " page=" << hrefc  << endl;
                sleep(DELAY);
                connect(page->hostname, string_format("/%s", hrefc));
                delete page;
            } // End of the if ///
        } // End of the for //
    } catch (boost::regex_error& e) {
        cout << "Error: " << e.what() << "\n";
    } // End of the try - catch //
    return 1;
} // End of the function //
 
int main() {
    cout << "Launching program" << endl;
    connect("http://cse.iitkgp.ac.in/", "index.php");
    cout << "Done" << endl;
    return 0;
} // End of the function //