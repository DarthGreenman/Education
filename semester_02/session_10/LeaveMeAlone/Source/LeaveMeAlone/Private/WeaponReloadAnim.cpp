// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "WeaponReloadAnim.h"

void UWeaponReloadAnim::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	OnNotifyReload.Broadcast(MeshComp);
	UAnimNotify::Notify(MeshComp, Animation);
}
