#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySystem.h"
#include "InventoryStructs.h"
#include "InventoryItem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UInventoryItem : public UObject
{
    GENERATED_BODY()

public:
    UInventoryItem();

    // Core Item Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    EItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    EEquipmentSlot EquipmentSlot;

    // Physical Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
    FVector2D ItemSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
    int32 StackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
    int32 MaxStackSize;

    // Durability System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Durability")
    float CurrentDurability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Durability")
    float MaxDurability;

    // Economy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    int32 Value;

    // Container Properties (for backpacks, vests, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
    bool bIsContainer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
    FInventoryGrid ContainerInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
    float WeightReduction; // Percentage of weight reduction for items inside

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
    TArray<EItemType> AllowedItemTypes; // What types of items can be stored

    // Stat Modifiers (for equipment)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    TMap<FString, float> StatModifiers;

    // Visual
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    class UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    class UStaticMesh* ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FString MeshPath;

public:
    // Item Functions
    UFUNCTION(BlueprintCallable, Category = "Item")
    bool CanStackWith(const UInventoryItem* OtherItem) const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    int32 AddToStack(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Item")
    int32 RemoveFromStack(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Item")
    float GetDurabilityPercentage() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void RepairItem(float RepairAmount);

    UFUNCTION(BlueprintCallable, Category = "Item")
    bool IsStackFull() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    bool IsBroken() const;

    // Container Functions
    UFUNCTION(BlueprintCallable, Category = "Container")
    bool CanStoreItem(const UInventoryItem* Item) const;

    UFUNCTION(BlueprintCallable, Category = "Container")
    float GetTotalWeight() const;

    UFUNCTION(BlueprintCallable, Category = "Container")
    void InitializeContainer(int32 Width, int32 Height);

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Item")
    FLinearColor GetRarityColor() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    FString GetFormattedDescription() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    void CopyFromDataTableRow(const FItemDataTableRow& RowData);
};
