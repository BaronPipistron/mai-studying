#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::string mode;
    std::cin >> mode;

    if (mode == "compress") {
        std::unordered_map<std::string, int> dict;
        int index = 0;

        {
            char c = 'a';
            
            for (; index != 26; ++index, ++c) {
                std::string s = "";
                s += c;
                dict[s] = index;
            }

            dict[std::to_string(EOF)] = index;
        }
        
        std::string input_str;
        std::cin >> input_str;

        std::string current_word = "";
        std::string word_with_next_char = "";

        current_word += input_str[0];

        for (size_t i = 0; i != input_str.length(); ++i) {
            if (i != input_str.length() - 1) {
                word_with_next_char = current_word + input_str[i + 1];
            } else {
                word_with_next_char += EOF;
            }

            if (dict.find(word_with_next_char) != dict.end()) {
                current_word = word_with_next_char;
            } else {
                std::cout << dict[current_word] << ' ';
                dict[word_with_next_char] = (++index);

                current_word = word_with_next_char.back();
                word_with_next_char = "";
            }
        }

        std::cout << dict[std::to_string(EOF)] << std::endl;
    } else if (mode == "decompress") {
        std::unordered_map<int, std::string> dict;
        int index = 0;

        {
            char c = 'a';
            
            for (; index != 26; ++index, ++c) {
                std::string s = "";
                s += c;
                dict[index] = s;
            }

            dict[index] = std::to_string(EOF);
        }

        std::string decoded_word = "";

        std::string last_decoded_word = "";
        std::string current_decoded_word = "";

        for (int code; std::cin >> code; ) {
            if (dict.find(code) == dict.end()) {
                dict[++index] = (last_decoded_word + last_decoded_word[0]);
            } else {
                current_decoded_word = dict[code];

                if (last_decoded_word == "") {
                    last_decoded_word = current_decoded_word;
                } else {
                    dict[++index] = (last_decoded_word + current_decoded_word[0]);
                }
            }

            last_decoded_word = current_decoded_word;
            decoded_word += dict[code];
        }
        decoded_word.pop_back();
        decoded_word.pop_back();

        std::cout << decoded_word << std::endl;
    }

    return 0;
}
