#include "BP_InventoryComponent.h"
#include "InventoryItem.h"
#include "InventorySystem.h"

UBP_InventoryComponent::UBP_InventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Initialize main inventory
    MainInventory = FInventoryGrid(10, 10);

    // Initialize equipment slots
    InitializeEquipmentSlots();
}

void UBP_InventoryComponent::InitializeEquipmentSlots()
{
    // Create default equipment slots
    TArray<EEquipmentSlot> DefaultSlots = {
        EEquipmentSlot::Helmet,
        EEquipmentSlot::Chest,
        EEquipmentSlot::Legs,
        EEquipmentSlot::Boots,
        EEquipmentSlot::Gloves,
        EEquipmentSlot::Backpack,
        EEquipmentSlot::Vest,
        EEquipmentSlot::PrimaryWeapon,
        EEquipmentSlot::SecondaryWeapon,
        EEquipmentSlot::Melee,
        EEquipmentSlot::Accessory1,
        EEquipmentSlot::Accessory2
    };

    for (EEquipmentSlot Slot : DefaultSlots)
    {
        FEquipmentSlotData SlotData;
        SlotData.SlotType = Slot;
        SlotData.EquippedItem = nullptr;
        SlotData.bIsLocked = false;
        EquipmentSlots.Add(Slot, SlotData);
    }
}

void UBP_InventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UBP_InventoryComponent::EquipItem(UInventoryItem* Item, EEquipmentSlot SlotType)
{
    if (!Item || !CanEquipItem(Item, SlotType))
        return false;

    FEquipmentSlotData* SlotData = EquipmentSlots.Find(SlotType);
    if (!SlotData)
        return false;

    // Unequip current item if any
    if (SlotData->EquippedItem)
    {
        UnequipItem(SlotType);
    }

    // Equip new item
    SlotData->EquippedItem = Item;
    RemoveItemFromInventory(Item);

    // Update containers if item is a container
    UpdateContainersFromEquipment();

    OnEquipmentChanged.Broadcast(SlotType, Item);
    return true;
}

bool UBP_InventoryComponent::UnequipItem(EEquipmentSlot SlotType)
{
    FEquipmentSlotData* SlotData = EquipmentSlots.Find(SlotType);
    if (!SlotData || !SlotData->EquippedItem)
        return false;

    UInventoryItem* Item = SlotData->EquippedItem;
    SlotData->EquippedItem = nullptr;

    // Add back to inventory
    AddItemToInventory(Item);

    // Update containers
    UpdateContainersFromEquipment();

    OnEquipmentChanged.Broadcast(SlotType, nullptr);
    return true;
}

UInventoryItem* UBP_InventoryComponent::GetEquippedItem(EEquipmentSlot SlotType)
{
    FEquipmentSlotData* SlotData = EquipmentSlots.Find(SlotType);
    return SlotData ? SlotData->EquippedItem : nullptr;
}

bool UBP_InventoryComponent::CanEquipItem(UInventoryItem* Item, EEquipmentSlot SlotType)
{
    if (!Item)
        return false;

    FEquipmentSlotData* SlotData = EquipmentSlots.Find(SlotType);
    if (!SlotData || SlotData->bIsLocked)
        return false;

    // Check if item can be equipped in this slot
    return Item->EquipmentSlot == SlotType || Item->EquipmentSlot == EEquipmentSlot::None;
}

bool UBP_InventoryComponent::AddItemToInventory(UInventoryItem* Item)
{
    if (!Item)
        return false;

    // Try to find a place in main inventory
    for (int32 Y = 0; Y < MainInventory.Height; ++Y)
    {
        for (int32 X = 0; X < MainInventory.Width; ++X)
        {
            FVector2D Position(X, Y);
            if (CanPlaceItemAt(Item, Position))
            {
                return PlaceItemAt(Item, Position);
            }
        }
    }

    return false;
}

bool UBP_InventoryComponent::AddItemToContainer(UInventoryItem* Item, const FString& ContainerName)
{
    if (!Item)
        return false;

    FInventoryGrid& Container = GetInventoryGrid(ContainerName);
    
    // Try to find a place in container
    for (int32 Y = 0; Y < Container.Height; ++Y)
    {
        for (int32 X = 0; X < Container.Width; ++X)
        {
            FVector2D Position(X, Y);
            if (CanPlaceItemAt(Item, Position, ContainerName))
            {
                return PlaceItemAt(Item, Position, ContainerName);
            }
        }
    }

    return false;
}

bool UBP_InventoryComponent::RemoveItemFromInventory(UInventoryItem* Item)
{
    if (!Item)
        return false;

    // Remove from main inventory
    MainInventory.Items.Remove(Item);
    MainInventory.ItemPositions.Remove(Item);

    // Remove from containers
    for (auto& Container : ContainerInventories)
    {
        Container.Value.Items.Remove(Item);
        Container.Value.ItemPositions.Remove(Item);
    }

    OnInventoryChanged.Broadcast(-1, Item);
    return true;
}

bool UBP_InventoryComponent::RemoveItemFromContainer(UInventoryItem* Item, const FString& ContainerName)
{
    if (!Item)
        return false;

    FInventoryGrid& Container = GetInventoryGrid(ContainerName);
    Container.Items.Remove(Item);
    Container.ItemPositions.Remove(Item);
    OnInventoryChanged.Broadcast(-1, Item);
    return true;
}

TArray<UInventoryItem*> UBP_InventoryComponent::GetAllItems()
{
    TArray<UInventoryItem*> AllItems;

    // Add items from main inventory
    AllItems.Append(MainInventory.Items);

    // Add items from containers
    for (const auto& Container : ContainerInventories)
    {
        AllItems.Append(Container.Value.Items);
    }

    // Add equipped items
    for (const auto& Equipment : EquipmentSlots)
    {
        if (Equipment.Value.EquippedItem)
        {
            AllItems.Add(Equipment.Value.EquippedItem);
        }
    }

    return AllItems;
}

UInventoryItem* UBP_InventoryComponent::FindItemByID(const FString& ItemID)
{
    TArray<UInventoryItem*> AllItems = GetAllItems();
    for (UInventoryItem* Item : AllItems)
    {
        if (Item && Item->ItemID == ItemID)
        {
            return Item;
        }
    }
    return nullptr;
}

bool UBP_InventoryComponent::CanPlaceItemAt(UInventoryItem* Item, FVector2D Position, const FString& ContainerName)
{
    if (!Item)
        return false;

    FInventoryGrid& Grid = GetInventoryGrid(ContainerName);
    return AreCellsAvailable(Grid, Position, Item->ItemSize);
}

bool UBP_InventoryComponent::PlaceItemAt(UInventoryItem* Item, FVector2D Position, const FString& ContainerName)
{
    if (!CanPlaceItemAt(Item, Position, ContainerName))
        return false;

    FInventoryGrid& Grid = GetInventoryGrid(ContainerName);
    Grid.Items.Add(Item);
    Grid.ItemPositions.Add(Item, FIntPoint((int32)Position.X, (int32)Position.Y));
    MarkCellsOccupied(Grid, Position, Item->ItemSize, true);

    OnInventoryChanged.Broadcast(-1, Item);
    return true;
}

void UBP_InventoryComponent::RemoveItemAt(FVector2D Position, const FString& ContainerName)
{
    UInventoryItem* Item = GetItemAt(Position, ContainerName);
    if (Item)
    {
        FInventoryGrid& Grid = GetInventoryGrid(ContainerName);
        FIntPoint* StoredPos = Grid.ItemPositions.Find(Item);
        FVector2D ClearPos = StoredPos ? FVector2D(StoredPos->X, StoredPos->Y) : Position;
        MarkCellsOccupied(Grid, ClearPos, Item->ItemSize, false);
        Grid.Items.Remove(Item);
        Grid.ItemPositions.Remove(Item);
        OnInventoryChanged.Broadcast(-1, Item);
    }
}

UInventoryItem* UBP_InventoryComponent::GetItemAt(FVector2D Position, const FString& ContainerName)
{
    FInventoryGrid& Grid = GetInventoryGrid(ContainerName);

    int32 Index = GetGridIndex((int32)Position.X, (int32)Position.Y, Grid.Width);
    if (Index >= 0 && Index < Grid.OccupiedCells.Num() && Grid.OccupiedCells[Index])
    {
        for (const auto& Pair : Grid.ItemPositions)
        {
            UInventoryItem* Item = Pair.Key;
            FIntPoint P = Pair.Value;
            if (Item)
            {
                int32 X0 = P.X;
                int32 Y0 = P.Y;
                int32 X1 = X0 + (int32)Item->ItemSize.X;
                int32 Y1 = Y0 + (int32)Item->ItemSize.Y;
                if ((int32)Position.X >= X0 && (int32)Position.X < X1 &&
                    (int32)Position.Y >= Y0 && (int32)Position.Y < Y1)
                {
                    return Item;
                }
            }
        }
    }

    return nullptr;
}

void UBP_InventoryComponent::CreateContainer(const FString& ContainerName, int32 Width, int32 Height)
{
    ContainerInventories.Add(ContainerName, FInventoryGrid(Width, Height));
}

void UBP_InventoryComponent::RemoveContainer(const FString& ContainerName)
{
    ContainerInventories.Remove(ContainerName);
}

TArray<FString> UBP_InventoryComponent::GetContainerNames()
{
    TArray<FString> Names;
    ContainerInventories.GetKeys(Names);
    return Names;
}

FInventoryGrid UBP_InventoryComponent::GetContainer(const FString& ContainerName)
{
    if (FInventoryGrid* Found = ContainerInventories.Find(ContainerName))
    {
        return *Found;
    }
    return FInventoryGrid(); // Return empty grid if not found
}

float UBP_InventoryComponent::GetTotalWeight()
{
    float TotalWeight = 0.0f;

    // Weight from all items
    TArray<UInventoryItem*> AllItems = GetAllItems();
    for (UInventoryItem* Item : AllItems)
    {
        if (Item)
        {
            TotalWeight += Item->GetTotalWeight();
        }
    }

    return TotalWeight;
}

float UBP_InventoryComponent::GetEquipmentStatModifier(const FString& StatName)
{
    float TotalModifier = 0.0f;

    for (const auto& Equipment : EquipmentSlots)
    {
        if (Equipment.Value.EquippedItem)
        {
            const float* Modifier = Equipment.Value.EquippedItem->StatModifiers.Find(StatName);
            if (Modifier)
            {
                TotalModifier += *Modifier;
            }
        }
    }

    return TotalModifier;
}

TMap<FString, float> UBP_InventoryComponent::GetAllStatModifiers()
{
    TMap<FString, float> AllModifiers;

    for (const auto& Equipment : EquipmentSlots)
    {
        if (Equipment.Value.EquippedItem)
        {
            for (const auto& Modifier : Equipment.Value.EquippedItem->StatModifiers)
            {
                float* ExistingModifier = AllModifiers.Find(Modifier.Key);
                if (ExistingModifier)
                {
                    *ExistingModifier += Modifier.Value;
                }
                else
                {
                    AllModifiers.Add(Modifier.Key, Modifier.Value);
                }
            }
        }
    }

    return AllModifiers;
}

void UBP_InventoryComponent::UpdateContainersFromEquipment()
{
    // Clear existing containers except main inventory
    ContainerInventories.Empty();

    // Add containers from equipped items
    for (const auto& Equipment : EquipmentSlots)
    {
        if (Equipment.Value.EquippedItem && Equipment.Value.EquippedItem->bIsContainer)
        {
            FString ContainerName = Equipment.Value.EquippedItem->ItemName;
            ContainerInventories.Add(ContainerName, Equipment.Value.EquippedItem->ContainerInventory);
        }
    }
}

FInventoryGrid& UBP_InventoryComponent::GetInventoryGrid(const FString& ContainerName)
{
    if (ContainerName.IsEmpty())
    {
        return MainInventory;
    }
    
    FInventoryGrid* Found = ContainerInventories.Find(ContainerName);
    return Found ? *Found : MainInventory; // Fallback to main inventory
}

void UBP_InventoryComponent::MarkCellsOccupied(FInventoryGrid& Grid, FVector2D Position, FVector2D Size, bool bOccupied)
{
    for (int32 Y = (int32)Position.Y; Y < (int32)Position.Y + (int32)Size.Y; ++Y)
    {
        for (int32 X = (int32)Position.X; X < (int32)Position.X + (int32)Size.X; ++X)
        {
            int32 Index = GetGridIndex(X, Y, Grid.Width);
            if (Index >= 0 && Index < Grid.OccupiedCells.Num())
            {
                Grid.OccupiedCells[Index] = bOccupied;
            }
        }
    }
}

bool UBP_InventoryComponent::AreCellsAvailable(FInventoryGrid& Grid, FVector2D Position, FVector2D Size)
{
    // Check if position + size would exceed grid bounds
    if (Position.X + Size.X > Grid.Width || Position.Y + Size.Y > Grid.Height)
        return false;

    // Check if all cells are free
    for (int32 Y = (int32)Position.Y; Y < (int32)Position.Y + (int32)Size.Y; ++Y)
    {
        for (int32 X = (int32)Position.X; X < (int32)Position.X + (int32)Size.X; ++X)
        {
            int32 Index = GetGridIndex(X, Y, Grid.Width);
            if (Index >= 0 && Index < Grid.OccupiedCells.Num() && Grid.OccupiedCells[Index])
            {
                return false;
            }
        }
    }

    return true;
}

int32 UBP_InventoryComponent::GetGridIndex(int32 X, int32 Y, int32 Width)
{
    return Y * Width + X;
}
