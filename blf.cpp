#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

template <typename T>
T clamp(const T& n, const T& lower, const T& upper)
{
  return std::max(lower, std::min(n, upper));
}

bool isExecutable(char character)
{
	const char* executable = "<>-+,.[]";
	bool exec = false;
	for(int i = 0; i < 8; i++) if(character == executable[i]) exec = true;
	return exec;
}

void execute(std::string program, bool debug)
{
	// Initialize memory
	const long MEMORY_SIZE = 30000;
	typedef unsigned char cell_t;
	cell_t* dp = (cell_t*) malloc(MEMORY_SIZE * sizeof(cell_t));
	cell_t* mp = dp;
	memset(dp, 0, MEMORY_SIZE);
	std::vector<int> loopStack;
	int cycles = 0, ip = 0;
	// Execute program
	while(ip >= 0 && ip < (int) program.length())
	{
		if(isExecutable(program.at(ip)))
		{
			if(debug) std::cout << "#" << cycles << " ip: " << ip << " := " << program.at(ip) << std::endl;
			switch(program.at(ip))
			{
				case '<': dp--; break;
				case '>': dp++; break;
				case '-': (*dp)--; break;
				case '+': (*dp)++; break;
				case ',': *dp = getchar(); break;
				case '.': putchar(*dp); break;
				case '[': 
					if(*dp != 0) loopStack.push_back(ip);
					else 
					{
						int brackets = 0;
						do
						{
							if(program.at(ip) == '[') brackets++;
							else if(program.at(ip) == ']') brackets--;
						} while(brackets != 0 && ip++ < (int) program.length());
					}
					break;
				case ']': 
					if(*dp != 0) ip = loopStack.at(loopStack.size() - 1);
					else loopStack.pop_back();
					break;
			}
			if((dp - mp) < 0 || (dp - mp) >= MEMORY_SIZE)
			{
				std::cout << std::endl << "DP (" << (dp - mp) << ") out of bounds. IP = " << ip << std::endl;
				int begin = clamp<int>(ip - 20, 0, MEMORY_SIZE - 1);
				std::cout << program.substr(begin, 40) << std::endl << std::string(20, ' ') << "^" << std::endl;
				break;
			}
			cycles++;
			if(debug)
			{
				for(int i = 0; i < 20; i++)
				{
					int num = (int) mp[i];
					int temp = num, digits = 0;
					while(temp != 0)
					{	
						temp /= 10;
						digits++;
					}
					if(num == 0) digits = 1;
					for(int i = 0; i < (4 - digits); i++) std::cout << ".";
					std::cout << (int) mp[i] << "|";
				}
				std::cout << std::endl;
				for(int i = 0; i < (dp - mp); i++) std::cout << "     ";
				std::cout << "  ^" << std::endl;
			}
		}
		ip++;
	}
	if(ip < 0)
		std::cout << std::endl << "IP (" << ip << ") out of bounds (lengh = " << program.length() << ")" << std::endl;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Usage: ./blf <source file> [-D]" << std::endl;
		return -1;
	}
	bool debug = (argc == 3 && (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "-D") == 0));
	std::ifstream inputFile(argv[1]);
	if(inputFile.fail())
	{
		std::cout << "Error opening '" << argv[1] << "'." << std::endl;
		return -1;
	}
	std::vector<char> program;
	char temp;
	while(inputFile.get(temp)) program.push_back(temp);
	if(debug) std::cout << "Utilizing Debug mode." << std::endl;
	execute(std::string(program.begin(), program.end()), debug);
}
