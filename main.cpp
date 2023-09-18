#include <iostream>
#include "qp.hpp"

int main(int, char **)
{
    const auto str = qp::encode("<!doctype html>\r\n"
                                "<html lang=\"de\">\r\n"
                                "  <head>\r\n"
                                "    <meta charset=\"utf-8\">\r\n"
                                "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
                                "    <title>Beschreibung der Seite</title>\r\n"
                                "  </head>\r\n"
                                "  <body>\r\n"
                                "    <p>Inhalt der Seite</p>\r\n"
                                "  </body>\r\n"
                                "</html>\r\n");
    std::cout << str;
}
