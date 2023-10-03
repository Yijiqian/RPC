#include "mprpcconfig.h"
#include <iostream>
#include <string>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char*config_file)
{
    FILE *pf = fopen(config_file, "r");
    if(pf == nullptr)
    {
        std::cout << config_file << "is not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    /*
    feof(pf)：如果文件结束符被设置，则返回非0值，否则返回0。即 feof(fp) == 0 ,说明还没到文件的结尾；而如果
                feof(fp) != 0，说明文件指针已经读取越过了EOF，文档已经到结尾。
    EOF(End Of File)文件结束符，它的意思是，文档的结尾都有一个隐藏字符”EOF”，当程序读取它的时候，
    就会知道文件已经到达结尾。通常使用while循环加EOF判断作为读取结束的标志。
    */
    // 1.注释  2.正确的配置项 =  3.去掉开头多余的空格
    while(!feof(pf))
    {
        char buf[512]={0};
        fgets(buf, 512, pf);   // 读取一行数据

        // 去掉字符串前面多余的空格
        std::string read_buf(buf);
        Trim(read_buf);

        // 判断#的注释
        if(read_buf[0] == '#' || read_buf.empty()) continue;

        // 解析配置项
        int idx = read_buf.find('=');
        if(idx==-1)  
        {
            // 配置项不合法
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0, idx);
        Trim(key);
        int endidx = read_buf.find('\n', idx);
        value = read_buf.substr(idx+1, endidx - idx-1);
        Trim(value);
        

        m_configMap.insert({key, value});
    }

}
    
// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if(it==m_configMap.end())
    {
        return "";
    }
    return it->second;
}



void MprpcConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' ');
    if(idx!=-1)
    {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }

    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if(idx!=-1)
    {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx+1);
    }
}