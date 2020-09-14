#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

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
	std::cout << "Executing..." << std::endl;
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
			cycles++;
			if(debug)
			{
				for(int i = 0; i < 10; i++)
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
				getchar();
			}
		}
		ip++;
	}
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Usage: ./blf <source file> [-D]" << std::endl;
		return -1;
	}
	bool debug = (argc == 3 && (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "-D") == 0));
	std::cout << "Loading Brainf*ck source from '" << argv[1] << "'..." << std::endl;
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
	std::cout << "Program terminated." << std::endl;
}
