#include <iostream>
#include <sstream>
#include <unordered_map>
#include<vector>
#include<set>
#include <unistd.h>
#include<list>
#include<algorithm>
#include<dirent.h>
#include<thread>
#include<unordered_set>
using namespace std;
#define DEBUG(...) printf(__VA_ARGS__);
#define ERROR(...) printf(__VA_ARGS__);
#define clearline std::cout << "\033[2K";
// 定义进程信息的结构体
struct PsInfo
{
    /* data */
    string user;
    int pid=0;
    int ppid=0;
    long long int vsz=0;
    long long int rss=0;
    long long int shrss=0;
    long long int filerss=0;
    long long int anonrss=0;
    long long int VmSwap=0;
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
    long long int enter_rss;
    long long int enter_shrss;
    long long int enter_filerss;
    long long int enter_anonrss;
    int increased_shrss;
    int increased_filerss;
    int increased_anonrss;
    int increased_rss;
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
        return *this;
    }
};
int displayBegin=0;
int displayCnt=15;
string fliter="";
vector<string> fliter_set;
string erase_text="";
vector<string> erase_set;
int debug_pid;
bool tag=false;
bool need_print=true;
void mySplit(string text,vector<string>& sset)
{
    size_t pos=0;
    while((pos=text.find("|"))!=text.npos)
    {
        sset.push_back(text.substr(0,pos));
        text=text.substr(pos+1);
    }
    sset.push_back(text);
}
void control_func()
{
    char op;
    while(cin>>op)
    {
        if(op=='\033')
        {
            need_print=false;
            cout<<"\033[1;1H";
            clearline cout<<":";
            char op2;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin>>op2;
            if(op2=='C')
            {
                cin>>displayCnt;
            }
            else if(op2=='F')
            {
                cin>>fliter;
                mySplit(fliter,fliter_set);
            }
            else if(op2=='I')
            {
                cin>>erase_text;
                mySplit(erase_text,erase_set);
            }
            else if(op2=='W')
            {
                displayBegin--;
                if(displayBegin<0) displayBegin=0;
            }
            else if(op2=='S')
            {
                displayBegin++;
            }
            else if(op2=='T')
            {
                tag=true;
            }
            else if(op2=='G')
            {
                fliter="";
                fliter_set.clear();
            }
            else if(op2=='D')
            {
                cin>>debug_pid;
            }
            for(int i=0;i<displayCnt;i++)
            {
                printf("\033[%d;1H",i+5);
                cout<<"\033[2K";
            }
            need_print=true;
        }
        
    }
}
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
// 通过ps获取进程信息，保存到processInfoList中（全局变量,vector)。
// 同时更新引索，对于本次新增的进程，把它的<pid,在vector中的位置>插到processInfoListPid(全局变量，unordered_map)中。\
     对于旧的进程。通过processInfoListPid以及pid，找到这个进程在vector中的位置。然后更新进程信息。
// 这个函数的下一个过程是organizeAndSummarize，这个过程将删除过期的进程，统计总体信息。
// 再下一个过程sortProcessInfo，这个过程会对processInfoList进行排序，并重建processInfoListPid
// 再下一个过程就是输出了。
bool isnumber(string a)
{
    for(int i=0;i<a.size();i++)
    {
        if(!(a[i]>='0'&&a[i]<='9'))
        {
            return false;
        }
    }
    return true;
}
bool getProcessInfo2()
{
    DIR* dir;
    dirent* entry;
    dir=opendir("/proc");
    if(dir==nullptr)
    {
        ERROR("open /proc error!\n")
        return false;
    }
    while((entry=readdir(dir))!=nullptr)
    {
        if(entry->d_type==DT_DIR&&isnumber(entry->d_name))
        {
            char fileName[50];
            sprintf(fileName,"/proc/%s/status",entry->d_name);
            FILE* file=fopen(fileName,"r");
            
            if(file!=NULL)
            {   
                bool nameFlag=false;
                char name[100];
                for(int i=0;i<100;i++) name[i]=0;
                PsInfo psInfo;
                char fileName2[30];
                //先从cmdline中读进程名，如果进程名为空，则再从status中读进程名。
                sprintf(fileName2,"/proc/%s/cmdline",entry->d_name);
                FILE* file2=fopen(fileName2,"r"); 
                if(file2!=nullptr)
                {
                    fgets(name,99,file2);
                    psInfo.name=name;
                    if(psInfo.name=="")
                    {
                        nameFlag=true;   
                    }
                    fclose(file2);
                }     
                else
                {
                    nameFlag=true;
                }
                
                char context[512];
                
                while(fgets(context,511,file)!=nullptr)
                {
                        char name2[100];
                        if(nameFlag&&sscanf(context,"Name: %s",name2)==1)
                        {
                            psInfo.name=name2;
                            continue;
                        }
                        if(sscanf(context,"Pid: %d",&psInfo.pid)==1) 
                        {
                            continue;
                        }
                        else if(sscanf(context,"VmSize: %lld KB",&psInfo.vsz)==1)
                        {
                            continue;
                        }
                        else if(sscanf(context,"RssAnon:  %lld KB",&psInfo.anonrss)==1)
                        {
                            continue;
                        }
                        else if(sscanf(context,"RssFile:  %lld KB",&psInfo.filerss)==1) 
                        {
                            continue;
                        }
                        else if(sscanf(context,"RssShmem:  %lld KB",&psInfo.shrss)==1) 
                        {
                            continue;
                        }
                        else if(sscanf(context,"VmRSS:  %lld KB",&psInfo.rss)==1)
                        {
                            continue;
                        } 
                        else if(sscanf(context,"VmSwap: %lld KB",&psInfo.VmSwap))
                        {
                            continue;
                        }    
                }
                    auto pidinfoptr=processInfoListPid.find(psInfo.pid);
                    if(pidinfoptr==processInfoListPid.end())
                    {
                        processInfoList.push_back(psInfo);
                        processInfoList.back().enter_epoch=epoch;
                        processInfoList.back().last_renew_epoch=epoch;
                        processInfoList.back().increased_rss=0;
                        processInfoList.back().increased_anonrss=0;
                        processInfoList.back().increased_filerss=0;
                        processInfoList.back().increased_shrss=0;
                        processInfoList.back().enter_anonrss=psInfo.anonrss;
                        processInfoList.back().enter_filerss=psInfo.filerss;
                        processInfoList.back().enter_shrss=psInfo.shrss;
                        processInfoList.back().enter_rss=psInfo.rss;
                    }
                    else
                    {
                        
                        auto& processInfo=*processInfoListPid[psInfo.pid];    //processInfoListPid保存了进程信息的索引，键为pid，值为一个指向进程信息内容的迭代器。
                        processInfo.psInfo=psInfo;
                        processInfo.last_renew_epoch=epoch;
                    }
                    fclose(file);
                }
                else
                {
                    //ERROR("%s open fail\n",fileName)
                }
        }
    }
    closedir(dir);
    return true;
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
            processInfoList.back().enter_rss=psInfo.rss;
        }
        else
        {
            auto& processInfo=*processInfoListPid[psInfo.pid];    //processInfoListPid保存了进程信息的索引，键为pid，值为一个整数，就是引索。
            processInfo.psInfo=psInfo;
            processInfo.last_renew_epoch=epoch;
            
        }
    }
    pclose(pipe);
    return true;
}
//这个过程将删除过期的进程，更新索引，统计总体信息。
bool organizeAndSummarize()
{
    processCnt=0;
    totalRss=0;
    bool t=tag;
    if(tag) tag=false;
    for(auto iter=processInfoList.begin();iter!=processInfoList.end();)
    {
        const auto& info_temp=*iter;
        if(info_temp.last_renew_epoch!=epoch)
        {
            processInfoListPid.erase(info_temp.psInfo.pid);
            iter=processInfoList.erase(iter);
        }
        else
        {
            auto& info=*iter;
            processCnt++;
            totalRss+=info.psInfo.rss;
            iter++;
            if(t)
            {
                info.enter_rss=info.psInfo.rss;
                info.enter_shrss=info.psInfo.shrss;
                info.enter_anonrss=info.psInfo.anonrss;
                info.enter_filerss=info.psInfo.filerss;
            }
            info.increased_rss=info.psInfo.rss-info.enter_rss;
            info.increased_anonrss=info.psInfo.anonrss-info.enter_anonrss;
            info.increased_filerss=info.psInfo.filerss-info.enter_filerss;
            info.increased_shrss=info.psInfo.shrss-info.enter_shrss;
        }
    }
    return true;
}
// 排序
//排序依据：enter_epoch（降序） rss（降序）pid
bool sortProcessInfo()
{
    auto sort_logic=[](const ProcessInfo& a,const ProcessInfo& b){
        if(a.enter_epoch!=b.enter_epoch)
        {
            return a.enter_epoch>b.enter_epoch;
        }
        else
        {
            if(a.increased_rss!=b.increased_rss)
            {
                return a.increased_rss>b.increased_rss;
            }
            else
            {
                return a.psInfo.pid>b.psInfo.pid;
            }
        }
    };
    processInfoList.sort(sort_logic);
    //sort(processInfoList.begin(),processInfoList.end(),sort_logic);
    return true;
}
//重建索引processInfoListPid
bool buildIndex()
{
    for(auto iter=processInfoList.begin();iter!=processInfoList.end();iter++)
    {
        auto& info=*iter;
        processInfoListPid[info.psInfo.pid]=iter;
    }
    return true;
}
bool need_save(decltype(processInfoList.begin()) &iter)
{
    
    for(auto& _ignore:erase_set)
    {
        if(iter->psInfo.name.find(_ignore)!=string::npos)
        {
            return false;
        }
    }
    if(fliter_set.size()==0)
    {
        return true;
    }
    bool save=false;
    for(auto& _fliter:fliter_set)
    {
        if(iter->psInfo.name.find(fliter)!=string::npos)
        {
            save=true;
            break;
        }
    }
    return save;
    
}
int main() {
    std::cout << "\033c";
    std::cout<<"command:"<<endl;
    thread t(control_func);
    while (true) {
        
        getProcessInfo2();
        //DEBUG("getProcessInfo2 return\n");
        organizeAndSummarize();
        //DEBUG("organizeAndSummarize return\n");
        sortProcessInfo();
        //DEBUG("sortProcessInfo return\n");
        buildIndex();

        epoch++;
        //DEBUG("buildIndex return\n");
        if(need_print)
        {
            cout<<"\033c";
            cout<<"cmdline:"<<endl;
            clearline std::cout<<"total rss: "<<totalRss<<endl;
            clearline std::cout<<"total process: "<<processCnt<<endl;
            clearline std::cout<<"pid\trss\tanonrss\tshrss\tfilerss\tvsz\tVMSwap\tirss\tishrss\tifrss\tianrss\tname\n";
            int index=0;
            decltype(processInfoList) processInfoList2=processInfoList;
            for(auto iter=processInfoList2.begin();iter!=processInfoList2.end();)
            {
                bool choose=false;
                for(auto& _fliter:fliter_set)
                {
                    if(iter->psInfo.name.find(fliter)!=string::npos)
                    {
                        choose=true;
                    }
                }
                if(!choose&&fliter_set.size()!=0)
                {
                    iter=processInfoList2.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
            // 输出进程信息
            for (const ProcessInfo&  prInfo : processInfoList2) {
                const PsInfo &psInfo=prInfo.psInfo;
                if(index<displayBegin)
                {
                    index++;
                    continue;
                }
                if(index>=displayBegin+displayCnt)
                {
                    index++;
                    continue;
                }
                
                
                clearline std::cout<< psInfo.pid <<"\t"<<psInfo.rss<<"\t"<<psInfo.anonrss<<"\t"<<psInfo.shrss<<"\t"<<psInfo.filerss<<"\t"<<psInfo.vsz<<"\t"<<psInfo.VmSwap<<"\t"
                <<prInfo.increased_rss<<"\t"<<prInfo.increased_shrss<<"\t"<<prInfo.increased_filerss<<"\t"<<prInfo.increased_anonrss<<"\t";
                cout<<psInfo.name<<endl;
                index++;
            }
        }
        sleep(1);
    }

    return 0;
}