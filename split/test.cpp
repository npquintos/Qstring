#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#pragma once
#include "include/osiDb.hpp"
#include "include/doctest.h"
#include "string.h"

TEST_CASE("Access any field value in Osi database") {
    char line[QLINEMAX];
    std::vector<std::tuple<std::string, std::string, std::string>> object_field_correctValue = {
        {"NA_CONST1", "case_title", "External17May2023"},
        {"LOAD", "name", "STN SERV"},
        {"TAP", "name", "1_7_1.05_0.9_0_0"},
        {"DBUS", "kv_mag", "0.946760237"},
        {"OPF_CONTINGENCY", "sol_status", ""} // this last one, no record
    }
    SUBCASE("Beginning of Object data properly determined") {
        auto qr = Qreader("../include/test.txt");
        int i = 0;
        while (qr.read(line)) {
            CHECK(!strcmp(line, correct[i]));
            ++i;
        }
    }
    SUBCASE("Values of fields are properly retrieved") {
        auto qr = Qreader("../include/test2.txt");
        int i = 0;
        while (qr.read(line)) {
            CAPTURE(line);
            CAPTURE(correct[i]);
            CHECK(!strcmp(line, correct2[i]));
            ++i;
        }
    }
}
