#include "p2Defs.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

using namespace std;

ModuleFileSystem::ModuleFileSystem(bool start_enabled) : Module(start_enabled)
{
	Set_Name_Module("File_System");

	// need to be created before Awake so other modules can use it
	// The dir where aplication runs
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	// By default we include executable's own directory
	// without this we won't be able to find config.xml :-(
	AddPath(".");
}

// Destructor
ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool ModuleFileSystem::Init()
{
	LOG("Loading File System");
	bool ret = true;

	if (PHYSFS_setWriteDir(".") == 0)
	{
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		// We add the writing directory as a reading directory too with speacial mount point
		//Creation of folders that will save FBX information
		Create_Dir("/Library");
		AddPath("Game/");

	}

	//SDL_free(write_path);

	return ret;
}

string ModuleFileSystem::Get_FileName_From_Path(const std::string strPath) const
{
	return strPath.substr(strPath.find_last_of("/\\") + 1);
}

// Called before quitting
bool ModuleFileSystem::CleanUp()
{
	//LOG("Freeing File System subsystem");
	return true;
}

// Add a new zip file or folder
bool ModuleFileSystem::AddPath(const char* path_or_zip, const char* mount_point)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		LOG("File System error while adding a path or zip(%s): %s\n", path_or_zip, PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::Delete(const char* file_name)
{
	bool ret = false;

	if (file_name != nullptr)
	{   //Delete files form write dir
		if (PHYSFS_delete(file_name) == 0)
		{
			LOG("File System error while deleting (%s): %s\n", file_name, PHYSFS_getLastError());
		}
		else
			ret = true;
	}

	return ret;
}

bool ModuleFileSystem::Create_Dir(const char* name_dir)
{
	bool ret = false;
	if (name_dir != nullptr && Exists(name_dir) == false)
	{   //Create a dir(folder) into the write dir
		if (PHYSFS_mkdir(name_dir) == 0)
		{
			LOG("File System error while creatring a new dir (%s): %s\n", name_dir, PHYSFS_getLastError());
		}
		else
			ret = true;
	}
	else
	{
		LOG("The %s already exists: %s\n", name_dir, PHYSFS_getLastError());
	}

	return ret;
}


const char* ModuleFileSystem::Get_Base_Dir()const
{
	const char* dir = nullptr;

	dir = PHYSFS_getBaseDir();

	if (dir == nullptr)
	{
		//Get the directory where the aplication resides
		LOG("File System error while getting the base dir : %s\n", PHYSFS_getLastError());
		return nullptr;
	}

	return dir;
}

const char* ModuleFileSystem::Get_User_Dir()const
{
	const char* dir = nullptr;

	dir = PHYSFS_getUserDir();

	if (dir == nullptr)
	{
		LOG("File System error while creatring the user dir : %s\n", PHYSFS_getLastError());
		return nullptr;
	}

	return dir;
}

const char* ModuleFileSystem::Get_Write_dir()const
{
	const char* write_dir = nullptr;

	write_dir = PHYSFS_getWriteDir();
	if (write_dir == nullptr)
	{   //Get the write dir
		LOG("File System error while getting the write dir : %s\n", PHYSFS_getLastError());
		return nullptr;
	}

	return write_dir;

}

bool ModuleFileSystem::Remove_From_Path(const char* file)
{
	bool ret = true;

	if (PHYSFS_removeFromSearchPath(file) == 0)
	{   //Delete an archive(document,zip or rar) from the dir where the aplication runs
		LOG("File System error while removing a file(%s) or dir : %s\n", file, PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}


char** ModuleFileSystem::Get_Search_path()const
{
	char** search_path = PHYSFS_getSearchPath();
	char** i;
	//Search all the archieve that we have in the dir where aplication runs
	for (i = search_path; *i != nullptr; i++)
		LOG("[%s] is in the search path.\n", *i);

	PHYSFS_freeList(search_path);

	if (i == nullptr)
	{
		LOG("File System error while searching paths: %s\n", PHYSFS_getLastError());
		return nullptr;
	}

	return i;
}

bool ModuleFileSystem::Add_to_Search_path(const char* file, int apendtopath)
{
	bool ret = true;

	if (PHYSFS_addToSearchPath(file, apendtopath) == 0)
	{
		LOG("File System error while appending archieve(%s): %s\n", file, PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}


const char* ModuleFileSystem::Get_Real_dir(const char* filename)const
{
	const char* dir = nullptr;

	dir = PHYSFS_getRealDir(filename);

	if (dir == nullptr)
	{
		LOG("File System error while getting the real dir of a file(%s): %s\n", filename, PHYSFS_getLastError());
	}

	return dir;
}

int ModuleFileSystem::Enumerate_files(const char* dir, std::vector<std::string>& buffer)const
{
	int ret = 0;

	char** files = PHYSFS_enumerateFiles(dir);
	char** i;

	for (i = files; *i != nullptr; i++)
	{
		LOG("*We have got [%s]", *i);
		buffer.push_back(*i);
		ret++;
	}

	PHYSFS_freeList(files);


	return ret;
}


int ModuleFileSystem::Get_Last_Mod(const char* file)const
{
	//Get the number of seconds from the last modification
	int ret = PHYSFS_getLastModTime(file);

	if (ret == -1)
	{
		LOG(" The last time modification of this file(%s) can't be determinated.", file, PHYSFS_getLastError());
		return 0;
	}

	LOG(" The last time modification of this file(%s) was %i.", file, ret, PHYSFS_getLastError());

	return ret;
}




// Check if a file exists
bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

//Check if the library PHYSFS is initialize
bool ModuleFileSystem::Is_Init()const
{
	return PHYSFS_isInit() != 0;
}

// Read a whole file and put it in a new buffer
unsigned int ModuleFileSystem::Load(const char* file, char** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = (uint)readed;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* ModuleFileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != nullptr)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return nullptr;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::Save_Unique(string& name, const void* buffer, uint size, const char* path, const char* extension, string& out_p)
{
	char save_name[100];
	sprintf_s(save_name, 50, "%s.%s", name.c_str(), extension);
	char name_file[200];
	sprintf_s(name_file, 200, "%s%s", path, save_name);


	//Check if the file is a texture
	//If is it we don't have to create a new other one with different name.
	//Several Meshes can share the same texture.
	string extension_type = extension;
	if (extension_type.compare("tex") != 0)
	{
		if (Exists(name_file))
		{
			int copy = 0;
			bool is_repeated = true;
			while (is_repeated)
			{
				//Change the name of the mesh
				++copy;
				sprintf_s(save_name, 100, "%s_m_%d.%s", name.c_str(), copy, extension);
				sprintf_s(name_file, 100, "%s%s", path, save_name);
				is_repeated = false;

				//Check if exist the new name of the file
				if (Exists(name_file))
				{
					is_repeated = true;
				}

			}
		}
	}

	if (Save(name_file, buffer, size) > 0)
	{
		out_p = name_file;
		return true;
	}

	return false;

}
