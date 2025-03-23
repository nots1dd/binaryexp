#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#define GIVE_NAME_ENV "GIVE_NAME"

std::vector<std::string> allowed_commands = {"ls",     "pwd", "lsblk", "whoami", "clear",
                                             "export", "run", "pushd", "dirs",   "popd"};

std::vector<std::string> dir_stack; // Manually track directories

void restricted_exec(const std::string& input)
{
  std::istringstream       iss(input);
  std::vector<std::string> args;
  std::string              word;

  while (iss >> word)
  {
    args.push_back(word);
  }

  if (args.empty())
  {
    return;
  }

  const std::string& command = args[0];

  // Check if command is allowed
  if (std::find(allowed_commands.begin(), allowed_commands.end(), command) ==
      allowed_commands.end())
  {
    std::cout << "Command '" << command << "' is not allowed.\n";
    return;
  }

  if (command == "run")
  {
    char* cmd = std::getenv(GIVE_NAME_ENV);
    std::cout << "Debug: GIVE_NAME environment variable: " << (cmd ? cmd : "NULL") << "\n";
    if (cmd && strlen(cmd) > 0)
    {
      std::cout << "Your name is " << cmd << "\n";
      std::string cat_command = "cat " + std::string(cmd);
      system(cat_command.c_str());
    }
    else
    {
      std::cout << "GIVE_NAME environment variable is not set.\n";
    }
  }
  else if (command == "pushd")
  {
    if (args.size() < 2)
    {
      std::cout << "Usage: pushd <directory>\n";
      return;
    }

    std::string new_dir = args[1];

    if (chdir(new_dir.c_str()) == 0)
    {
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)))
      {
        dir_stack.push_back(cwd);
        std::cout << "Directory changed to " << cwd << "\n";
      }
    }
    else
    {
      perror("pushd failed");
    }
  }
  else if (command == "dirs")
  {
    if (dir_stack.empty())
    {
      std::cout << "Directory stack is empty.\n";
    }
    else
    {
      std::cout << "Directory stack:\n";
      for (const auto& dir : dir_stack)
      {
        std::cout << dir << "\n";
      }
    }
  }

  else if (command == "export" && args.size() > 1)
  {
    std::string export_arg = args[1]; // Example: GIVE_NAME=flag.txt

    size_t pos = export_arg.find('=');
    if (pos != std::string::npos)
    {
      std::string key   = export_arg.substr(0, pos);
      std::string value = export_arg.substr(pos + 1);

      if (key == GIVE_NAME_ENV)
      {
        setenv(GIVE_NAME_ENV, value.c_str(), 1);
        std::cout << "Set " << key << "=" << value << "\n";
      }
    }
    return;
  }

  else if (command == "popd")
  {
    if (dir_stack.empty())
    {
      std::cout << "Directory stack is empty.\n";
      return;
    }

    std::string last_dir = dir_stack.back();
    dir_stack.pop_back();

    if (chdir(last_dir.c_str()) == 0)
    {
      std::cout << "Returned to directory " << last_dir << "\n";
    }
    else
    {
      perror("popd failed");
    }
  }
  else
  {
    std::string full_command = input;
    system(full_command.c_str());
  }
}

void print_banner()
{
  std::cout << "You are in a restricted shell.\n";
  std::cout << "Allowed commands: ";
  for (const auto& cmd : allowed_commands)
  {
    std::cout << cmd << " ";
  }
  std::cout << std::endl;
}

auto main() -> int
{
  // Remove GIVE_NAME environment variable if it exists
  unsetenv(GIVE_NAME_ENV);

  print_banner();

  while (true)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    if (std::cin.eof() || input == "exit")
    {
      std::cout << "Exiting restricted shell.\n";
      break;
    }

    restricted_exec(input);
  }

  return 0;
}
