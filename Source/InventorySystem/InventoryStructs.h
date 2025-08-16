#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryStructs.generated.h"

// Forward declarations (avoid pulling heavy headers here)
class UInventoryItem;

// Equipment Slot Types
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    None            UMETA(DisplayName="None"),
    Helmet          UMETA(DisplayName="Helmet"),
    Chest           UMETA(DisplayName="Chest Armor"),
    Legs            UMETA(DisplayName="Leg Armor"),
    Boots           UMETA(DisplayName="Boots"),
    Gloves          UMETA(DisplayName="Gloves"),
    Backpack        UMETA(DisplayName="Backpack"),
    Vest            UMETA(DisplayName="Vest"),
    PrimaryWeapon   UMETA(DisplayName="Primary Weapon"),
    SecondaryWeapon UMETA(DisplayName="Secondary Weapon"),
    Melee           UMETA(DisplayName="Melee Weapon"),
    Accessory1      UMETA(DisplayName="Accessory 1"),
    Accessory2      UMETA(DisplayName="Accessory 2")
};

// Item Rarity
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common      UMETA(DisplayName="Common"),
    Uncommon    UMETA(DisplayName="Uncommon"),
    Rare        UMETA(DisplayName="Rare"),
    Epic        UMETA(DisplayName="Epic"),
    Legendary   UMETA(DisplayName="Legendary")
};

// Item Type
UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon      UMETA(DisplayName="Weapon"),
    Armor       UMETA(DisplayName="Armor"),
    Consumable  UMETA(DisplayName="Consumable"),
    Container   UMETA(DisplayName="Container"),
    Material    UMETA(DisplayName="Material"),
    Tool        UMETA(DisplayName="Tool"),
    Accessory   UMETA(DisplayName="Accessory"),
    Quest       UMETA(DisplayName="Quest Item")
};

// Inventory Grid Structure
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInventoryGrid
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
    int32 Width = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
    int32 Height = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    TArray<bool> OccupiedCells;

    // Items placed in this grid (position tracking simplified elsewhere)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    TArray<UInventoryItem*> Items;

    // Track bottom-left grid position of each item
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    TMap<UInventoryItem*, FIntPoint> ItemPositions;

    FInventoryGrid()
    {
        OccupiedCells.Init(false, Width * Height);
    }

    FInventoryGrid(int32 InWidth, int32 InHeight)
        : Width(InWidth), Height(InHeight)
    {
        OccupiedCells.Init(false, Width * Height);
    }
};

// Equipment Slot Data
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FEquipmentSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
    EEquipmentSlot SlotType = EEquipmentSlot::None;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
    UInventoryItem* EquippedItem = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
    bool bIsLocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
    TArray<EItemType> AllowedItemTypes;
};

// Data Table Row for Items
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemDataTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    EItemType ItemType = EItemType::Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    EItemRarity Rarity = EItemRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Data")
    EEquipmentSlot EquipmentSlot = EEquipmentSlot::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical")
    FVector2D ItemSize = FVector2D(1, 1);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical")
    float Weight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical")
    int32 MaxStackSize = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MaxDurability = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy")
    int32 Value = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Container")
    bool bIsContainer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Container")
    FVector2D ContainerSize = FVector2D(0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Container")
    float WeightReduction = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
    FString MeshPath;
};
