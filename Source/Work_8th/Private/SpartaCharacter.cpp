// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "SpartaGameState.h"
#include "Components/TextBlock.h"
#include "SpartaPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameState.h"


ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//스켈레탈 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny.SKM_Manny"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPAsset(TEXT("/Game/BluePrints/ABP_SpartaCharacter"));
	if (AnimBPAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBPAsset.Class);
	}

	Tags.Add(FName("Player"));
	SpringArm=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->SetRelativeLocation(FVector(0.0f,0.0f,150.0f));
	SpringArm->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	
	//위젯
	OverHeadWidget=CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHead"));
	OverHeadWidget->SetupAttachment(GetMesh());
	OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//속도
	OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintRate = 1.3f;
	
	//체력
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	//스태미나
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 1.0f;
	StaminaRegenDelay = 0.1f;

	//디버프
	bIsReverse = false;
	ReverseTime = 5.0f;
	
	
}

// Called when the game starts or when spawned
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	GetWorldTimerManager().SetTimer(
		StaminaRegenTimerHandle,
		this,
		&ASpartaCharacter::RegenStamina,
		StaminaRegenDelay,
		true);
}

// Called every frame
void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::MoveForward);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump);
				
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJumping);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint);
				
				EnhancedInputComponent->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}
		}
	}
}

//----------------------//
//       로직 함수      //
//---------------------//

float ASpartaCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth-ActualDamage , 0.0f, MaxHealth);
	UpdateOverheadHP();
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		if (ASpartaGameState* SpartaGameState = Cast<ASpartaGameState>(GameState))
		{
			SpartaGameState->OnGameOver();
		}
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverHeadWidget)return;
	if (UUserWidget* OverHeadWidgetInstance = OverHeadWidget->GetUserWidgetObject())
	{
		if (UProgressBar* HPProgressBar = Cast<UProgressBar>(OverHeadWidgetInstance->GetWidgetFromName("HPBar")))
		{
			float Percent = FMath::Clamp(CurrentHealth/MaxHealth,0.0f,1.0f);
			HPProgressBar->SetPercent(Percent);
		}
		if (UProgressBar* StaminaBar = Cast<UProgressBar>(OverHeadWidgetInstance->GetWidgetFromName("StaminaBar")))
		{
			float Percent = FMath::Clamp(CurrentStamina/MaxStamina,0.0f,1.0f);
			StaminaBar->SetPercent(Percent);
		}
	}
}

void ASpartaCharacter::UpdateOverheadStamina()
{
	if (!OverHeadWidget)return;
	if (UUserWidget* OverHeadWidgetInstance = OverHeadWidget->GetUserWidgetObject())
	{
		if (UProgressBar* StaminaBar = Cast<UProgressBar>(OverHeadWidgetInstance->GetWidgetFromName("StaminaBar")))
		{
			float Percent = FMath::Clamp(CurrentStamina/MaxStamina,0.0f,1.0f);
			StaminaBar->SetPercent(Percent);
		}
	}
}

//----------------------//
//       회복 함수       //
//---------------------//

//스태미나 회복
void ASpartaCharacter::RegenStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate, 0.0f, MaxStamina);
	UpdateOverheadStamina();
}

//체력 회복
void ASpartaCharacter::Heal(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Heal!!, Current Health = %f"), CurrentHealth));
	UpdateOverheadHP();
}

//----------------------//
//      디버프 관련      //
//---------------------//
void ASpartaCharacter::ReversControl(float Duration)
{
	bIsReverse = true;
	if (!FMath::IsNearlyZero(ReverseTime))
	{
		ReverseTime += Duration;
		GetWorldTimerManager().ClearTimer(ReverseClearTimerHandle);
		GetWorldTimerManager().ClearTimer(ReverseUITimerHandle);
	}
	else ReverseTime = Duration;
		
	GetWorldTimerManager().SetTimer(
		ReverseUITimerHandle,
		this,
		&ASpartaCharacter::UpdateReverseUITimer,
		0.1f,
		true);
	
	GetWorldTimerManager().SetTimer(
	ReverseClearTimerHandle,
	this,
	&ASpartaCharacter::ClearReverse,
	ReverseTime,
	false);
}

void ASpartaCharacter::ClearReverse()
{
	bIsReverse = false;
}

void ASpartaCharacter::SlowMovement(float SlowRate, float SlowDuration)
{
	bIsSlow = true;
	SlowedSpeed = OriginSpeed * SlowRate;
	if (SlowTime > 0.0f)
	{
		SlowTime += SlowDuration;
		GetWorldTimerManager().ClearTimer(SlowClearTimerHandle);
		GetWorldTimerManager().ClearTimer(SlowUITimerHandle);
	}
	else
	{
		SlowTime = SlowDuration;
	}
	GetWorldTimerManager().SetTimer(
	SlowUITimerHandle,
	this,
	&ASpartaCharacter::UpdateSlowUITimer,
	0.1f,
	true);
	
	GetWorldTimerManager().SetTimer(
	SlowClearTimerHandle,
	this,
	&ASpartaCharacter::ClearSlow,
	SlowTime,
	false);
	
}
void ASpartaCharacter::ClearSlow()
{
	bIsSlow = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}



//----------------------//
//       디버프 UI      //
//---------------------//
void ASpartaCharacter::UpdateReverseUITimer()
{
	if (ReverseTime > 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Green, 
			FString::Printf(TEXT("입력 반전!! 남은 시간: %.1f"), ReverseTime));

		ReverseTime -= 0.1f; 
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ReverseUITimerHandle);
	}
}

void ASpartaCharacter::UpdateSlowUITimer()
{
	if (SlowTime > 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, 
			FString::Printf(TEXT("속도 감소!! 남은 시간: %.1f"), SlowTime));

		SlowTime -= 0.1f; 
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SlowUITimerHandle);
	}
}


//----------------------//
//        Get함수       //
//---------------------//

float ASpartaCharacter::GetHealth() const
{
	return CurrentHealth;
}


float ASpartaCharacter::GetStamina() const
{
	return CurrentStamina;
}


//----------------------//
//      움직임 함수      //
//---------------------//
void ASpartaCharacter::MoveForward(const FInputActionValue& Value)
{
	if (bIsSlow)
	{
		GetCharacterMovement()->MaxWalkSpeed = SlowedSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	}
	if (!Controller) return;
	FVector2d value = Value.Get<FVector2d>();
	if (bIsReverse)
	{
		value.X *= -1.0f;
		value.Y *= -1.0f;
	}
	if (!FMath::IsNearlyZero(value.X))
	{
		AddMovementInput(GetActorForwardVector(), value.X);
		
	}
	if (!FMath::IsNearlyZero(value.Y))
	{
		AddMovementInput(GetActorRightVector(), value.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& Value)
{
	if(Value.Get<bool>()){
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void ASpartaCharacter::StartSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement()){
		if (CurrentStamina > 0.0f)
		{
			if (bIsSlow)
			{
				SprintSpeed = SlowedSpeed * SprintRate;
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
				StaminaRegenRate = 0.0f;
				CurrentStamina -= 0.1f;
				UpdateOverheadStamina();
			}
			else
			{
				SprintSpeed = OriginSpeed * SprintRate;
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
				StaminaRegenRate = 0.0f;
				CurrentStamina -= 0.1f;
				UpdateOverheadStamina();
			}
		}
	}
	
}

void ASpartaCharacter::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	StaminaRegenRate = 1.0f;
}

void ASpartaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d value = Value.Get<FVector2d>();
	
	AddControllerYawInput(value.X);
	AddControllerPitchInput(-value.Y);
}

