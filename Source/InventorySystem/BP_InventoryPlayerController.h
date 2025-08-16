#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventorySystem.h"
#include "BP_InventoryPlayerController.generated.h"

// Forward declarations
class UBP_InventoryComponent;

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API ABP_InventoryPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABP_InventoryPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // UI References
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> InventoryUIClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* InventoryUI;

    // Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FName ToggleInventoryInputAction = "ToggleInventory";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FName QuickUseInputAction = "QuickUse";

    // Inventory Management
    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    class UBP_InventoryComponent* PlayerInventoryComponent;

    // UI State
    UPROPERTY(BlueprintReadOnly, Category = "UI State")
    bool bIsInventoryOpen;

public:
    // Input Functions
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ToggleInventory();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void QuickUse();

    // UI Management
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OpenInventory();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseInventory();

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsInventoryOpen() const { return bIsInventoryOpen; }

    // Inventory Access
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    class UBP_InventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent; }

private:
    void FindPlayerInventoryComponent();
};
