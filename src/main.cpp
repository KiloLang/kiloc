
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct ParsedArgs
{
    std::unordered_map<std::string, std::string> options;
    std::vector<std::string> flags;
    std::vector<std::string> positional;
};

ParsedArgs parse_args(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    ParsedArgs result;
    
    for (unsigned int i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (arg.size() >= 2 && arg[0] == '-' && arg[1] == '-') {
            std::string body = arg.substr(2);
            auto eq = body.find('=');
            if (eq != std::string::npos) {
                result.options[body.substr(0, eq)] = body.substr(eq + 1);
            } else {
                result.flags.push_back(body);
            }
        }
        else if (arg.size() >= 2 && arg[0] == '-') {
            char opt = arg[1];

            if (arg.size() > 2) {
                result.options[std::string(1, opt)] = arg.substr(2);
            }
            else if (opt == 'o' && i + 1 < args.size()) {
                result.options["o"] = args[++i];
            } else {
                result.flags.push_back(std::string(1, opt));
            }
        } else {
            result.positional.push_back(arg);
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    ParsedArgs args = parse_args(argc, argv);

    if (auto it = args.options.find("o"); it != args.options.end()) {
        std::cout << "Output file: " << it->second << std::endl;
    }

    if (auto it = args.options.find("sh"); it != args.options.end()) {
        bool running = true;
        do {
            std::string input = "";

            std::cout << "kilo > ";
            std::getline(std::cin, input);

            std::cout << input << std::endl;
        } while(running);
    }

    bool should_use_shell = false;
    for (const auto& f : args.flags) {
        std::cout << "Flags: " << f << std::endl;
        if (f == "S") {
            should_use_shell = true;
        }
    }

    //for (const auto& p : args.positional) {
    //    std::cout << "Positional: " << p << std::endl;
    //}

    return 0;
}
