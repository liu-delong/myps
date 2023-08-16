#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unistd.h>

// 定义进程信息的结构体
struct ProcessInfo {
    int pid;
    int memory;
};

// 获取进程信息并保存在map中
std::unordered_map<int, ProcessInfo> getProcessInfo() {
    std::unordered_map<int, ProcessInfo> processMap;

    // 执行ps命令并读取输出
    FILE* pipe = popen("ps -eo pid,rss", "r");
    if (!pipe) {
        std::cout << "Error executing ps command\n";
        return processMap;
    }

    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);  // 忽略表头

    // 读取每行进程信息并保存到map中
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::stringstream ss(buffer);
        int pid, memory;
        ss >> pid >> memory;

        ProcessInfo processInfo;
        processInfo.pid = pid;
        processInfo.memory = memory;

        processMap[pid] = processInfo;
    }

    pclose(pipe);

    return processMap;
}

int main() {
    while (true) {
        std::unordered_map<int, ProcessInfo> processMap = getProcessInfo();

        // 清空终端
        std::cout << "\033c";
        std::cout<<"hello"<<std::endl;
        // 输出进程信息
        for (const auto& kvp : processMap) {
            int pid = kvp.first;
            const ProcessInfo& processInfo = kvp.second;

            std::cout << "PID: " << pid << ", Memory: " << processInfo.memory << " KB\n";
        }

        // 休眠1秒
        sleep(1);
    }

    return 0;
}