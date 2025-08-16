# Implementing the Spatial Inventory System in Unreal Engine

## Step-by-Step Integration Guide

### 1. Project Setup

#### Create or Prepare Your Unreal Project
1. Open Unreal Engine and create a new **C++ Third Person** project
2. Name your project (e.g., "SpatialInventoryGame")
3. Choose **C++** as the project type
4. Click **Create Project**

#### Module Configuration
1. Open your project's `.uproject` file in a text editor
2. Ensure it includes:
```json
{
    "FileVersion": 3,
    "EngineAssociation": "5.3",
    "Category": "",
    "Description": "",
    "Modules": [
        {
            "Name": "YourProjectName",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": [
                "Engine",
                "CoreUObject",
                "UMG",
                "Slate",
                "SlateCore"
            ]
        }
    ]
}
```

### 2. Adding the Source Files

#### Directory Structure
Create the following folder structure in your project's `Source/YourProjectName/` directory:
```
Source/YourProjectName/
├── InventorySystemCore/
│   ├── BP_InventoryComponent.h
│   ├── BP_InventoryComponent.cpp
│   ├── InventoryStructs.h
│   ├── InventoryItem.h
│   └── InventoryItem.cpp
├── UI/
│   └── InventoryUI/
│       ├── WBP_InventoryGrid.h
│       ├── WBP_InventoryGrid.cpp
│       ├── WBP_InventorySlot.h
│       ├── WBP_InventorySlot.cpp
│       ├── WBP_EquipmentSlot.h
│       ├── WBP_MainInventory.h
│       ├── ItemDragDropOperation.h
│       └── ItemDragDropOperation.cpp
├── Blueprints/
│   ├── BP_InventoryManager.h
│   ├── BP_InventoryManager.cpp
│   └── BP_InventoryPlayerController.h
└── Examples/
    ├── BP_ExampleCharacter.h
    └── BP_ExampleCharacter.cpp
```

#### Copy Source Files
1. Copy all the `.h` and `.cpp` files from this project into your Unreal project's corresponding directories
2. Update the `#include` statements in each file to match your project name:

**Replace this line in all files:**
```cpp
#include "CoreMinimal.h"
```

**With your project name:**
```cpp
#include "CoreMinimal.h"
// Add your project's generated include
#include "YourProjectName.h"  // Replace with your actual project name
```

### 3. Build Configuration

#### Update Build.cs File
Open your project's `Source/YourProjectName/YourProjectName.Build.cs` file and modify it:

```csharp
using UnrealBuildTool;

public class YourProjectName : ModuleRules
{
    public YourProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ApplicationCore"
        });

        // Enable Blueprint support
        PublicIncludePaths.AddRange(new string[] {
            "Runtime/UMG/Public",
            "Runtime/Slate/Public"
        });
    }
}
```

### 4. Header File Updates

#### Fix Include Paths
Update all include statements in the source files to use your project structure:

**Example for BP_InventoryComponent.h:**
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventorySystemCore/InventoryStructs.h"
#include "BP_InventoryComponent.generated.h"
```

**Make sure the macro at the bottom uses your project name:**
```cpp
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOURPROJECTNAME_API UBP_InventoryComponent : public UActorComponent
```

### 5. Generate Project Files

#### Windows (Visual Studio):
1. Right-click your `.uproject` file
2. Select **Generate Visual Studio project files**
3. Wait for the process to complete

#### Mac (Xcode):
```bash
cd /path/to/your/project
/Applications/Epic\ Games/UE_5.3/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.exe -projectfiles -project="/path/to/your/project/YourProject.uproject" -game -rocket -progress
```

### 6. Compile the Code

#### In Visual Studio/Xcode:
1. Open the generated `.sln` (Windows) or `.xcworkspace` (Mac) file
2. Set the configuration to **Development Editor**
3. Build the project (Ctrl+Shift+B in Visual Studio)

#### Alternative - Compile in Unreal:
1. Open your project in Unreal Engine
2. Click **Compile** button in the toolbar
3. Wait for compilation to complete

### 7. Create Blueprint Assets

#### Data Table Setup:
1. In Content Browser, right-click → **Miscellaneous** → **Data Table**
2. Choose **FItemDataTableRow** as the Row Structure
3. Name it `DT_ItemDatabase`
4. Import the provided `ItemDatabase.csv` file

#### Widget Blueprint Creation:
1. Create **Widget Blueprint** assets for each UI component:
   - `WBP_MainInventory`
   - `WBP_InventoryGrid`
   - `WBP_InventorySlot`
   - `WBP_EquipmentSlot`

2. Set their **Parent Class** to the corresponding C++ classes:
   - WBP_MainInventory → UBP_MainInventory
   - WBP_InventoryGrid → UBP_InventoryGrid
   - etc.

#### Blueprint Class Creation:
1. Create **Blueprint Classes** based on C++ classes:
   - `BP_InventoryManager` (parent: ABP_InventoryManager)
   - `BP_PlayerCharacter` (parent: ABP_ExampleCharacter)
   - `BP_GamePlayerController` (parent: ABP_InventoryPlayerController)

### 8. Character Integration

#### Method 1: Modify Existing Character
Add to your existing character's header file:
```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
class UBP_InventoryComponent* InventoryComponent;
```

In the constructor:
```cpp
InventoryComponent = CreateDefaultSubobject<UBP_InventoryComponent>(TEXT("InventoryComponent"));
```

#### Method 2: Use Example Character
1. Use the provided `BP_ExampleCharacter` as your player character
2. Set it as your default pawn in **Game Mode**

### 9. UI Setup

#### Design the Widget Blueprints:
1. **WBP_MainInventory**: Create the main layout with panels for equipment and inventory
2. **WBP_InventoryGrid**: Design the grid background and cell structure
3. **WBP_InventorySlot**: Design individual item slots with icon, stack text, etc.
4. **WBP_EquipmentSlot**: Design equipment-specific slots

#### Bind Widget Components:
Ensure widget names in Blueprint match the `meta = (BindWidget)` names in C++:
- `GridCanvas`
- `GridBackground`
- `SlotBorder`
- `ItemIcon`
- etc.

### 10. Input Setup

#### Project Settings:
1. Go to **Edit** → **Project Settings** → **Input**
2. Add Action Mappings:
   - `ToggleInventory` (default: Tab key)
   - `QuickUse` (default: E key)

#### PlayerController Setup:
Use the provided `BP_InventoryPlayerController` or integrate input handling into your existing controller.

### 11. Game Mode Configuration

1. Create a **Game Mode Blueprint** based on `AGameModeBase`
2. Set the **Default Pawn Class** to your character with inventory
3. Set the **Player Controller Class** to your inventory-enabled controller
4. Apply this Game Mode to your level or set as project default

### 12. Testing

#### Basic Test:
1. Play the game
2. Press Tab to open inventory
3. Create test items in BeginPlay:
```cpp
void AYourCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Create test items
    if (ABP_InventoryManager* Manager = GetWorld()->SpawnActor<ABP_InventoryManager>())
    {
        UInventoryItem* TestSword = Manager->CreateItemFromID("BasicSword");
        UInventoryItem* TestBackpack = Manager->CreateItemFromID("SmallBackpack");
        
        if (InventoryComponent)
        {
            InventoryComponent->AddItemToInventory(TestSword);
            InventoryComponent->AddItemToInventory(TestBackpack);
        }
    }
}
```

### 13. Common Issues and Solutions

#### Compilation Errors:
- **Missing includes**: Add required headers to each file
- **API macro errors**: Ensure `YOURPROJECTNAME_API` matches your project name
- **Generated header not found**: Make sure `.generated.h` files are included last

#### Runtime Issues:
- **Widget not showing**: Check parent class assignment in Blueprint
- **Drag-drop not working**: Verify widget binding names match C++ variables
- **Items not creating**: Ensure Data Table is properly set up and referenced

#### Performance Issues:
- **Large grids**: Consider viewport culling for very large inventories
- **Many items**: Implement object pooling for item widgets

### 14. Customization

#### Custom Item Types:
1. Inherit from `UInventoryItem`
2. Add custom properties and functions
3. Register with the item factory

#### Custom Equipment Stats:
```cpp
// In item creation
NewItem->StatModifiers.Add(TEXT("Damage"), 10.0f);
NewItem->StatModifiers.Add(TEXT("Defense"), 5.0f);
```

#### Custom UI Themes:
Modify widget blueprints to match your game's visual style while maintaining functionality.

### 15. Packaging for Distribution

1. **Package Settings**: Ensure all content is included in packaging
2. **Cook Settings**: Include all widget blueprints and data tables
3. **Test Build**: Always test in a packaged build before distribution

## Summary

This integration process will give you a fully functional spatial inventory system in your Unreal Engine project. The system is designed to be modular and extensible, allowing you to customize it for your specific game requirements while maintaining the core functionality.

For additional help or specific customizations, refer to the included documentation and example files.