#include "InventorySystem.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogInventorySystem);

class FInventorySystemModule : public IModuleInterface
{
public:
        virtual void StartupModule() override
        {
                UE_LOG(LogInventorySystem, Log, TEXT("InventorySystem module started"));
        }

        virtual void ShutdownModule() override
        {
                UE_LOG(LogInventorySystem, Log, TEXT("InventorySystem module shut down"));
        }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FInventorySystemModule, InventorySystem, "InventorySystem");
