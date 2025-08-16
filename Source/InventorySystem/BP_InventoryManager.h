#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryStructs.h"                // FItemDataTableRow lives here (USTRUCT, needs full include)
#include "BP_InventoryManager.generated.h"

// Forward declarations to avoid pulling heavy headers in a .h
class UDataTable;
class UInventoryItem;

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API ABP_InventoryManager : public AActor
{
    GENERATED_BODY()
    
public:
    ABP_InventoryManager();

protected:
    virtual void BeginPlay() override;

    // Data Tables
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Data")
    UDataTable* ItemDataTable = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Data")
    UDataTable* EquipmentDataTable = nullptr;

    // Optional overrides: map ItemID -> custom UInventoryItem subclass
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Data")
    TMap<FString, TSubclassOf<UInventoryItem>> ItemClasses;

public:
    virtual void Tick(float DeltaTime) override;

    // Item Factory Functions
    UFUNCTION(BlueprintCallable, Category="Inventory|Factory")
    UInventoryItem* CreateItemFromID(const FString& ItemID);

    UFUNCTION(BlueprintCallable, Category="Inventory|Factory")
    UInventoryItem* CreateItemFromDataTable(const FString& RowName);

    UFUNCTION(BlueprintCallable, Category="Inventory|Factory")
    TArray<UInventoryItem*> CreateMultipleItems(const FString& ItemID, int32 Quantity);

    // Data Table Functions
    UFUNCTION(BlueprintCallable, Category="Inventory|Data")
    bool LoadItemDataTable(const FString& DataTablePath);

    UFUNCTION(BlueprintCallable, Category="Inventory|Data")
    TArray<FString> GetAllItemIDs();

    UFUNCTION(BlueprintCallable, Category="Inventory|Data")
    FItemDataTableRow GetItemData(const FString& ItemID);

    // Manager Access
    UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContext"), Category="Inventory|Manager")
    static ABP_InventoryManager* GetInventoryManager(const UObject* WorldContext);

private:
    // Helper
    UInventoryItem* CreateItemFromRowData(const FItemDataTableRow& RowData);
};
