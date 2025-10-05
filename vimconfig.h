#ifndef VIMCONFIG_H
#define VIMCONFIG_H

#include <string>
#include <vector>

// 配置项结构体
typedef struct {
    std::wstring name;
    std::wstring description;
    std::string option;
    bool enabled;
    bool isToggle;
} ConfigItem;

// 配置处理类
class VimConfig {
private:
    std::vector<ConfigItem> configItems;
    std::string vimrcPath;
    std::string originalContent;
    std::string currentContent;
    
public:
    VimConfig();
    ~VimConfig();
    
    bool LoadFromFile(const std::string& path);
    bool SaveToFile(const std::string& path);
    
    std::vector<ConfigItem>& GetConfigItems();
    std::string GetOriginalContent() const;
    std::string GenerateConfigContent() const;
    
    void UpdateConfigItem(const std::wstring& name, bool enabled);
};

#endif // VIMCONFIG_H