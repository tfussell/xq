#include <cstdio>

// Platform-specific includes
#ifdef _WIN32
#include <fcntl.h> // For O_BINARY
#include <io.h> // For _setmode, _fileno, _isatty
#else // POSIX
#include <unistd.h> // For fileno, isatty
#endif

#include <xlnt/xlnt.hpp>

namespace {

// Returns true if program is being run directly
// If this is false, we assume that input will be given through stdin
bool is_tty()
{
#ifdef _WIN32
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(fileno(stdin)) != 0;
#endif
}

} // namespace

// Either 1. Print sheet names of XLSX workbook given as filename or piped through stdin
// or 2. Print all cells of worksheet in aforementioned workbook
int main(int argc, char *argv[])
{
    xlnt::workbook wb;
    std::vector<std::string> args;
    
    for(int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    
    if(is_tty()) // Expect first argument to be XLSX file path
    {
        if(args.size() < 1)
        {
            std::cout << "usage: " << argv[0] << " xlsx_file [sheet_to_print]" << std::endl;
            std::cout << "    (xlsx_file can be replaced by data piped from standard input)" << std::endl;
            return 1;
        }

        wb.load(args.front());
        args.erase(args.begin());
    }
    else
    {
#ifdef _WIN32
        // Ensure that we are reading the raw binary data when using a pipe in Windows.
        _setmode(_fileno(stdin), O_BINARY);
#endif
        wb.load(std::cin);
    }
    
    if(args.empty())
    {
	for(auto ws : wb)
	{
	    std::cout << ws.get_title() << " ";
	}

	std::cout << std::endl;
    }
    else
    {
	auto ws = wb.get_sheet_by_name(args.front());
	
	for(auto row : ws.rows())
	{
	    for(auto cell : row)
	    {
		std::cout << cell << "| ";
	    }

	    std::cout << std::endl;
	}
    }
    
    return 0;
}
