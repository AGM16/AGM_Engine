#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__


#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem( bool start_enabled = true);

	// Destructor
	virtual ~ModuleFileSystem();

	// Called before render is available
	bool Init();

	// Called before quitting
	bool CleanUp();

	// Utility functions
	//bool Delete(const char* filename);
	bool AddPath(const char* path_or_zip, const char* mount_point = nullptr);
	bool Is_Init()const;
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	bool Create_Dir(const char* name_dir);
	bool Delete(const char* file_name);
	const char* Get_Base_Dir()const;
	const char* Get_User_Dir()const;
	const char* Get_Write_dir()const;
	bool Remove_From_Path(const char* file);
	char** Get_Search_path()const;
	bool Add_to_Search_path(const char* file, int apendtopath = 1);
	const char* Get_Real_dir(const char* filename)const;
	int Enumerate_files(const char* dir, std::vector<std::string>& buffer)const;
	int Get_Last_Mod(const char* file)const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	string Get_FileName_From_Path(const std::string strPath) const;

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const void* buffer, unsigned int size) const;
	bool Save_Unique(string& name, const void* buffer, uint size, const char* path, const char* extension, string& out_p);

private:

};

#endif