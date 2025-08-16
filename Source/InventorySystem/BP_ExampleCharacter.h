#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BP_ExampleCharacter.generated.h"

// Forward declarations to avoid include loops
class UBP_InventoryComponent;
class UInventoryItem;

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API ABP_ExampleCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABP_ExampleCharacter();

protected:
    virtual void BeginPlay() override;

    // Inventory Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UBP_InventoryComponent* InventoryComponent;

    // Character Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxMana;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float CurrentMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CarryCapacity;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float CurrentWeight;

    // Base Stats (before equipment modifiers)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseDefense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseIntelligence;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Stat Functions
    UFUNCTION(BlueprintCallable, Category = "Stats")
    float GetTotalStat(const FString& StatName);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void UpdateCurrentWeight();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    bool IsOverweight();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    float GetWeightPercentage();

    // Health and Mana
    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(float DamageAmount); // renamed from TakeDamage

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Mana")
    bool UseMana(float ManaAmount);

    UFUNCTION(BlueprintCallable, Category = "Mana")
    void RestoreMana(float ManaAmount);

    // Inventory Access
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    UBP_InventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

    // Test Functions for Demo
    UFUNCTION(BlueprintCallable, Category = "Testing", CallInEditor)
    void CreateTestItems();

private:
    // Event handlers
    UFUNCTION()
    void OnInventoryChanged(int32 SlotIndex, UInventoryItem* Item);

    UFUNCTION()
    void OnEquipmentChanged(EEquipmentSlot SlotType, UInventoryItem* Item);

    void UpdateStatsFromEquipment();
};
