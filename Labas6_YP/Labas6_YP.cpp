
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

/*

All exception:
    ME_NoFindFile
    ME_FileAlreadyCreated
    ME_NoAllocateMemory


*/

// Class for the exception on the run time if no find a file in system;
class ME_NoFindFile : public exception
{
protected:

    string fname;

public:

    ME_NoFindFile(string& fname) : fname(fname) {};

    const char* what() const noexcept override
    {
        static string errMsg = "[Not find file] >> " + fname + " <<";

        return errMsg.c_str();
    }

    string getFName() const { return fname; };

    virtual ~ME_NoFindFile() = default;
};


// Class for exception on run time if system already creat file
class ME_FileAlreadyCreated : public exception
{
protected:

    string fname;
    size_t size;

public:

    ME_FileAlreadyCreated(string& fname, size_t size) : fname(fname), size(size) {};

    const char* what() const noexcept override
    {
        static string errMsg = "[File already created] >> " + fname + " << and size >> " + to_string(size) + " <<";

        return errMsg.c_str();
    }

    size_t getSize() const { return size; };
    string getFName() const { return fname; };

    virtual ~ME_FileAlreadyCreated() = default;
};


// Class for the exception on run time if system don't have memory that give in file;
class ME_NoAllocateMemory : public exception
{
protected:

    size_t needMemory;
    size_t freeMemory;

public:
    
    ME_NoAllocateMemory(size_t needMemory, size_t freeMemory) : needMemory(needMemory), freeMemory(freeMemory) {};

    const char* what() const noexcept override
    {
        static string errMsg = "[No allocate memory] need >> " + to_string(needMemory) + " <<, system have >> " + to_string(freeMemory) + " <<";

        return errMsg.c_str();
    }

    size_t getNeedMemory() const { return needMemory; };
    size_t getFreeMemory() const { return freeMemory; };

    virtual ~ME_NoAllocateMemory() = default;
};


typedef struct File File;

typedef struct File
{
    string name;
    size_t size;
    size_t index;

    File(string& name, size_t size) : name(name), size(size), index(0) {};
    File(string& name, size_t size, size_t index) : name(name), size(size), index(index) {};
} File;


// Little system for creat and write in files
class System
{
protected:

    char* memory = nullptr;
    size_t capacity = 0;
    size_t size = 0;
    vector<File> files;

public:

    System(size_t capacity) : capacity(capacity)
    {
        memory = nullptr;

        if (capacity != 0)
        {
            memory = new char[capacity];
        }

        if (memory == nullptr)
        {
            throw bad_alloc();
        }

        memset(memory, 0, capacity - 1);
    }

    int creatFile(string& fname, size_t size)
    {
        if (this->size + size >= capacity) throw ME_NoAllocateMemory(size, capacity - this->size);

        for (File& file : files)
        {
            if (file.name == fname)
            {
                throw ME_FileAlreadyCreated(fname, file.size);
            }
        }

        files.push_back(File(fname, size, this->size));

        this->size += size;

        return 0;
    }


    int writeFile(string& fname, const char* data, size_t length)
    {

        for (File& file : files)
        {
            if (file.name == fname)
            {
                if (file.size < length) throw length_error("File [" + fname + "] have " + to_string(file.size) + ", but need " + to_string(length));

                memcpy(memory + file.index, data, length);

                return 0;
            }
        }

        throw ME_NoFindFile(fname);

        return 1;
    }


    void printFiles(ostream& stream) const
    {
        stream << "System have " << files.size() << " files\nMemory " << size << " per " << capacity << endl << "All files:\n";

        for (const File& file : files)
        {
            stream << "File [" << file.name << "] Size: " << file.size << " Bytes\tData: " << memory + file.index << endl;;
        }
    }

    virtual ~System()
    {
        if (memory != nullptr)
        {
            delete[] memory;
        }

        files.clear();
    }
};

//ME_NoFindFile
//ME_FileAlreadyCreated
//ME_NoAllocateMemory

#define CREATFILE(name, fsize) {(string&)string(name), (size_t)fsize}

int main()
{

    size_t capacitySystem = 1024;
    vector<File> preCreatedFiles = {
        CREATFILE("Name.txt", 200),
        CREATFILE("text.txt", 200),
        CREATFILE("fi.txt", 187)
    };
    string fname = "text.txt";
    const char data[] = "Text text text WAAHKLSFBC_FISAHsdkfnaskojdfb sadjipowajrfkjdbdghuisd";

    size_t fsize = strlen(data) + 1;
    System* system = nullptr;

    try
    {
        system = new System(capacitySystem);

        for (File& file : preCreatedFiles)
        {
            system->creatFile(file.name, file.size);
        }

        system->writeFile(fname, data, fsize);

        cout << "System end with no error!\n";
    }
    catch (const ME_NoFindFile& e)
    {
        cout << "Error:\n" << e.what() << endl;

        system->creatFile(fname, fsize);
        system->writeFile(fname, data, fsize);
    }
    catch (const std::exception& e)
    {
        cout << "ERROR" << endl;
        cout << e.what() << endl;
    }


    if (system != nullptr)
    {
        cout << "=== === ===\n";
        system->printFiles(cout);
        delete system;
    }

    std::cout << "End of program\n";
}