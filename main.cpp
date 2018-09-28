#include <iostream>
#include <string>
#include <cstring>

#include "fcgio.h"
#include "libpostal/libpostal.h"

using namespace std;

char * parse_query_string(char * str, char find, char replace) {
  char *current_pos = std::strchr(str, find);
  while (current_pos) {
    *current_pos = replace;
    current_pos = std::strchr(current_pos, find);
  }
  return str;
}

int main(void) {
  // Backup the stdio streambufs
  streambuf * cin_streambuf  = cin.rdbuf();
  streambuf * cout_streambuf = cout.rdbuf();
  streambuf * cerr_streambuf = cerr.rdbuf();

  FCGX_Request request;

  FCGX_Init();
  FCGX_InitRequest(&request, 0, 0);

  // Setup (only called once at the beginning of your program)
  if (!libpostal_setup() || !libpostal_setup_parser()) {
    exit(EXIT_FAILURE);
  }

  while (FCGX_Accept_r(&request) == 0) {
    fcgi_streambuf cin_fcgi_streambuf(request.in);
    fcgi_streambuf cout_fcgi_streambuf(request.out);
    fcgi_streambuf cerr_fcgi_streambuf(request.err);

    cin.rdbuf(&cin_fcgi_streambuf);
    cout.rdbuf(&cout_fcgi_streambuf);
    cerr.rdbuf(&cerr_fcgi_streambuf);

    char * uri = FCGX_GetParam("QUERY_STRING", request.envp);
    char * address = parse_query_string(uri, '+', ' ');

    std::string parsed_address = "{";

    libpostal_address_parser_options_t options = libpostal_get_address_parser_default_options();
    libpostal_address_parser_response_t *parsed = libpostal_parse_address(address, options);

    for (size_t i = 0; i < parsed->num_components; i++) {
      parsed_address.append("\"");
      parsed_address.append(parsed->labels[i]);
      parsed_address.append("\":\"");
      parsed_address.append(parsed->components[i]);
      parsed_address.append("\"");
      if (i < parsed->num_components - 1) parsed_address.append(",");
    }

    parsed_address.append("}");

    cout << "Content-type: application/json\r\n\r\n" << parsed_address;

    // Free parse result
    libpostal_address_parser_response_destroy(parsed);
  }

  // Teardown (only called once at the end of your program)
  libpostal_teardown();
  libpostal_teardown_parser();

  // restore stdio streambufs
  cin.rdbuf(cin_streambuf);
  cout.rdbuf(cout_streambuf);
  cerr.rdbuf(cerr_streambuf);

  return 0;
}
