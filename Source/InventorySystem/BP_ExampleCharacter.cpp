#include "BP_ExampleCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryItem.h"
#include "BP_InventoryComponent.h"
#include "InventorySystem.h"

ABP_ExampleCharacter::ABP_ExampleCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create inventory component
    InventoryComponent = CreateDefaultSubobject<UBP_InventoryComponent>(TEXT("InventoryComponent"));

    // Initialize stats
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MaxMana = 100.0f;
    CurrentMana = MaxMana;
    CarryCapacity = 100.0f;
    CurrentWeight = 0.0f;

    // Initialize base stats
    BaseStrength = 10.0f;
    BaseDefense = 10.0f;
    BaseSpeed = 10.0f;
    BaseIntelligence = 10.0f;
}

void ABP_ExampleCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Bind to inventory events
    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryChanged.AddDynamic(this, &ABP_ExampleCharacter::OnInventoryChanged);
        InventoryComponent->OnEquipmentChanged.AddDynamic(this, &ABP_ExampleCharacter::OnEquipmentChanged);
        
        // Update initial weight
        UpdateCurrentWeight();
    }

    // Create some test items for demonstration
    CreateTestItems();
}

void ABP_ExampleCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update weight regularly
    UpdateCurrentWeight();

    // Apply movement speed modifier based on weight
    if (IsOverweight())
    {
        GetCharacterMovement()->MaxWalkSpeed = 300.0f; // Slower when overweight
    }
    else
    {
        float SpeedModifier = GetTotalStat(TEXT("Speed"));
        GetCharacterMovement()->MaxWalkSpeed = 600.0f + (SpeedModifier * 10.0f);
    }
}

void ABP_ExampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Input is handled by the PlayerController
}

float ABP_ExampleCharacter::GetTotalStat(const FString& StatName)
{
    float BaseStat = 0.0f;

    // Get base stat
    if (StatName == TEXT("Strength"))
        BaseStat = BaseStrength;
    else if (StatName == TEXT("Defense"))
        BaseStat = BaseDefense;
    else if (StatName == TEXT("Speed"))
        BaseStat = BaseSpeed;
    else if (StatName == TEXT("Intelligence"))
        BaseStat = BaseIntelligence;

    // Add equipment modifiers
    if (InventoryComponent)
    {
        BaseStat += InventoryComponent->GetEquipmentStatModifier(StatName);
    }

    return BaseStat;
}

void ABP_ExampleCharacter::UpdateCurrentWeight()
{
    if (InventoryComponent)
    {
        CurrentWeight = InventoryComponent->GetTotalWeight();
    }
}

bool ABP_ExampleCharacter::IsOverweight()
{
    return CurrentWeight > CarryCapacity;
}

float ABP_ExampleCharacter::GetWeightPercentage()
{
    if (CarryCapacity <= 0.0f)
        return 0.0f;
        
    return (CurrentWeight / CarryCapacity) * 100.0f;
}

void ABP_ExampleCharacter::ApplyDamage(float DamageAmount)
{
    float Defense = GetTotalStat(TEXT("Defense"));
    float ActualDamage = FMath::Max(1.0f, DamageAmount - (Defense * 0.5f));
    
    CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);
    
    if (CurrentHealth <= 0.0f)
    {
        // Handle death
        UE_LOG(LogInventorySystem, Warning, TEXT("Character died!"));
    }
}

void ABP_ExampleCharacter::Heal(float HealAmount)
{
    CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
}

bool ABP_ExampleCharacter::UseMana(float ManaAmount)
{
    if (CurrentMana >= ManaAmount)
    {
        CurrentMana -= ManaAmount;
        return true;
    }
    return false;
}

void ABP_ExampleCharacter::RestoreMana(float ManaAmount)
{
    CurrentMana = FMath::Min(MaxMana, CurrentMana + ManaAmount);
}

void ABP_ExampleCharacter::CreateTestItems()
{
    if (!InventoryComponent)
        return;

    // Create a basic sword
    UInventoryItem* Sword = NewObject<UInventoryItem>(this);
    Sword->ItemID = TEXT("BasicSword");
    Sword->ItemName = TEXT("Iron Sword");
    Sword->Description = TEXT("A sturdy iron sword");
    Sword->ItemType = EItemType::Weapon;
    Sword->Rarity = EItemRarity::Common;
    Sword->EquipmentSlot = EEquipmentSlot::PrimaryWeapon;
    Sword->ItemSize = FVector2D(1, 3);
    Sword->Weight = 3.0f;
    Sword->MaxStackSize = 1;
    Sword->StackSize = 1;
    Sword->Value = 50;
    Sword->StatModifiers.Add(TEXT("Strength"), 5.0f);
    
    // Create a backpack
    UInventoryItem* Backpack = NewObject<UInventoryItem>(this);
    Backpack->ItemID = TEXT("SmallBackpack");
    Backpack->ItemName = TEXT("Small Backpack");
    Backpack->Description = TEXT("A small leather backpack");
    Backpack->ItemType = EItemType::Container;
    Backpack->Rarity = EItemRarity::Common;
    Backpack->EquipmentSlot = EEquipmentSlot::Backpack;
    Backpack->ItemSize = FVector2D(2, 2);
    Backpack->Weight = 2.0f;
    Backpack->MaxStackSize = 1;
    Backpack->StackSize = 1;
    Backpack->Value = 25;
    Backpack->bIsContainer = true;
    Backpack->WeightReduction = 0.2f; // 20% weight reduction
    Backpack->InitializeContainer(6, 6);
    
    // Create some potions
    UInventoryItem* HealthPotion = NewObject<UInventoryItem>(this);
    HealthPotion->ItemID = TEXT("HealthPotion");
    HealthPotion->ItemName = TEXT("Health Potion");
    HealthPotion->Description = TEXT("Restores 50 health");
    HealthPotion->ItemType = EItemType::Consumable;
    HealthPotion->Rarity = EItemRarity::Common;
    HealthPotion->EquipmentSlot = EEquipmentSlot::None;
    HealthPotion->ItemSize = FVector2D(1, 1);
    HealthPotion->Weight = 0.5f;
    HealthPotion->MaxStackSize = 10;
    HealthPotion->StackSize = 3;
    HealthPotion->Value = 15;

    // Add items to inventory
    InventoryComponent->AddItemToInventory(Sword);
    InventoryComponent->AddItemToInventory(Backpack);
    InventoryComponent->AddItemToInventory(HealthPotion);

    UE_LOG(LogInventorySystem, Warning, TEXT("Created test items for character"));
}

void ABP_ExampleCharacter::OnInventoryChanged(int32 SlotIndex, UInventoryItem* Item)
{
    UpdateCurrentWeight();
    UpdateStatsFromEquipment();
}

void ABP_ExampleCharacter::OnEquipmentChanged(EEquipmentSlot SlotType, UInventoryItem* Item)
{
    UpdateStatsFromEquipment();
    
    if (Item)
    {
        UE_LOG(LogInventorySystem, Warning, TEXT("Equipped: %s"), *Item->ItemName);
    }
    else
    {
        UE_LOG(LogInventorySystem, Warning, TEXT("Unequipped item from slot"));
    }
}

void ABP_ExampleCharacter::UpdateStatsFromEquipment()
{
    float TotalStrength = GetTotalStat(TEXT("Strength"));
    float TotalDefense = GetTotalStat(TEXT("Defense"));
    
    UE_LOG(LogInventorySystem, Log, TEXT("Total Strength: %.1f, Total Defense: %.1f"), 
        TotalStrength, TotalDefense);
}
