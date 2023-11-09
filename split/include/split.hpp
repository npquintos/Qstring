#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>
#include "include/doctest.h"

#define MAX_JOINED_CHARACTERS 10000
// #define MAX_JOINED_CHARACTERS 20

class Qstring {
    // Usage:
    // auto xsplit = Qstring(line).split(delimeter);
    // for (auto word : xsplit) {
    //      std::cout << word << std::endl
    // }
    // if 'word' is to be split, 'ptr' below will
    // hold the starting indexes of the start of
    // the word. 'word' will then be modified to
    // be delimited with \0. For example, say you have
    // this word and you want to split with 'abcd' as
    // delimiter, similar to word.split('abcd') in python
    // for splitting -> "abcdFirstabcdSecondabcdThirdabcd"
    //                   01234567890123456789012345678901
    //                              1         2         3
    //                   ^   ^        ^         ^        ^
    //                   |   |        |         |        |
    // ptr --------------+---+--------+---------+--------+
    //                 s+0,0
    //                     s+4,5
    // modified word ->"\0bcdFirst\0bcdSecond\0bcdThird\0bcd"
    std::string_view line_to_process;
    std::vector<std::string_view> lines_to_process{};
    std::string_view strip_default {" \t\n\r"};
    std::string_view &for_stripping {strip_default};
    std::string_view join_default {""};
    std::string_view joining_word;
    char joined_words[MAX_JOINED_CHARACTERS];

    // This is "greedy"  split where it tries to match as much delimeter as possible
    // instead of just once. qs(",,,a,,b,,,c,,,").gsplit(",") will return
    // {"", a", "b", "c", ""}
    std::vector<std::string_view> &v_gsplit(const std::string_view delim) {
        int index = 0; // tracks where we are in the line to split
        split_words.clear();
        auto pdelim0 = delim.begin();
        const int delim_length = delim.length();
        int match = 0;
        const auto v_word_start = line_to_process.begin();
        int prev_index = 0;
        for (auto line_char : line_to_process) {
            // std::cout << line_char << *(pdelim0+match)<< " ";
            ++index;
            if (line_char == *(pdelim0+match)) {
                ++match;
                if (match == delim_length) {
                    // std::cout << "got a match, pushing" << std::basic_string_view(v_word_start+prev_index, index-prev_index-delim_length) << std::endl;
                    if ( !split_words.size() ) {
                        split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index-delim_length));
                    }
                    else {
                        if (delim_length != index-prev_index) {
                            // std::cout << "before inserting" << std::endl;
                            // for (auto xw : split_words) {
                                // std::cout << ">" << xw << "<" << std::endl;
                            // }
                            split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index-delim_length));
                            // std::cout << "after inserting" << std::endl;
                            // for (auto xw : split_words) {
                                // std::cout << ">" << xw << "<" << std::endl;
                            // }
                        }
                    }
                    prev_index = index;
                    match = 0;
                }
            }
            else {
                match = 0;
            }
        }
        // std::cout << "reached the end" << std::endl;
        // for (auto xw : split_words) {
        //     std::cout << ">" << xw << "<" << std::endl;
        // }
        // std::cout << "indeces" << index << " " << prev_index << " " << delim_length;
        if (prev_index != index) {
            split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index));
        }
        else {
            split_words.push_back(std::basic_string_view(v_word_start+prev_index, 0));
        }
        // std::cout << "final" << std::endl;
        // for (auto xw : split_words) {
        //     std::cout << ">" << xw << "<" << std::endl;
        // }
        return split_words;
    };

    std::vector<std::string_view> &v_wsplit() {
        int index = 0; // tracks where we are in the line to split
        constexpr std::string_view delim {" \t"};
        split_words.clear();
        int match = 0;
        const auto v_word_start = line_to_process.begin()-1;
        int prev_index = 0;
        for (auto line_char : line_to_process) {
            ++index;
            if (std::any_of(delim.begin(), delim.end(), [line_char](char x) {return x == line_char; })) {
                ++match;
                continue;
            }
            else {
                if (match) {
                    if (match != index-1) {
                        split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index-match));
                    }
                    prev_index = index;
                    match = 0;
                }
                else {
                    match = 0;
                }
            }
        }
        // std::cout << index << " " << prev_index << " " << delim_length;
        if ((index-prev_index) != match) {
            split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index));
        }
        return split_words;
    };


    std::vector<std::string_view> &v_split(const std::string_view delim) {
        int index = 0; // tracks where we are in the line to split
        split_words.clear();
        auto pdelim0 = delim.begin();
        const int delim_length = delim.length();
        int match = 0;
        const auto v_word_start = line_to_process.begin();
        int prev_index = 0;
        for (auto line_char : line_to_process) {
            // std::cout << line_char << *(pdelim0+match)<< " ";
            ++index;
            if (line_char == *(pdelim0+match)) {
                ++match;
                if (match == delim_length) {
                    split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index-delim_length));
                    prev_index = index;
                    match = 0;
                }
            }
            else {
                match = 0;
            }
        }
        // std::cout << index << " " << prev_index << " " << delim_length;
        if (prev_index == index) {
            split_words.push_back(std::basic_string_view(v_word_start+prev_index, 0));
        }
        else {
            split_words.push_back(std::basic_string_view(v_word_start+prev_index, index-prev_index));
        }
        return split_words;
    };

    bool isToBeTrimmed(const char c) const {
        for(const char w : for_stripping) {
            if(c == w)
                return true;
        }
        return false;
    };

    std::string_view v_strip() const {
        auto ppre = line_to_process.begin();
        auto ppost = line_to_process.end();
        if (ppre == ppost) {
            return std::string_view(ppre, 0);
        }
        // strip first from the front
        while ( ppre <= ppost && isToBeTrimmed(*ppre)) {
            ++ppre;
        }
        // Beware of situation where everything are to be stripped.
        // If so, stop!
        if (ppre == ppost) {
            --ppre;
            return std::string_view(ppre, 0);
        }
        // Then, strip coming from the end, going forward
        // Take note that end() is pointing AFTER the last
        // characted and therefore have to be moved one
        // character back so that it points to the last
        // character of the string before parsing backwards.
        --ppost;
        while ( ppre < ppost && isToBeTrimmed(*ppost)) {
            --ppost;
        }
        return std::string_view(ppre, ppost-ppre+1);
    };

    std::string v_join() {
        auto pChar = joined_words;
        int count = 0;
        int processed_elements = 0;
        const int no_elements = lines_to_process.size();
        for (const auto word : lines_to_process) {
            for (const auto c : word) {
                *pChar++ = c;
                ++count;
                if (count >= MAX_JOINED_CHARACTERS) {
                    break;
                }
            }
            if (count >= MAX_JOINED_CHARACTERS) {
                return std::string(joined_words, MAX_JOINED_CHARACTERS);
            }
            if (++processed_elements != no_elements) {
                for (const auto c : joining_word) {
                    *pChar++ = c;
                    ++count;
                    if (count >= MAX_JOINED_CHARACTERS) {
                        break;
                    }
                }
            }
            if (count >= MAX_JOINED_CHARACTERS) {
                return std::string(joined_words, MAX_JOINED_CHARACTERS);
            }
        }
        return std::string(joined_words, count);
    };

    public:
        Qstring() {};
        std::vector<std::string_view> split_words;

        Qstring &operator()(const char* line) {
            line_to_process = std::string_view(line, strlen(line));
            return *this;
        };

        Qstring &operator()(std::string_view line) {
            line_to_process = line;
            return *this;
        };

        Qstring &operator()(const std::vector<const char*> &lines) {
            lines_to_process.clear();
            for (auto const &line : lines) {
                lines_to_process.push_back(std::string_view{line});
            }
            return *this;
        };

        Qstring &operator()(const std::vector<std::string_view> &lines) {
            lines_to_process = lines;
            return *this;
        };

        Qstring &operator()(const std::vector<std::string> &lines) {
            lines_to_process.clear();
            for (const auto &line : lines) {
                lines_to_process.push_back(std::string_view{line});
            }
            return *this;
        };

        // ****************************************************************
        // ****************   split ***************************************
        // ****************************************************************
        std::vector<std::string_view> &gsplit() {
            return v_wsplit();
        };

        std::vector<std::string_view> &split(const char *delim=" ") {
            return v_split(std::basic_string_view(delim, strlen(delim)));
        };

        std::vector<std::string_view> &split(const std::string_view &delim) {
            return v_split(delim);
        };

        std::vector<std::string_view> &gsplit(const char *delim) {
            return v_gsplit(std::basic_string_view(delim, strlen(delim)));
        };

        std::vector<std::string_view> &gsplit(const std::string_view &delim) {
            return v_gsplit(delim);
        };

        // ****************************************************************
        // ****************   split chained *******************************
        // ****************************************************************
        Qstring &gsplitc() {
            lines_to_process = v_wsplit();
            return *this;
        };

        Qstring &splitc(const char *delim=" ") {
            lines_to_process = v_split(std::basic_string_view(delim, strlen(delim)));
            return *this;
        };

        Qstring &splitc(const std::string_view &delim) {
            lines_to_process = v_split(delim);
            return *this;
        };

        Qstring &gsplitc(const char *delim) {
            lines_to_process = v_gsplit(std::basic_string_view(delim, strlen(delim)));
            return *this;
        };

        Qstring &gsplitc(const std::string_view &delim) {
            lines_to_process = v_gsplit(delim);
            return *this;
        };


        // ****************************************************************
        // ****************   strip ***************************************
        // ****************************************************************

        std::string_view strip() {
            for_stripping = strip_default;
            return v_strip();
        };

        std::string_view strip(const char *to_be_stripped_out) {
            for_stripping = std::string_view(to_be_stripped_out, strlen(to_be_stripped_out));
            return v_strip();
        };

        std::string_view strip(const std::string_view &to_be_stripped_out) {
            for_stripping = to_be_stripped_out;
            return v_strip();
        };


        // ****************************************************************
        // ****************   strip chained  ******************************
        // ****************************************************************

        Qstring &stripc() {
            for_stripping = strip_default;
            line_to_process = v_strip();
            return *this;
        };

        Qstring &stripc(const char *to_be_stripped_out) {
            for_stripping = std::string_view(to_be_stripped_out, strlen(to_be_stripped_out));
            line_to_process = v_strip();
            return *this;
        };

        Qstring &stripc(const std::string_view &to_be_stripped_out) {
            for_stripping = to_be_stripped_out;
            line_to_process = v_strip();
            return *this;
        };

        // ****************************************************************
        // ****************   join  ***************************************
        // ****************************************************************

        std::string join() {
            joining_word = join_default;
            return v_join();
        };

        std::string join(const char *to_be_joined) {
            joining_word = std::string_view(to_be_joined, strlen(to_be_joined));
            return v_join();
        };

        std::string join(const std::string_view &to_be_joined) {
            joining_word = to_be_joined;
            return v_join();
        };

        // ****************************************************************
        // ****************   join chained ********************************
        // ****************************************************************

        Qstring &joinc() {
            joining_word = join_default;
            line_to_process = v_join();
            return *this;
        };

        Qstring &joinc(const char *to_be_joined) {
            joining_word = std::string_view(to_be_joined, strlen(to_be_joined));
            line_to_process = v_join();
            return *this;
        };

        Qstring &joinc(const std::string_view &to_be_joined) {
            joining_word = to_be_joined;
            line_to_process = v_join();
            return *this;
        };
};
