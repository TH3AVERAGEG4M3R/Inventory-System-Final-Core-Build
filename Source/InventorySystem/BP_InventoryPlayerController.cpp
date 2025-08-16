#include "BP_InventoryPlayerController.h"
#include "BP_InventoryComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "BP_ExampleCharacter.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.h"

ABP_InventoryPlayerController::ABP_InventoryPlayerController()
{
    bIsInventoryOpen = false;
    InventoryUI = nullptr;
    PlayerInventoryComponent = nullptr;
}

void ABP_InventoryPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Find the player's inventory component
    FindPlayerInventoryComponent();
    
    UE_LOG(LogInventorySystem, Warning, TEXT("Inventory Player Controller initialized"));
}

void ABP_InventoryPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Bind input actions
    if (InputComponent)
    {
        InputComponent->BindAction(ToggleInventoryInputAction, IE_Pressed, this, &ABP_InventoryPlayerController::ToggleInventory);
        InputComponent->BindAction(QuickUseInputAction, IE_Pressed, this, &ABP_InventoryPlayerController::QuickUse);
    }
}

void ABP_InventoryPlayerController::ToggleInventory()
{
    if (bIsInventoryOpen)
    {
        CloseInventory();
    }
    else
    {
        OpenInventory();
    }
}

void ABP_InventoryPlayerController::QuickUse()
{
    UE_LOG(LogInventorySystem, Warning, TEXT("Quick use pressed"));
    // Implement quick use functionality here
}

void ABP_InventoryPlayerController::OpenInventory()
{
    if (bIsInventoryOpen)
        return;
        
    // Create UI if it doesn't exist
    if (!InventoryUI && InventoryUIClass)
    {
        InventoryUI = CreateWidget<UUserWidget>(this, InventoryUIClass);
    }
    
    if (InventoryUI)
    {
        InventoryUI->AddToViewport();
        bIsInventoryOpen = true;
        
        // Set input mode to UI only
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(InventoryUI->TakeWidget());
        SetInputMode(InputMode);
        
        // Show mouse cursor
        bShowMouseCursor = true;
        
        UE_LOG(LogInventorySystem, Warning, TEXT("Inventory opened"));
    }
    else
    {
        UE_LOG(LogInventorySystem, Warning, TEXT("Failed to open inventory - UI class not set or widget creation failed"));
    }
}

void ABP_InventoryPlayerController::CloseInventory()
{
    if (!bIsInventoryOpen)
        return;
        
    if (InventoryUI)
    {
        InventoryUI->RemoveFromParent();
        bIsInventoryOpen = false;
        
        // Set input mode back to game only
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        
        // Hide mouse cursor
        bShowMouseCursor = false;
        
        UE_LOG(LogInventorySystem, Warning, TEXT("Inventory closed"));
    }
}

void ABP_InventoryPlayerController::FindPlayerInventoryComponent()
{
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        PlayerInventoryComponent = ControlledPawn->FindComponentByClass<UBP_InventoryComponent>();
        
        if (PlayerInventoryComponent)
        {
            UE_LOG(LogInventorySystem, Warning, TEXT("Found player inventory component"));
        }
        else
        {
            UE_LOG(LogInventorySystem, Warning, TEXT("Player pawn does not have an inventory component"));
        }
    }
}
