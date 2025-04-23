#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

class DuplicateItemException : public runtime_error 
{
public:
    DuplicateItemException(const string& msg) : runtime_error(msg) {}
};

class ItemNotFoundException : public runtime_error 
{
public:
    ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};

class StoredItem 
{
private:
    string id;
    string description;
    string location;

public:
    StoredItem(string id, string desc, string loc)
        : id(id), description(desc), location(loc) {}

    string getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
};


class StorageManager 
{
private:
    unordered_map<string, shared_ptr<StoredItem>> itemById;
    map<string, shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const shared_ptr<StoredItem>& item) 
    {
        string id = item->getId();
        if (itemById.count(id)) //counts the number of times it shows up in the itembyid
        {
            throw DuplicateItemException("Item with ID " + id + " already exists!");
        }
        itemById[id] = item;//adds item to map
        itemByDescription[item->getDescription()] = item;//sanme as above
    }

    shared_ptr<StoredItem> findById(const string& id) const 
    {
        auto it = itemById.find(id);//look up an item by it's id
        if (it == itemById.end()) 
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        return it->second;
    }

    void removeItem(const string& id) // removes item by id 
    {
        auto it = itemById.find(id);
        if (it == itemById.end()) 
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        auto desc = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(desc);//removes from both maps
    }

    void listItemsByDescription() const 
    {
        cout << "Items in Description Order:\n";
        for (const auto& pair : itemByDescription) //loops through description like LED motor from stored item class
        {
            auto item = pair.second; //addsd to shared pair of desc and location (-fan motor : aisle 1000 shelf 3000
            cout << "- " << item->getDescription()
                << ": " << item->getLocation() << "\n";
        }
    }
};

void testDuplicateAddition(StorageManager& manager) //fucntions to test for duplicates
{
    cout << "Attempting to add ITEM001 again.\n";
    try {
        auto item = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 2, Shelf 3");
        manager.addItem(item);
    }
    catch (const DuplicateItemException& ex) 
    {
        cout << "Error: " << ex.what() << "\n";
    }
}

void testItemNotFound(StorageManager& manager) //test item not found
{
    cout << "Removing ITEM003...\n";
    try {
        manager.removeItem("ITEM003");
    }
    catch (const ItemNotFoundException& ex) 
    {
        cout << "Error: " << ex.what() << "\n";
    }
}

int main() 
{
    StorageManager manager;

    cout << "Adding item: ITEM001 - LED Light\n";//add items to start
    auto item1 = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 2, Shelf 1");
    manager.addItem(item1);

    cout << "Adding item: ITEM002 - Fan Motor\n";//add items to start
    auto item2 = make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 3, Shelf 5");
    manager.addItem(item2);

    testDuplicateAddition(manager);//see if we found dupliocates (which should)

    cout << "Retrieving ITEM002\n";//find and retrieeve item 002
    try {
        auto found = manager.findById("ITEM002");
        cout << "Found: " << found->getDescription()
            << " at " << found->getLocation() << "\n";
    }
    catch (const ItemNotFoundException& ex) {
        cout << "Error: " << ex.what() << "\n";
    }

    testItemNotFound(manager);//find non existing item

    manager.listItemsByDescription();//list all the items out

    return 0;
}
