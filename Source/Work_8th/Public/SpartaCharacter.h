// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SpartaCharacter.generated.h"

struct FInputActionValue;
class UWidgetComponent;

UCLASS()
class WORK_8TH_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	UWidgetComponent* OverHeadWidget;

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	//디버프 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	bool bIsReverse;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	bool bIsSlow;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	float ReverseTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	float SlowTime;
	
	//속도 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float OriginSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float SprintSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float SprintRate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float CurrentSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float SlowedSpeed;
	
	//체력, 스태미나 관련 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float CurrentHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stamina")
	float MaxStamina;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stamina")
	float CurrentStamina;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stamina")
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stamina")
	float StaminaRegenDelay;

	//스태미너 타이머 핸들
	FTimerHandle StaminaRegenTimerHandle;
	//디버프 타이머 핸들
	FTimerHandle ReverseClearTimerHandle;
	FTimerHandle SlowClearTimerHandle;
	//디버프 지속시간 업데이트 타이머 핸들
	FTimerHandle ReverseUITimerHandle;
	FTimerHandle SlowUITimerHandle;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
public:
	//디버프
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ReversControl(float Duration);
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ClearReverse();
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void SlowMovement(float SlowRate, float SlowDuration);
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ClearSlow();
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void UpdateReverseUITimer();
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void UpdateSlowUITimer();
	
	//체력, 스태미나
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount);
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStamina() const;
	UFUNCTION()
	void RegenStamina();

	//로직
	void OnDeath();

	//3D 위젯 업데이트
	void UpdateOverheadHP();
	void UpdateOverheadStamina();
};
