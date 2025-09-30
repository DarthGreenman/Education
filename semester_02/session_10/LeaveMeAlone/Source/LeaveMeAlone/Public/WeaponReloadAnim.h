// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "WeaponReloadAnim.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyReloadFinishedSignature, USkeletalMeshComponent*);

UCLASS()
class LEAVEMEALONE_API UWeaponReloadAnim : public UAnimNotify
{
	GENERATED_BODY()

	public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FOnNotifyReloadFinishedSignature OnNotifyReload{};
	
};
