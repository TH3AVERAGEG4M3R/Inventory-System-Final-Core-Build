#include "InventoryItem.h"

UInventoryItem::UInventoryItem()
{
    ItemID = TEXT("");
    ItemName = TEXT("New Item");
    Description = TEXT("A new item");
    ItemType = EItemType::Material;
    Rarity = EItemRarity::Common;
    EquipmentSlot = EEquipmentSlot::None;
    
    ItemSize = FVector2D(1, 1);
    Weight = 1.0f;
    StackSize = 1;
    MaxStackSize = 1;
    
    CurrentDurability = 100.0f;
    MaxDurability = 100.0f;
    
    Value = 10;
    
    bIsContainer = false;
    WeightReduction = 0.0f;
    
    ItemIcon = nullptr;
    ItemMesh = nullptr;
}

bool UInventoryItem::CanStackWith(const UInventoryItem* OtherItem) const
{
    if (!OtherItem || OtherItem == this)
        return false;
        
    return (ItemID == OtherItem->ItemID && 
            StackSize < MaxStackSize && 
            OtherItem->StackSize < OtherItem->MaxStackSize);
}

int32 UInventoryItem::AddToStack(int32 Amount)
{
    int32 CanAdd = FMath::Min(Amount, MaxStackSize - StackSize);
    StackSize += CanAdd;
    return Amount - CanAdd; // Return remainder
}

int32 UInventoryItem::RemoveFromStack(int32 Amount)
{
    int32 CanRemove = FMath::Min(Amount, StackSize);
    StackSize -= CanRemove;
    return CanRemove;
}

float UInventoryItem::GetDurabilityPercentage() const
{
    if (MaxDurability <= 0.0f)
        return 1.0f;
        
    return CurrentDurability / MaxDurability;
}

void UInventoryItem::TakeDamage(float DamageAmount)
{
    CurrentDurability = FMath::Max(0.0f, CurrentDurability - DamageAmount);
}

void UInventoryItem::RepairItem(float RepairAmount)
{
    CurrentDurability = FMath::Min(MaxDurability, CurrentDurability + RepairAmount);
}

bool UInventoryItem::IsStackFull() const
{
    return StackSize >= MaxStackSize;
}

bool UInventoryItem::IsBroken() const
{
    return CurrentDurability <= 0.0f;
}

bool UInventoryItem::CanStoreItem(const UInventoryItem* Item) const
{
    if (!bIsContainer || !Item)
        return false;
        
    // Check if item type is allowed
    if (AllowedItemTypes.Num() > 0 && !AllowedItemTypes.Contains(Item->ItemType))
        return false;
        
    return true;
}

float UInventoryItem::GetTotalWeight() const
{
    float TotalWeight = Weight;
    
    if (bIsContainer)
    {
        float ContainerWeight = 0.0f;
        for (const UInventoryItem* Item : ContainerInventory.Items)
        {
            if (Item)
            {
                ContainerWeight += Item->GetTotalWeight();
            }
        }
        
        // Apply weight reduction
        ContainerWeight *= (1.0f - WeightReduction);
        TotalWeight += ContainerWeight;
    }
    
    return TotalWeight;
}

void UInventoryItem::InitializeContainer(int32 Width, int32 Height)
{
    if (bIsContainer)
    {
        ContainerInventory = FInventoryGrid(Width, Height);
    }
}

FLinearColor UInventoryItem::GetRarityColor() const
{
    switch (Rarity)
    {
        case EItemRarity::Common:
            return FLinearColor::Gray;
        case EItemRarity::Uncommon:
            return FLinearColor::Green;
        case EItemRarity::Rare:
            return FLinearColor::Blue;
        case EItemRarity::Epic:
            return FLinearColor(0.5f, 0.0f, 1.0f); // Purple
        case EItemRarity::Legendary:
            return FLinearColor(1.0f, 0.5f, 0.0f); // Orange
        default:
            return FLinearColor::White;
    }
}

FString UInventoryItem::GetFormattedDescription() const
{
    FString FormattedDesc = Description;
    
    if (bIsContainer)
    {
        FormattedDesc += FString::Printf(TEXT("\n\nContainer: %dx%d slots"), 
            (int32)ContainerInventory.Width, (int32)ContainerInventory.Height);
            
        if (WeightReduction > 0.0f)
        {
            FormattedDesc += FString::Printf(TEXT("\nWeight Reduction: %.0f%%"), WeightReduction * 100.0f);
        }
    }
    
    if (StatModifiers.Num() > 0)
    {
        FormattedDesc += TEXT("\n\nStat Modifiers:");
        for (const auto& Modifier : StatModifiers)
        {
            FormattedDesc += FString::Printf(TEXT("\n%s: +%.1f"), *Modifier.Key, Modifier.Value);
        }
    }
    
    return FormattedDesc;
}

void UInventoryItem::CopyFromDataTableRow(const FItemDataTableRow& RowData)
{
    ItemID = RowData.ItemID;
    ItemName = RowData.ItemName;
    Description = RowData.Description;
    ItemType = RowData.ItemType;
    Rarity = RowData.Rarity;
    EquipmentSlot = RowData.EquipmentSlot;
    
    ItemSize = RowData.ItemSize;
    Weight = RowData.Weight;
    MaxStackSize = RowData.MaxStackSize;
    StackSize = 1;
    
    MaxDurability = RowData.MaxDurability;
    CurrentDurability = MaxDurability;
    
    Value = RowData.Value;
    
    bIsContainer = RowData.bIsContainer;
    WeightReduction = RowData.WeightReduction;
    
    IconPath = RowData.IconPath;
    MeshPath = RowData.MeshPath;
    
    if (bIsContainer)
    {
        InitializeContainer((int32)RowData.ContainerSize.X, (int32)RowData.ContainerSize.Y);
    }
}
