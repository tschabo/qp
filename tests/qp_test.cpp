#include "acutest.h"
#include "qp.hpp"
#include <iostream>
#include <string>

void test_html(void)
{
    TEST_CHECK(qp::encode("<!doctype html>\r\n"
                          "<html lang=\"de\">\r\n"
                          "  <head>\r\n"
                          "    <meta charset=\"utf-8\">\r\n"
                          "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
                          "    <title>Beschreibung der Seite</title>\r\n"
                          "  </head>\r\n"
                          "  <body>\r\n"
                          "    <p>Inhalt der Seite</p>\r\n"
                          "  </body>\r\n"
                          "</html>\r\n") == std::string("<!doctype html>\r\n"
                                                        "<html lang=3D\"de\">\r\n"
                                                        "  <head>\r\n"
                                                        "    <meta charset=3D\"utf-8\">\r\n"
                                                        "    <meta name=3D\"viewport\" content=3D\"width=3Ddevice-width, initial-scale=\r\n"
                                                        "=3D1.0\">\r\n"
                                                        "    <title>Beschreibung der Seite</title>\r\n"
                                                        "  </head>\r\n"
                                                        "  <body>\r\n"
                                                        "    <p>Inhalt der Seite</p>\r\n"
                                                        "  </body>\r\n"
                                                        "</html>\r\n"));
}

void test_line_breaks(void)
{
    auto res = qp::encode("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567");
    TEST_CHECK(res == std::string("1234567890123456789012345678901234567890123456789012345678901234567890123456=\r\n78901234567"));
    res = qp::encode("123456789012345678901234567890123456789012345678901234567890123456789012345=78901234567");
    TEST_CHECK(res == std::string("123456789012345678901234567890123456789012345678901234567890123456789012345=\r\n=3D78901234567"));
    res = qp::encode("12345678901234567890123456789012345678901234567890123456789012345678901234=678901234567");
    TEST_CHECK(res == std::string("12345678901234567890123456789012345678901234567890123456789012345678901234=\r\n=3D678901234567"));
    res = qp::encode("1234567890123456789012345678901234567890123456789012345678901234567890123=5678901234567");
    TEST_CHECK(res == std::string("1234567890123456789012345678901234567890123456789012345678901234567890123=3D=\r\n5678901234567"));
    res = qp::encode("123456789012345678901234567890123456789012345678901234567890123456789012==5678901234567");
    TEST_CHECK(res == std::string("123456789012345678901234567890123456789012345678901234567890123456789012=3D=\r\n=3D5678901234567"));
    res = qp::encode("12345678901234567890123456789012345678901234567890123456789012345678901==5678901234567\r\n12345678901234567890123456789012345678901234567890123456789012345678901==5678901234567");
    TEST_CHECK(res == std::string("12345678901234567890123456789012345678901234567890123456789012345678901=3D=\r\n=3D5678901234567\r\n12345678901234567890123456789012345678901234567890123456789012345678901=3D=\r\n=3D5678901234567"));
}

void test_dot_stuffing(void)
{
    TEST_CHECK(qp::encode(".\r\n", qp::flags::use_dot_stuffing) == std::string("..\r\n"));
    TEST_CHECK(qp::encode("..\r\n", qp::flags::use_dot_stuffing) == std::string("..\r\n"));
    TEST_CHECK(qp::encode("\r\n.\r\n", qp::flags::use_dot_stuffing) == std::string("\r\n..\r\n"));
    TEST_CHECK(qp::encode(".\r\n") == std::string(".\r\n"));
    TEST_CHECK(qp::encode("..\r\n") == std::string("..\r\n"));
    TEST_CHECK(qp::encode("\r\n.\r\n") == std::string("\r\n.\r\n"));
}

TEST_LIST = {
    {"simple html", test_html},
    {"line breaks", test_line_breaks},
    {"dot stuffing", test_dot_stuffing},
    {NULL, NULL}};