#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>

#define INT64 unsigned long long

#ifdef __WIN32__
#define PREFEX '\\'
#else
#define PREFEX '/'
#endif

std::mutex m;

bool matchPattern(const std::string& file, const std::string& pattern) {
  if (pattern.size() > 2 && pattern[0] == '*' && pattern[1] == '.') {
    std::string ext = pattern.substr(1);
    if (file.size() >= ext.size() &&
      file.compare(file.size() - ext.size(), ext.size(), ext) == 0)
      return true;
  }
  return false;
}

bool ignored(const std::filesystem::path& p, const std::vector<std::string>& ignoredList) {
  std::string name = p.filename().string();

  for (const auto& x : ignoredList) {
    if (name == x) return true;  
    if (matchPattern(name, x)) return true; 
  }
  return false;
}

INT64 countLines(const std::string& path) {
  INT64 lines = 0;
  std::ifstream file(path);
  std::string s;
  while (std::getline(file, s)) lines++;
  return lines;
}

void worker(const std::vector<std::filesystem::path>& files,
  std::unordered_map<std::string, INT64>& result)
{
  std::unordered_map<std::string, INT64> local;
  for (const auto& f : files) {
    INT64 lines = countLines(f.string());
    local[f.extension().string()] += lines;
    local["total"] += lines;
  }
  std::lock_guard<std::mutex> lock(m);
  for (auto& p : local) result[p.first] += p.second;
}

std::unordered_map<std::string, INT64> getAll(const std::string& start,
  const std::vector<std::string>& ignore)
{
  std::vector<std::filesystem::path> files;

  for (auto it = std::filesystem::recursive_directory_iterator(
    start, std::filesystem::directory_options::skip_permission_denied);
    it != std::filesystem::recursive_directory_iterator();
    ++it)
  {
    auto& f = *it;
    if (ignored(f.path(), ignore)) {
      if (f.is_directory()) {
        it.disable_recursion_pending();
      }
      continue;
    }

    if (f.is_regular_file()) {
      files.push_back(f.path());
    }
  }

  unsigned int threadCount = std::thread::hardware_concurrency();
  if (threadCount == 0) threadCount = 4;

  std::vector<std::thread> threads;
  std::unordered_map<std::string, INT64> result;

  size_t chunk = files.size() / threadCount;
  size_t startIndex = 0;

  for (unsigned int i = 0; i < threadCount; i++) {
    size_t endIndex = (i == threadCount - 1) ? files.size() : startIndex + chunk;
    std::vector<std::filesystem::path> part(files.begin() + startIndex,
      files.begin() + endIndex);
    threads.emplace_back(worker, part, std::ref(result));
    startIndex = endIndex;
  }

  for (auto& t : threads) t.join();

  return result;
}


std::vector<std::string> parseConfigFile(const std::string& path) {
  std::vector<std::string> out;
  std::ifstream file(path);
  std::string line;
  while (std::getline(file, line)) out.push_back(line);
  return out;
}

int main() {
  std::string start = std::filesystem::current_path().string();
  std::vector<std::string> ignore;

  std::string cfg = start + PREFEX + ".ignorefiles";
  if (std::filesystem::exists(cfg))
    ignore = parseConfigFile(cfg);

  auto lines = getAll(start, ignore);

  std::cout << "Total Lines : " << lines["total"] << std::endl;

  for (const auto& p : lines) {
    if (p.first == "total") continue;
    double percent = (double)p.second / (double)lines["total"];
    std::cout << "%" << p.first << " : " << (percent * 100) << " | " << p.second << " line" << std::endl;
  }
}

