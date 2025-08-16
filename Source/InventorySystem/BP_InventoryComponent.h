#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryStructs.h"          // Needed for FInventoryGrid / FEquipmentSlotData / EEquipmentSlot
#include "BP_InventoryComponent.generated.h"

// Forward declare to avoid pulling InventoryItem.h here
class UInventoryItem;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, int32, SlotIndex, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, SlotType, UInventoryItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UBP_InventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBP_InventoryComponent();

protected:
    virtual void BeginPlay() override;

    // Equipment Slots
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
    TMap<EEquipmentSlot, FEquipmentSlotData> EquipmentSlots;

    // Main Inventory Grid
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    FInventoryGrid MainInventory;

    // Container Inventories (backpack, vest, clothing items)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TMap<FString, FInventoryGrid> ContainerInventories;

public:
    // Equipment Management
    UFUNCTION(BlueprintCallable, Category="Equipment")
    bool EquipItem(UInventoryItem* Item, EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category="Equipment")
    bool UnequipItem(EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category="Equipment")
    UInventoryItem* GetEquippedItem(EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category="Equipment")
    bool CanEquipItem(UInventoryItem* Item, EEquipmentSlot SlotType);

    // Inventory Management
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool AddItemToInventory(UInventoryItem* Item);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool AddItemToContainer(UInventoryItem* Item, const FString& ContainerName);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemFromInventory(UInventoryItem* Item);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemFromContainer(UInventoryItem* Item, const FString& ContainerName);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    TArray<UInventoryItem*> GetAllItems();

    UFUNCTION(BlueprintCallable, Category="Inventory")
    UInventoryItem* FindItemByID(const FString& ItemID);

    // Grid Management
    UFUNCTION(BlueprintCallable, Category="Grid")
    bool CanPlaceItemAt(UInventoryItem* Item, FVector2D Position, const FString& ContainerName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category="Grid")
    bool PlaceItemAt(UInventoryItem* Item, FVector2D Position, const FString& ContainerName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category="Grid")
    void RemoveItemAt(FVector2D Position, const FString& ContainerName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category="Grid")
    UInventoryItem* GetItemAt(FVector2D Position, const FString& ContainerName = TEXT(""));

    // Container Management
    UFUNCTION(BlueprintCallable, Category="Container")
    void CreateContainer(const FString& ContainerName, int32 Width, int32 Height);

    UFUNCTION(BlueprintCallable, Category="Container")
    void RemoveContainer(const FString& ContainerName);

    UFUNCTION(BlueprintCallable, Category="Container")
    TArray<FString> GetContainerNames();

    UFUNCTION(BlueprintCallable, Category="Container")
    FInventoryGrid GetContainer(const FString& ContainerName);

    // Stats and Weight
    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetTotalWeight();

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetEquipmentStatModifier(const FString& StatName);

    UFUNCTION(BlueprintCallable, Category="Stats")
    TMap<FString, float> GetAllStatModifiers();

    // Events
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnInventoryChanged OnInventoryChanged;

    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnEquipmentChanged OnEquipmentChanged;

    // Utility
    UFUNCTION(BlueprintCallable, Category="Utility")
    void InitializeEquipmentSlots();

private:
    // Internal helpers
    void UpdateContainersFromEquipment();
    FInventoryGrid& GetInventoryGrid(const FString& ContainerName);
    void MarkCellsOccupied(FInventoryGrid& Grid, FVector2D Position, FVector2D Size, bool bOccupied);
    bool AreCellsAvailable(FInventoryGrid& Grid, FVector2D Position, FVector2D Size);
    int32 GetGridIndex(int32 X, int32 Y, int32 Width);
};
