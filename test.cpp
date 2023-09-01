#include<iostream>
#include<unistd.h>
using namespace std;
int main()
{
    cout<<"\033c";
    cout<<"hello world"<<endl;
    cout<<"222"<<endl;
    cout<<"\033[1;1H";
    cout<<"\033[K";
    sleep(5);
}