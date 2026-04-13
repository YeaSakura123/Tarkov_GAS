// YeaSakura123 2025.11


#include "UI/EFT_HUDSubsystem.h"
#include "UI/EFT_HUD.h"
#include "UI/EFT_HUDViewModel.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"
void UEFT_HUDSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    if (World->GetNetMode() == NM_DedicatedServer)
    {
        return;
    }
    
    static const TCHAR* HUDPath = TEXT("/Game/tarkov/UI/WBP_HUD.WBP_HUD_C");
    HUDWidgetClass = LoadClass<UEFT_HUD>(nullptr, HUDPath);
    
    if (!HUDWidgetClass)
    {
        return;
    }
    
    HUDViewModel = NewObject<UEFT_HUDViewModel>(this, TEXT("HUDViewModel"));
    
    // 循环定时器
    World->GetTimerManager().SetTimer(
        SetupTimerHandle,
        this,
        &ThisClass::TrySetup,
        0.1f,
        true,
        0.0f
    );

}

void UEFT_HUDSubsystem::Deinitialize()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SetupTimerHandle);
    }
    
    DestroyHUD();
    
    if (HUDViewModel)
    {
        HUDViewModel->Uninitialize();
        HUDViewModel = nullptr;
    }
    
    Super::Deinitialize();
}

void UEFT_HUDSubsystem::TrySetup()
{
    if (bIsSetupComplete)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(SetupTimerHandle);
        }
        return;
    }
    
    ULocalPlayer* LP = GetLocalPlayer();
    if (!LP) return;
    
    UWorld* World = GetWorld();
    if (!World) return;
    
    APlayerController* PC = LP->GetPlayerController(World);
    if (!PC) return;
    
    PC->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPlayerPawnChanged);
    
    if (APawn* CurrentPawn = PC->GetPawn())
    {
        OnPlayerPawnChanged(nullptr, CurrentPawn);
    }
    
    bIsSetupComplete = true;
    World->GetTimerManager().ClearTimer(SetupTimerHandle);
}

void UEFT_HUDSubsystem::CreateHUD()
{
    if (HUDWidget) return;
    
    if (!HUDWidgetClass)
    {
        return;
    }
    
    ULocalPlayer* LP = GetLocalPlayer();
    if (!LP) return;
    
    APlayerController* PC = LP->GetPlayerController(GetWorld());
    if (!PC) return;
    
    HUDWidget = CreateWidget<UEFT_HUD>(PC, HUDWidgetClass);
    if (HUDWidget)
    {
        HUDWidget->InitializeHUD(HUDViewModel);
        HUDWidget->AddToViewport(0);
    }
}

void UEFT_HUDSubsystem::DestroyHUD()
{
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
    }
}

void UEFT_HUDSubsystem::ShowHUD()
{
    if (!HUDWidget) CreateHUD();
    if (HUDWidget) HUDWidget->SetVisibility(ESlateVisibility::Visible);
    if (HUDViewModel) HUDViewModel->SetHUDVisible(true);
}

void UEFT_HUDSubsystem::HideHUD()
{
    if (HUDWidget) HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (HUDViewModel) HUDViewModel->SetHUDVisible(false);
}

void UEFT_HUDSubsystem::OnPlayerPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    if (HUDViewModel)
    {
        if (OldPawn) HUDViewModel->Uninitialize();
        if (NewPawn) HUDViewModel->InitializeWithPlayer(NewPawn);
    }
    
    if (NewPawn && !HUDWidget)
    {
        CreateHUD();
    }
}
