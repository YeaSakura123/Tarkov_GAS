// YeaSakura123 2025.11


#include "UI/EFT_HUD.h"
#include "UI/EFT_HUDViewModel.h"

void UEFT_HUD::InitializeHUD(UEFT_HUDViewModel* InViewModel)
{
	HUDViewModel = InViewModel;
}

void UEFT_HUD::NativeDestruct()
{
	HUDViewModel = nullptr;
	Super::NativeDestruct();
}