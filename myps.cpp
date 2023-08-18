#include <iostream>
#include <sstream>
#include <unordered_map>
#include<vector>
#include<set>
#include <unistd.h>
#include<list>
using namespace std;
// 定义进程信息的结构体
struct PsInfo
{
    /* data */
    string user;
    int pid;
    int ppid;
    long long int vsz;
    long long int rss;
    string wchan;
    string addr;
    string s;
    string name;
};

class ProcessInfo {
    public:
    PsInfo psInfo;
    int enter_epoch;   //这个pid是在哪一轮进入的。
    int last_renew_epoch;  //这个pid最后一次被更新的epoch。
    operator PsInfo()
    {
        return psInfo;
    }
    ProcessInfo(const PsInfo& p)
    {
        psInfo=p;
        enter_epoch=0;
        last_renew_epoch=0;
    }
    ProcessInfo operator=(const PsInfo& p)
    {
        psInfo=p;
    }
};
std::list<ProcessInfo> processInfoList;
unordered_map<int,decltype(processInfoList.begin())> processInfoListPid;
long long int epoch=0;
long long int processCnt=0;
unsigned long long int totalRss=0;
bool sort_logic(const ProcessInfo& a,const ProcessInfo& b)
{
    bool a_new=(a.enter_epoch==epoch?true:false);
    bool b_new=(b.enter_epoch==epoch?true:false);
    if(a_new&&!b_new) return true;  //a需要排在b的前面
    else if(!a_new&&b_new) return false; //a不应该排在b的前面
    else  //都不是新的或者都是新的
    {
        if(a.enter_epoch==b.enter_epoch)
        {
            return a.psInfo.pid<b.psInfo.pid;
        }
        else
        {
            return a.enter_epoch<b.enter_epoch;
        }
    }
}
bool getProcessInfo()
{
    // 执行ps命令并读取输出
    FILE* pipe = popen("ps -A", "r");
    if (!pipe) {
        std::cout << "Error executing ps command\n";
        return false;
    }
    char buffer[512];
    fgets(buffer, sizeof(buffer), pipe);  // 忽略表头
    // 读取每行进程信息并保存到map中
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::stringstream ss(buffer);
        PsInfo psInfo;
        ss>>psInfo.user>>psInfo.pid>>psInfo.ppid>>psInfo.vsz>>psInfo.rss>>psInfo.wchan>>psInfo.addr>>psInfo.s>>psInfo.name;
        if(psInfo.name=="ps") continue;
        auto pidinfoptr=processInfoListPid.find(psInfo.pid);
        if(pidinfoptr==processInfoListPid.end())
        {
            processInfoList.push_back(psInfo);
            processInfoList.back().enter_epoch=epoch;
            processInfoList.back().last_renew_epoch=epoch;
        }
        else
        {
            auto& processInfo=*processInfoListPid[psInfo.pid];
            processInfo.psInfo=psInfo;
            processInfo.last_renew_epoch=epoch;
        }
    }
    pclose(pipe);
    return true;
}
bool organizeAndSummarize()
{
    for(auto iter=processInfoList.begin();iter!=processInfoList.end();)
    {
        const auto& info=*iter;
        
        return true;
    }
    return true;
}
bool sortProcessInfo()
{
    return true;
}
bool buildIndex()
{
    return true;
}

int main() {
    while (true) {
        getProcessInfo();
        organizeAndSummarize();
        

        // 清空终端
        std::cout << "\033c";
        std::cout<<"hello"<<std::endl;
        // 输出进程信息
        for (const auto&  psInfo : processInfoList) {
            std::cout << "PID: " << psInfo.pid <<"\t"<<psInfo.name<<endl;
        }

        // 休眠1秒
        sleep(1);
        epoch++;
    }

    return 0;
}