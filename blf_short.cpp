#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Usage: ./blf <source file>" << std::endl;
		return -1;
	}
	std::ifstream inputFile(argv[1]);
	if(inputFile.fail())
	{
		std::cout << "Error opening '" << argv[1] << "'." << std::endl;
		return -1;
	}
	std::vector<char> raw;
	char temp;
	while(inputFile.get(temp)) raw.push_back(temp);
	std::string program(raw.begin(), raw.end());
	unsigned char* dp = (unsigned char*) malloc(30000 * sizeof(unsigned char));
	memset(dp, 0, 30000);
	std::vector<int> loopStack;
	int ip = 0;
	while(ip >= 0 && ip < (int) program.length())
	{
		const char* executable = "<>-+,.[]";
		bool exec = false;
		for(int i = 0; i < 8; i++) if(program.at(ip) == executable[i]) exec = true;
		if(exec)
		{
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
		}
		ip++;
	}
}
