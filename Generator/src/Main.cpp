#include "pch.h"

namespace fs = std::filesystem;

#ifdef _DEBUG
#define DLOG(x) std::cout << x << std::endl
#else
#define DLOG(x) 
#endif
#define LOG(x) std::cout << x << std::endl

static inline std::string ToUpper(const std::string& string)
{
	std::string upper = "";
	for (auto& c : string)
		upper += toupper(c);
	return upper;
}

static inline std::string ToLower(const std::string& string)
{
	std::string upper = "";
	for (auto& c : string)
		upper += tolower(c);
	return upper;
}

namespace Names
{
	std::string SolutionParseSimbol = "##"; // Simble the program will use to replace with information
	std::string SolutionParseCapsSimbol = "&&";
	std::string ProjectParseSimbol = "$$"; // Simble the program will use to replace with information
	std::string ProjectParseCapsSimbol = "%%";
	std::string Extension = "txt";
	std::string SolutionTemplateFilePath = std::string("templates\\SolutionTemplate.") + Extension;
	std::string SolutionTemplateFileName = "SolutionTemplate";
	std::string ProjectTemplateFilePath = std::string("templates\\ProjectTemplate.") + Extension;
	std::string ProjectTemplateFileName = "ProjectTemplate";
	std::string ProjectDependenciesFilePath = std::string("templates\\Dependencies.") + Extension;
	std::string ProjectDependenciesFileName = "Dependencies";
}

std::string SolutionName = "";
std::string ProjectName = "";

std::string ParsedSolution = "";
std::string ParsedProject = "";
std::string ParsedDependencies = "";

static std::string ParseTokens(const std::string& file);

static void OutputSolution();
static void OutputProject();
static void OutputDependencies();

static inline std::string CurrentLocation()
{
	return fs::current_path().string();
}

int main(int argc, char** argv)
{
	if(argc)
		DLOG("Executable location: " << argv[0]);
	DLOG("Execution location: " << CurrentLocation());

	std::cout << "Solution Name: ";
	std::getline(std::cin, SolutionName);
	std::cout << "\nProject Name: ";
	std::getline(std::cin, ProjectName);

	ParsedSolution = ParseTokens(Names::SolutionTemplateFilePath);
	ParsedProject = ParseTokens(Names::ProjectTemplateFilePath);
	ParsedDependencies = ParseTokens(Names::ProjectDependenciesFilePath);

	DLOG("");
	DLOG("===== Solution Parsed =====");
	DLOG(ParsedSolution);
	DLOG("");
	DLOG("===== Project Parsed =====");
	DLOG(ParsedProject);
	DLOG("");
	DLOG("===== Dependencies Parsed =====");
	DLOG(ParsedDependencies);

	if (ParsedSolution.empty() || ParsedProject.empty() || ParsedDependencies.empty()) exit(1);

	DLOG("Outputing Solution");
	OutputSolution();
	DLOG("");
	DLOG("Outputing Project");
	OutputProject();
	DLOG("");
	DLOG("Outputing Dependencies");
	OutputDependencies();
}

static inline void ParseLine(std::string& line, const std::string& simbol, const std::string& name)
{
	int index = 0;
	while ((index = line.find(simbol)) != std::string::npos)
		line.replace(index, simbol.size(), name);
}

static std::string ParseTokens(const std::string& file)
{
	std::string line;
	std::ifstream stream(file);
	std::stringstream final_stream;
	while (getline(stream, line))
	{
		// {
		// 	auto tkSolution = line.find(Names::SolutionParseSimbol);
		// 	if (tkSolution != std::string::npos)
		// 		line.replace(tkSolution, Names::SolutionParseSimbol.size(), SolutionName);
		// 	
		// 	auto tkCapsSolution = line.find(Names::SolutionParseCapsSimbol);
		// 	if (tkCapsSolution != std::string::npos)
		// 		line.replace(tkCapsSolution, Names::SolutionParseCapsSimbol.size(), ToUpper(SolutionName));
		// }
		// 
		// {
		// 	auto tkProject = line.find(Names::ProjectParseSimbol);
		// 	if (tkProject != std::string::npos)
		// 		line.replace(tkProject, Names::ProjectParseSimbol.size(), ProjectName);
		// 
		// 	auto tkCapsProject = line.find(Names::ProjectParseCapsSimbol);
		// 	if (tkCapsProject != std::string::npos)
		// 		line.replace(tkCapsProject, Names::ProjectParseCapsSimbol.size(), ToUpper(ProjectName));
		// }
	
		ParseLine(line, Names::SolutionParseSimbol, SolutionName);
		ParseLine(line, Names::SolutionParseCapsSimbol, ToUpper(SolutionName));

		ParseLine(line, Names::ProjectParseSimbol, ProjectName);
		ParseLine(line, Names::ProjectParseCapsSimbol, ToUpper(ProjectName));

		final_stream << line << '\n';
	}
	return final_stream.str();

}

static void OutputSolution()
{
	const std::string directory = CurrentLocation() + "/" + SolutionName;
	const std::string filePath = directory + "/" + "premake5.lua";

	DLOG("");
	DLOG("Directory: " << directory);
	DLOG("File Path: " << filePath);

	fs::create_directory(directory);
	std::ofstream file(filePath);
	file << ParsedSolution;
	file.close();
}

static void OutputProject()
{
	const std::string directory = CurrentLocation() + "/" + SolutionName + "/" + ProjectName;
	const std::string sourceDir = directory + "/src";
	const std::string filePath = directory + "/" + "premake5.lua";

	DLOG("");
	DLOG("Directory: " << directory);
	DLOG("Source Dir: " << sourceDir);
	DLOG("File Path: " << filePath);

	fs::create_directory(directory);
	fs::create_directory(sourceDir);
	std::ofstream file(filePath);
	file << ParsedProject;
	file.close();
}

static void OutputDependencies()
{
	const std::string directory = CurrentLocation() + "/" + SolutionName;
	const std::string filePath = directory + "/" + "Dependencies.lua";

	DLOG("");
	DLOG("Directory: " << directory);
	DLOG("File Path: " << filePath);

	fs::create_directory(directory);
	std::ofstream file(filePath);
	file << ParsedDependencies;
	file.close();
}
