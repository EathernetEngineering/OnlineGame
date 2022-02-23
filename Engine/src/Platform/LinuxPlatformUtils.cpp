#include <Engine/pch.h>
#include <Engine/PlatformUtils.h>

#include <unistd.h>

namespace cee
{
	namespace engine
	{
		namespace FileDialogs
		{
			std::string OpenFile(const char* filter, const char* title)
			{
				const char zenityP[] = "/usr/bin/zenity";
				
				char cwd[512];
				getcwd(cwd, 512);
				
				uint32_t filterLength = strlen(filter);
				
				char call[1024];
				
				if (filterLength > 1)
				{
					std::string formattedFilter;
					uint32_t lastDelimiterIndex = -1;
					for (uint32_t i = 0; i < filterLength; i++)
					{
						if (filter[i] == ';')
						{
							formattedFilter.append(" *");
							formattedFilter.append(filter + (lastDelimiterIndex + 1), filter + i);
							lastDelimiterIndex = i;
						}
					}
					formattedFilter.append(" *");
					formattedFilter.append(filter + (lastDelimiterIndex + 1), filter + filterLength);
					sprintf(call, "%s --file-selection --modal --filename =\"%s\" --title=\"%s\" --file-filter=\"%s\"", zenityP, cwd, title, formattedFilter.c_str());
				}
				else
				{
					sprintf(call, "%s --file-selection --modal --filename =\"%s\" --title=\"%s\"", zenityP, cwd, title);
				}
				
				char path[1024];
				FILE* f = popen(call, "r");
				fgets(path, 1024, f);
				
				int ret = pclose(f);
				
				switch (ret)
				{
					case 0:
					{
						return path;
					}
					
					case 1:
					{
						CEE_CORE_WARN("Open file cancelled");
						return std::string();
					}
					
					default:
					{
						CEE_CORE_ERROR("Failed to open file");
						return std::string();
					}
				}
			};
			
			std::string SaveFile(const char* filter, const char* title)
			{
				const char zenityP[] = "/usr/bin/zenity";
				uint32_t filterLength = strlen(filter);
				
				char call[1024];
				
				if (filterLength > 1)
				{
					std::string formattedFilter;
					uint32_t lastDelimiterIndex = -1;
					for (uint32_t i = 0; i < filterLength; i++)
					{
						if (filter[i] == ';')
						{
							formattedFilter.append(" *");
							formattedFilter.append(filter + (lastDelimiterIndex + 1), filter + i);
							lastDelimiterIndex = i;
						}
					}
					formattedFilter.append(" *");
					formattedFilter.append(filter + (lastDelimiterIndex + 1), filter + filterLength);
					sprintf(call, "%s --file-selection --modal --save --title=\"%s\" --file-filter=\"%s\"", zenityP, title, formattedFilter.c_str());
				}
				else
				{
					sprintf(call, "%s --file-selection --save --modal --title=\"%s\"", zenityP, title);
				}
				
				char path[1024];
				FILE* f = popen(call, "r");
				fgets(path, 1024, f);
				
				int ret = pclose(f);
				
				switch (ret)
				{
					case 0:
					{
						return path;
					}
					
					case 1:
					{
						CEE_CORE_WARN("Save file cancelled");
						return std::string();
					}
					
					default:
					{
						CEE_CORE_ERROR("Failed to save file");
						return std::string();
					}
				}
			};
		}
	}
}
