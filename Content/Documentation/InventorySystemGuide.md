# Spatial Inventory System for Unreal Engine

## Overview

This spatial inventory system provides a comprehensive solution for managing equipment, clothing, and items in Unreal Engine projects. The system features a grid-based spatial inventory, equipment slots, and clothing items with their own customizable inventory spaces.

## Key Features

### 1. Equipment System
- **12 Equipment Slots**: Helmet, Chest, Legs, Boots, Gloves, Backpack, Vest, Primary Weapon, Secondary Weapon, Melee, and 2 Accessory slots
- **Dynamic Slot Configuration**: Each slot can be configured to accept specific item types
- **Equipment Stats**: Items can provide stat modifications when equipped

### 2. Spatial Inventory
- **Grid-Based Layout**: Items occupy space based on their size (width x height)
- **Drag and Drop**: Full drag-and-drop support for moving items
- **Visual Feedback**: Highlight valid/invalid drop zones during dragging
- **Multiple Containers**: Support for multiple inventory containers

### 3. Clothing with Inventory Slots
- **Container Items**: Backpacks, vests, and clothing can have their own inventory spaces
- **Configurable Sizes**: Each container can define its own width and height
- **Weight Limits**: Containers can have weight restrictions
- **Item Type Filters**: Restrict what types of items can be stored in specific containers

### 4. Item Management
- **Item Factory**: Create items from data tables or templates
- **Rarity System**: Common, Uncommon, Rare, Epic, Legendary
- **Durability System**: Items can degrade over time
- **Stacking**: Support for stackable items with configurable stack sizes

## Core Components

### BP_InventoryComponent
The main component that handles all inventory logic:
```cpp
// Add to your character
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
UBP_InventoryComponent* InventoryComponent;
```

### UInventoryItem
Base class for all items in the system:
```cpp
// Core properties
FString ItemName;
EItemType ItemType;
EItemRarity Rarity;
FVector2D ItemSize;
float Weight;
```

### UI Components
- **WBP_MainInventory**: Main inventory interface
- **WBP_InventoryGrid**: Grid-based inventory display
- **WBP_InventorySlot**: Individual item slots
- **WBP_EquipmentSlot**: Equipment slot widgets

## Setup Instructions

### 1. Add Components to Your Character
```cpp
// In your character class constructor
InventoryComponent = CreateDefaultSubobject<UBP_InventoryComponent>(TEXT("InventoryComponent"));
```

### 2. Create Item Data Table
1. Create a new Data Table asset
2. Use `FItemDataTableRow` as the row structure
3. Import the provided CSV file or manually add items

### 3. Setup UI
1. Create Widget Blueprints based on the provided header files
2. Configure the UI layout in the WBP_MainInventory widget
3. Bind events for drag-and-drop functionality

### 4. Initialize the System
```cpp
// In BeginPlay or when needed
PlayerController->CreateInventoryUI();
InventoryComponent->InitializeEquipmentSlots();
```

## Usage Examples

### Creating Items
```cpp
// Using the inventory manager
ABP_InventoryManager* Manager = GetWorld()->SpawnActor<ABP_InventoryManager>();
UInventoryItem* Sword = Manager->CreateItemFromID("BasicSword");
UInventoryItem* Backpack = Manager->CreateContainerItem("SmallBackpack", 6, 6);
```

### Adding Items to Inventory
```cpp
// Add item to main inventory
InventoryComponent->AddItemToInventory(NewItem);

// Add item to specific container
InventoryComponent->AddItemToInventory(NewItem, "BackpackInventory");
```

### Equipping Items
```cpp
// Equip item to specific slot
InventoryComponent->EquipItem(WeaponItem, EEquipmentSlot::PrimaryWeapon);

// Unequip item
InventoryComponent->UnequipItem(EEquipmentSlot::PrimaryWeapon);
```

### Creating Container Items
```cpp
// Create backpack with inventory
UInventoryItem* Backpack = CreateItemFromID("SmallBackpack");
Backpack->bHasInventorySlots = true;
Backpack->ContainerData.InventoryWidth = 6;
Backpack->ContainerData.InventoryHeight = 6;
Backpack->ContainerData.WeightLimit = 25.0f;
```

## Configuration

### Item Types
- **Equipment**: Items that can be equipped to specific slots
- **Consumable**: Items that can be used and consumed
- **Material**: Crafting materials and resources
- **Key**: Special items for quest progression
- **Container**: Items that provide additional inventory space

### Equipment Slots
Configure which items can be equipped to which slots by setting the `EquipmentSlot` property on items.

### Container Configuration
For clothing items with inventory:
```cpp
ContainerData.ContainerName = "VestInventory";
ContainerData.InventoryWidth = 8;
ContainerData.InventoryHeight = 4;
ContainerData.WeightLimit = 15.0f;
ContainerData.AllowedItemTypes = {EItemType::Consumable, EItemType::Material};
```

## Events and Delegates

### Inventory Events
- `OnInventoryChanged`: Fired when items are added/removed
- `OnEquipmentChanged`: Fired when equipment is equipped/unequipped

### UI Events
- `OnItemDropped`: Handles item drag-and-drop
- `OnItemClicked`: Handles item selection

## Performance Considerations

- Items are stored as UObject references for memory efficiency
- Grid calculations are optimized for large inventories
- UI updates are batched to prevent performance issues
- Container inventories are only created when needed

## Extending the System

### Custom Item Types
1. Inherit from `UInventoryItem`
2. Add custom properties and functions
3. Register with the item factory

### Custom Equipment Stats
```cpp
// Add stat modifiers to equipment
Item->StatModifiers.Add("Damage", 10.0f);
Item->StatModifiers.Add("Defense", 5.0f);
```

### Custom Container Behaviors
Override container functions in `UInventoryItem` to implement special behaviors like auto-sorting or item transformation.

## Troubleshooting

### Common Issues
1. **Items not displaying**: Check that ItemIcon is set in the data table
2. **Drag-and-drop not working**: Ensure drag detection is properly configured
3. **Container not appearing**: Verify that `bHasInventorySlots` is true and container data is configured

### Debug Functions
The system includes several debug functions for troubleshooting:
- Check inventory state
- Validate item placements
- Monitor weight calculations

## Blueprint Integration

All core functionality is exposed to Blueprint, allowing designers to:
- Create items visually
- Configure equipment stats
- Set up UI layouts
- Handle game-specific events

This system provides a solid foundation for any game requiring a sophisticated inventory management system with spatial constraints and equipment mechanics.