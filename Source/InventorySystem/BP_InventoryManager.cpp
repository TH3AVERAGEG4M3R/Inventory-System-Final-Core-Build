#include "BP_InventoryManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/DataTable.h"      // UDataTable
#include "InventoryItem.h"         // UInventoryItem
#include "InventoryStructs.h"      // FItemDataTableRow
#include "InventorySystem.h"       // LogInventorySystem (if you use it)

ABP_InventoryManager::ABP_InventoryManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABP_InventoryManager::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogInventorySystem, Warning, TEXT("Inventory Manager initialized"));
}

void ABP_InventoryManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

UInventoryItem* ABP_InventoryManager::CreateItemFromID(const FString& ItemID)
{
    if (!ItemDataTable)
    {
        UE_LOG(LogInventorySystem, Error, TEXT("ItemDataTable is not set in InventoryManager"));
        return nullptr;
    }

    FItemDataTableRow* RowData = ItemDataTable->FindRow<FItemDataTableRow>(FName(*ItemID), TEXT(""));
    if (!RowData)
    {
        UE_LOG(LogInventorySystem, Warning, TEXT("Item ID '%s' not found in data table"), *ItemID);
        return nullptr;
    }

    return CreateItemFromRowData(*RowData);
}

UInventoryItem* ABP_InventoryManager::CreateItemFromDataTable(const FString& RowName)
{
    return CreateItemFromID(RowName);
}

TArray<UInventoryItem*> ABP_InventoryManager::CreateMultipleItems(const FString& ItemID, int32 Quantity)
{
    TArray<UInventoryItem*> Items;
    
    for (int32 i = 0; i < Quantity; ++i)
    {
        UInventoryItem* Item = CreateItemFromID(ItemID);
        if (Item)
        {
            Items.Add(Item);
        }
    }
    
    return Items;
}

bool ABP_InventoryManager::LoadItemDataTable(const FString& DataTablePath)
{
    UDataTable* LoadedTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
    if (LoadedTable)
    {
        ItemDataTable = LoadedTable;
        UE_LOG(LogInventorySystem, Warning, TEXT("Loaded item data table from: %s"), *DataTablePath);
        return true;
    }
    
    UE_LOG(LogInventorySystem, Error, TEXT("Failed to load data table from: %s"), *DataTablePath);
    return false;
}

TArray<FString> ABP_InventoryManager::GetAllItemIDs()
{
    TArray<FString> ItemIDs;
    
    if (ItemDataTable)
    {
        TArray<FName> RowNames = ItemDataTable->GetRowNames();
        for (const FName& RowName : RowNames)
        {
            ItemIDs.Add(RowName.ToString());
        }
    }
    
    return ItemIDs;
}

FItemDataTableRow ABP_InventoryManager::GetItemData(const FString& ItemID)
{
    FItemDataTableRow DefaultRow;
    
    if (!ItemDataTable)
        return DefaultRow;
        
    FItemDataTableRow* RowData = ItemDataTable->FindRow<FItemDataTableRow>(FName(*ItemID), TEXT(""));
    return RowData ? *RowData : DefaultRow;
}

ABP_InventoryManager* ABP_InventoryManager::GetInventoryManager(const UObject* WorldContext)
{
    if (!WorldContext)
        return nullptr;
        
    UWorld* World = WorldContext->GetWorld();
    if (!World)
        return nullptr;
        
    // Find existing manager in world
    for (TActorIterator<ABP_InventoryManager> ActorItr(World); ActorItr; ++ActorItr)
    {
        return *ActorItr;
    }
    
    // Create new manager if none exists
    ABP_InventoryManager* NewManager = World->SpawnActor<ABP_InventoryManager>();
    UE_LOG(LogInventorySystem, Warning, TEXT("Created new Inventory Manager instance"));
    return NewManager;
}

UInventoryItem* ABP_InventoryManager::CreateItemFromRowData(const FItemDataTableRow& RowData)
{
    // Check if there's a custom class for this item
    TSubclassOf<UInventoryItem>* ItemClass = ItemClasses.Find(RowData.ItemID);
    
    UInventoryItem* NewItem;
    if (ItemClass && *ItemClass)
    {
        NewItem = NewObject<UInventoryItem>(this, *ItemClass);
    }
    else
    {
        NewItem = NewObject<UInventoryItem>(this);
    }
    
    if (NewItem)
    {
        NewItem->CopyFromDataTableRow(RowData);
        UE_LOG(LogInventorySystem, Log, TEXT("Created item: %s"), *NewItem->ItemName);
    }
    
    return NewItem;
}
