# Quick Start Guide - 15 Minutes to Working Inventory

This guide will get you up and running with the spatial inventory system in just 15 minutes.

## Prerequisites
- Unreal Engine 5.1 or newer
- Basic C++ project (not Blueprint-only)
- Visual Studio or Xcode

## Step 1: Project Setup (2 minutes)

### New Project
1. Create new **C++ Third Person** project
2. Name it anything you want
3. Open the project

### Existing Project
1. Ensure your project has C++ support
2. If Blueprint-only, convert to C++ by adding a C++ class

## Step 2: Copy Source Files (3 minutes)

### Download and Copy
1. Download all files from this repository
2. Copy the entire folder structure to your project:
   ```
   YourProject/Source/YourProjectName/
   ├── InventorySystemCore/     (copy all files here)
   ├── UI/InventoryUI/         (copy all files here)
   ├── Blueprints/             (copy all files here)
   └── Examples/               (copy all files here)
   ```

### Update Project Name
3. Open each `.h` file and replace `SPATIALINVENTORY_API` with `YOURPROJECTNAME_API`
   - Use Find & Replace in your IDE
   - Replace `SPATIALINVENTORY` with your actual project name (uppercase)

## Step 3: Update Build File (1 minute)

Open `Source/YourProjectName/YourProjectName.Build.cs` and replace with:

```csharp
using UnrealBuildTool;

public class YourProjectName : ModuleRules  // Replace YourProjectName
{
    public YourProjectName(ReadOnlyTargetRules Target) : base(Target)  // Replace YourProjectName
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ApplicationCore"
        });
    }
}
```

## Step 4: Generate & Compile (3 minutes)

1. **Generate Project Files**: Right-click your `.uproject` file → "Generate Visual Studio project files"
2. **Open in IDE**: Open the generated `.sln` file
3. **Compile**: Build the project (Ctrl+Shift+B)

## Step 5: Create Blueprint Assets (4 minutes)

### Data Table
1. In Content Browser: Right-click → Miscellaneous → **Data Table**
2. Choose **FItemDataTableRow** as Row Structure
3. Name it `DT_ItemDatabase`
4. Open it and import the `ItemDatabase.csv` file

### Widget Blueprints
Create these Widget Blueprints:
1. `WBP_MainInventory` (Parent Class: **WBP Main Inventory**)
2. `WBP_InventoryGrid` (Parent Class: **WBP Inventory Grid**)
3. `WBP_InventorySlot` (Parent Class: **WBP Inventory Slot**)
4. `WBP_EquipmentSlot` (Parent Class: **WBP Equipment Slot**)

### Character Blueprint
1. Create Blueprint based on **BP Example Character**
2. Name it `BP_PlayerCharacter`

### Game Mode
1. Create Blueprint based on **Game Mode Base**
2. Set **Default Pawn Class** to `BP_PlayerCharacter`
3. In World Settings, set this as the **Game Mode Override**

## Step 6: Quick Test (2 minutes)

### Add Test Items
In your character's BeginPlay (or create a simple Blueprint node):

**Blueprint Version:**
1. Open `BP_PlayerCharacter`
2. In BeginPlay, add these nodes:
   - **Spawn Actor from Class** → `BP_InventoryManager`
   - **Create Item From ID** → ItemID: "BasicSword"
   - **Get Inventory Component** → **Add Item To Inventory**

### Test Input
1. **Project Settings** → **Input**
2. Add Action Mapping: `ToggleInventory` mapped to **Tab** key

### Play Test
1. Press **Play**
2. Press **Tab** to open inventory
3. You should see the inventory interface with a test sword

## Troubleshooting

### Common Issues:

**"Class not found" errors:**
- Make sure you replaced `SPATIALINVENTORY_API` with your project name
- Regenerate project files and recompile

**Widget not appearing:**
- Check that Widget Blueprint parent classes are set correctly
- Verify widget names match the C++ `meta = (BindWidget)` names

**Compilation errors:**
- Ensure all dependencies are added to Build.cs
- Check that all include paths are correct

## Next Steps

Once you have the basic system working:

1. **Customize UI**: Edit the Widget Blueprints to match your game's style
2. **Add More Items**: Expand the Data Table with your own items
3. **Configure Equipment**: Set up equipment stats and restrictions
4. **Style Integration**: Match the inventory to your game's visual theme

## Full Documentation

For detailed customization and advanced features, see:
- `Documentation/InventorySystemGuide.md`
- `Documentation/UnrealEngineSetup.md`
- Interactive demo at the main project page

## Support

If you encounter issues:
1. Check the full documentation
2. Verify all steps were followed correctly
3. Ensure your project has C++ support enabled
4. Try the example character first before integrating with existing systems

The system is designed to work out-of-the-box with minimal configuration. Most issues are related to file paths or project name mismatches.