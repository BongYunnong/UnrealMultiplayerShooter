// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterTypes/Team.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	EWS_Initial UMETA(Displayname = "Initial State"),
	EWS_Equipped UMETA(Displayname = "Equipped"),
	EWS_EquippedSecondary UMETA(Displayname = "EquippedSecondary"),
	EWS_Dropped UMETA(Displayname = "Dropped"),
	EWS_MAX UMETA(Displayname = "DefaultMAX"),
};

UENUM(BlueprintType)
enum class EFireType :uint8
{
	EFT_HitScan UMETA(Displayname = "HitScanWeapon"),
	EFT_Projectile UMETA(Displayname = "ProjectileWeapon"),
	EFT_Shotgun UMETA(Displayname = "ShotgunWeapon"),
	EWS_MAX UMETA(Displayname = "DefaultMAX"),
};


UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	void ShowPickupWidget(bool bShowWidget);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void SetHUDAmmo();
	virtual void Fire(const FVector& HitTarget);
	virtual void Dropped();
	void AddAmmo(int32 AmmoToAdd);
	FVector TraceEndWithScatter(const FVector& HitTarget);

	// Textures for the weapon Crosshairs
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsBottom;

	// Zoom FOV While Aiming
	UPROPERTY(EditAnywhere)
		float ZoomedFOV = 30.f;
	UPROPERTY(EditAnywhere)
		float ZoomInterpSpeed = 20.f;

	// Automatic Fire
	UPROPERTY(EditAnywhere, Category = Combat)
		float FireDelay = .15f;
	UPROPERTY(EditAnywhere, Category = Combat)
		bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	// Enable or disable custom depth
	void EnableCustomDepth(bool bEnable);

	bool bDestroyWeapon = false;

	UPROPERTY(EditAnywhere)
	EFireType FireType;
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		bool bUseScatter = false;
protected:
	virtual void BeginPlay() override;

	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	UFUNCTION()
	virtual void OnSphereOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	// Trace End With Scatter
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		float DistanceToSphere = 800.f;
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		float SphereRadius = 75.f;


	UPROPERTY()
		class ABlasterCharacter* BlasterOwnerCharacter;
	UPROPERTY()
		class ABlasterPlayerController* BlasterOwnerController;
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UFUNCTION(Client, Reliable)
		void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
		void ClientAddAmmo(int32 AmmoToAdd);
	// the number of unprocessed server request for ammo
	// incremented in spendround, decremented in clientUpateAmmo
	int32 Sequence = 0;

	void SpendRound();
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;


	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
		ETeam Team;

public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	bool IsEmpty();
	bool IsFull();
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE ETeam GetTeam() const { return Team; }
};
