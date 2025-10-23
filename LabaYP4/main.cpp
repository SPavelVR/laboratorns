#include <iostream>
#include <string>

using namespace std;

// Общий класс для хранилищь информаций
class StorageMemory
{
protected:

    string typeDisk = "UNKNOWN";
    unsigned int capacity = 0;
    unsigned int size = 0;

public:

    StorageMemory() {}
    StorageMemory(string typeDisk, unsigned int capacity) : typeDisk(typeDisk), capacity(capacity) {}

    virtual ~StorageMemory()
    {
        cout << " -- StorageMemory delete --\n";
    };

    virtual int pushMemory(unsigned int pushMemory) = 0;
    virtual int freeMemory(unsigned int freeMemory) = 0;

    virtual string getInfo() = 0;
};


// Класс жесткого диска
class HardDisk : public StorageMemory
{

protected:

    unsigned int speedDisk = 0;

public:

    HardDisk(string typeDisk, unsigned int capacity, unsigned int speedDisk) : StorageMemory(typeDisk, capacity), speedDisk(speedDisk) {}

    ~HardDisk() override
    {
        cout << " -- HardDisk delete --\n";
    }

    int pushMemory(unsigned int pushMemory) override
    {
        if (this->size + pushMemory >= capacity) return 0;

        this->size += pushMemory;
        return 1;
    }

    int freeMemory(unsigned int freeMemory) override
    {
        if (freeMemory > this->size) return 0;

        this->size -= freeMemory;
        return 1;
    }

    string getInfo() override
    {
        return "HardDisk: " + string(typeDisk) + ", Capacity: " + to_string(capacity) +
            ", Speed: " + to_string(speedDisk) + " RPM, Used: " + to_string(size);
    }

};

// Класс жесткого съёмного диска
class RemovableHardDisk : public HardDisk
{

    string interfaceType = "USB";

public:

    RemovableHardDisk(string typeDisk, unsigned int capacity, unsigned int speedDisk, string interfaceType)
        : HardDisk(typeDisk, capacity, speedDisk), interfaceType(interfaceType) {
    }

    ~RemovableHardDisk() override
    {
        cout << " -- RemovableHardDisk delete --\n";
    }

    string getInfo() override
    {
        return "RemovableHardDisk: " + string(typeDisk) + ", Capacity: " + to_string(capacity) +
            ", Speed: " + to_string(speedDisk) + " RPM, Interface: " + string(interfaceType) +
            ", Used: " + to_string(size);
    }

};

// Класс флешки
class FlashCard : public StorageMemory
{

    string cardType = "fleshka";
    unsigned int writeSpeed = 0;

public:

    FlashCard(string typeDisk, unsigned int capacity, string cardType, unsigned int writeSpeed)
        : StorageMemory(typeDisk, capacity), cardType(cardType), writeSpeed(writeSpeed) {
    }

    ~FlashCard() override
    {
        cout << " -- FlashCard delete --\n";
    }

    int pushMemory(unsigned int pushMemory) override
    {
        if (this->size + pushMemory >= capacity) return 0;

        this->size += pushMemory;
        return 1;
    }

    int freeMemory(unsigned int freeMemory) override
    {
        if (freeMemory > this->size) return 0;

        this->size -= freeMemory;
        return 1;
    }

    string getInfo() override
    {
        return "FlashCard: " + string(typeDisk) + ", Capacity: " + to_string(capacity) +
            ", Type: " + string(cardType) + ", Write Speed: " + to_string(writeSpeed) +
            " MB/s, Used: " + to_string(size);
    }

};

int main()
{
    StorageMemory* storage1 = new HardDisk("SSD", 100, 5000);
    StorageMemory* storage2 = new RemovableHardDisk("RDisk", 230, 5400, "USB 4.0");
    StorageMemory* storage3 = new FlashCard("flesh", 40, "mecroSD", 100);

    cout << "=== Storage Devices Info ===" << endl;
    cout << storage1->getInfo() << endl;
    cout << storage2->getInfo() << endl;
    cout << storage3->getInfo() << endl;

    cout << "\n=== Testing Memory Operations ===" << endl;
    storage1->pushMemory(27);
    storage2->pushMemory(99);
    storage3->pushMemory(41);

    cout << "After pushing memory:" << endl;
    cout << storage1->getInfo() << endl;
    cout << storage2->getInfo() << endl;
    cout << storage3->getInfo() << endl;

    storage1->freeMemory(22);
    storage3->freeMemory(15);

    cout << "\nAfter freeing memory:" << endl;
    cout << storage1->getInfo() << endl;
    cout << storage2->getInfo() << endl;
    cout << storage3->getInfo() << endl;

    cout << "\n=== Cleanup ===" << endl;
    delete storage1;
    delete storage2;
    delete storage3;

    return 0;
}