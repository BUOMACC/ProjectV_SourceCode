// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameplayTags.h"
#include "GameplayTagsManager.h"

FCombatGameplayTags FCombatGameplayTags::Tags;

FCombatGameplayTags& FCombatGameplayTags::Get()
{
	if (Tags.bInitialized == false)
	{
		Tags.InitializeGameplayTags();
	}
	return Tags;
}

void FCombatGameplayTags::InitializeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	Tags.RegistAllTags(Manager);
	Manager.DoneAddingNativeTags();
	Tags.bInitialized = true;
}

FGameplayTag FCombatGameplayTags::FindGameplayTagByName(const FName& TagName)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	return Manager.RequestGameplayTag(TagName, false);
}

void FCombatGameplayTags::RegistAllTags(UGameplayTagsManager& TagManager)
{
	// Input Tags
	FindOrAddTag(Input_Type_Move, TEXT("Input.Type.Move"));
	FindOrAddTag(Input_Type_Look, TEXT("Input.Type.Look"));
	FindOrAddTag(Input_Type_Run, TEXT("Input.Type.Run"));
	FindOrAddTag(Input_Type_Crouch, TEXT("Input.Type.Crouch"));
	FindOrAddTag(Input_Type_Lockon, TEXT("Input.Type.Lockon"));
	FindOrAddTag(Input_Type_LockonChange, TEXT("Input.Type.LockonChange"));
	FindOrAddTag(Input_Type_LockonChange_Left, TEXT("Input.Type.LockonChange.Left"));
	FindOrAddTag(Input_Type_LockonChange_Right, TEXT("Input.Type.LockonChange.Right"));
	FindOrAddTag(Input_Type_Jump, TEXT("Input.Type.Jump"));
	FindOrAddTag(Input_Type_Guard, TEXT("Input.Type.Guard"));
	FindOrAddTag(Input_Type_LightAttack, TEXT("Input.Type.LightAttack"));
	FindOrAddTag(Input_Type_Skill, TEXT("Input.Type.Skill"));
	FindOrAddTag(Input_Type_Dodge, TEXT("Input.Type.Dodge"));
	FindOrAddTag(Input_Type_Interact, TEXT("Input.Type.Interact"));
	FindOrAddTag(Input_Type_Consume, TEXT("Input.Type.Consume"));
	FindOrAddTag(Input_Type_DeathBlow, TEXT("Input.Type.DeathBlow"));

	FindOrAddTag(Input_Type_QuickSlot_ConsumptionPrev, TEXT("Input.Type.QuickSlot.ConsumptionPrev"));
	FindOrAddTag(Input_Type_QuickSlot_ConsumptionNext, TEXT("Input.Type.QuickSlot.ConsumptionNext"));

	FindOrAddTag(Input_Type_QuickSlot_SkillPrev, TEXT("Input.Type.QuickSlot.SkillPrev"));
	FindOrAddTag(Input_Type_QuickSlot_SkillNext, TEXT("Input.Type.QuickSlot.SkillNext"));

	// Input State
	FindOrAddTag(Input_State_DisableFirstCombo, TEXT("Input.State.DisableFirstCombo"));
	//FindOrAddTag(Input_State_DisableCombo, TEXT("Input.State.DisableCombo"));

	// Character State - 캐릭터 상태 (화상, 출혈, 무적, 입력차단 등...)
	FindOrAddTag(Character_MainState_Hit, TEXT("Character.MainState.Hit"));
	FindOrAddTag(Character_MainState_Airborne, TEXT("Character.MainState.Airborne"));
	FindOrAddTag(Character_MainState_Ground, TEXT("Character.MainState.Ground"));
	FindOrAddTag(Character_MainState_Dead, TEXT("Character.MainState.Dead"));
	FindOrAddTag(Character_MainState_Groggy, TEXT("Character.MainState.Groggy"));

	FindOrAddTag(Character_State_Run, TEXT("Character.State.Run"));
	FindOrAddTag(Character_State_Crouch, TEXT("Character.State.Crouch"));
	FindOrAddTag(Character_State_Jump, TEXT("Character.State.Jump"));
	FindOrAddTag(Character_State_Lockon, TEXT("Character.State.Lockon"));
	FindOrAddTag(Character_State_CombatMode, TEXT("Character.State.CombatMode"));
	FindOrAddTag(Character_State_Attack, TEXT("Character.State.Attack"));
	FindOrAddTag(Character_State_Skill, TEXT("Character.State.Skill"));
	FindOrAddTag(Character_State_Dodge, TEXT("Character.State.Dodge"));
	FindOrAddTag(Character_State_SuperArmor, TEXT("Character.State.SuperArmor"));
	FindOrAddTag(Character_State_Invincible, TEXT("Character.State.Invincible"));
	FindOrAddTag(Character_State_OneDamage, TEXT("Character.State.OneDamage"));
	FindOrAddTag(Character_State_BlockInput, TEXT("Character.State.BlockInput"));
	FindOrAddTag(Character_State_BlockMovement, TEXT("Character.State.BlockMovement"));
	FindOrAddTag(Character_State_IgnoreLockon, TEXT("Character.State.IgnoreLockon"));		// Lockon회전 무시
	FindOrAddTag(Character_State_Guard, TEXT("Character.State.Guard"));
	FindOrAddTag(Character_State_Parryable, TEXT("Character.State.Parryable"));
	FindOrAddTag(Character_State_Parry, TEXT("Character.State.Parry"));
	FindOrAddTag(Character_State_Interact, TEXT("Character.State.Interact"));
	FindOrAddTag(Character_State_Consume, TEXT("Character.State.Consume"));
	FindOrAddTag(Character_State_Counter, TEXT("Character.State.Counter"));
	
	FindOrAddTag(Character_State_ExecutionReady, TEXT("Character.State.ExecutionReady"));	// 처형 준비상태
	FindOrAddTag(Character_State_Execution, TEXT("Character.State.Execution"));				// 처형중 상태
	FindOrAddTag(Character_State_Executable, TEXT("Character.State.Executable"));			// 처형가능 상태
	FindOrAddTag(Character_State_DeathBlow, TEXT("Character.State.DeathBlow"));				// 처형가능 상태

	// Movement Mode (via CMC)
	FindOrAddTag(Character_MovementMode_Walking, TEXT("Character.MovementMode.Walking"));
	FindOrAddTag(Character_MovementMode_NavWalking, TEXT("Character.MovementMode.NavWalking"));
	FindOrAddTag(Character_MovementMode_Falling, TEXT("Character.MovementMode.Falling"));
	FindOrAddTag(Character_MovementMode_Swimming, TEXT("Character.MovementMode.Swimming"));
	FindOrAddTag(Character_MovementMode_Flying, TEXT("Character.MovementMode.Flying"));
	FindOrAddTag(Character_MovementMode_Custom, TEXT("Character.MovementMode.Custom"));

	MovementModeTagMap.Add(MOVE_Walking, Character_MovementMode_Walking);
	MovementModeTagMap.Add(MOVE_NavWalking, Character_MovementMode_NavWalking);
	MovementModeTagMap.Add(MOVE_Falling, Character_MovementMode_Falling);
	MovementModeTagMap.Add(MOVE_Swimming, Character_MovementMode_Swimming);
	MovementModeTagMap.Add(MOVE_Flying, Character_MovementMode_Flying);
	MovementModeTagMap.Add(MOVE_Custom, Character_MovementMode_Custom);

	// For AI (used only in AI)
	FindOrAddTag(Character_AI_DisableAutoGuard, TEXT("Character.AI.DisableAutoGuard"));		// AI 전용 - 자동가드 비활성화

	// Ability Type Tags - 어빌리티를 구분할 태그
	FindOrAddTag(Ability_Type_Attack, TEXT("Ability.Type.Attack"));
	FindOrAddTag(Ability_Type_Skill, TEXT("Ability.Type.Skill"));
	FindOrAddTag(Ability_Type_Dead, TEXT("Ability.Type.Dead"));
	FindOrAddTag(Ability_Type_Lockon, TEXT("Ability.Type.Lockon"));
	FindOrAddTag(Ability_Type_CombatMode, TEXT("Ability.Type.CombatMode"));

	FindOrAddTag(Ability_Type_Movement, TEXT("Ability.Type.Movement")); // parent.
	FindOrAddTag(Ability_Type_Movement_Run, TEXT("Ability.Type.Movement.Run"));
	FindOrAddTag(Ability_Type_Movement_Crouch, TEXT("Ability.Type.Movement.Crouch"));
	FindOrAddTag(Ability_Type_Movement_Jump, TEXT("Ability.Type.Movement.Jump"));
	FindOrAddTag(Ability_Type_Movement_Dodge, TEXT("Ability.Type.Movement.Dodge"));

	FindOrAddTag(Ability_Type_Action, TEXT("Ability.Type.Action"));
	FindOrAddTag(Ability_Type_Action_Guard, TEXT("Ability.Type.Action.Guard"));
	FindOrAddTag(Ability_Type_Action_UseControllerYaw, TEXT("Ability.Type.Action.UseControllerYaw"));
	FindOrAddTag(Ability_Type_Action_Interact, TEXT("Ability.Type.Action.Interact"));
	FindOrAddTag(Ability_Type_Action_InteractActive, TEXT("Ability.Type.Action.InteractActive"));
	FindOrAddTag(Ability_Type_Action_Landing, TEXT("Ability.Type.Action.Landing"));
	FindOrAddTag(Ability_Type_Action_Consume, TEXT("Ability.Type.Action.Consume"));

	FindOrAddTag(Ability_Type_Action_Counter_PierceAttack, TEXT("Ability.Type.Action.Counter.PierceAttack"));
	FindOrAddTag(Ability_Type_Action_Countered_PierceAttack, TEXT("Ability.Type.Action.Countered.PierceAttack"));

	FindOrAddTag(Ability_Type_Action_Counter_BelowAttack, TEXT("Ability.Type.Action.Counter.BelowAttack"));
	FindOrAddTag(Ability_Type_Action_Countered_BelowAttack, TEXT("Ability.Type.Action.Countered.BelowAttack"));

	FindOrAddTag(Ability_Type_Action_ExecutionReady, TEXT("Ability.Type.Action.ExecutionReady"));
	FindOrAddTag(Ability_Type_Action_Execution, TEXT("Ability.Type.Action.Execution"));
	FindOrAddTag(Ability_Type_Action_Executed, TEXT("Ability.Type.Action.Executed"));
	FindOrAddTag(Ability_Type_Action_DeathBlow, TEXT("Ability.Type.Action.DeathBlow"));

	FindOrAddTag(Ability_Type_Hit, TEXT("Ability.Type.Hit")); // parent.
	FindOrAddTag(Ability_Type_Hit_Small, TEXT("Ability.Type.Hit.Small"));
	FindOrAddTag(Ability_Type_Hit_Large, TEXT("Ability.Type.Hit.Large"));
	FindOrAddTag(Ability_Type_Hit_Airborne, TEXT("Ability.Type.Hit.Airborne"));
	FindOrAddTag(Ability_Type_Hit_Knockdown, TEXT("Ability.Type.Hit.Knockdown"));
	FindOrAddTag(Ability_Type_Hit_Falldown, TEXT("Ability.Type.Hit.Falldown"));
	FindOrAddTag(Ability_Type_Hit_Ground, TEXT("Ability.Type.Hit.Ground"));
	FindOrAddTag(Ability_Type_Hit_Getup, TEXT("Ability.Type.Hit.Getup"));
	FindOrAddTag(Ability_Type_Hit_Guard, TEXT("Ability.Type.Hit.Guard"));
	FindOrAddTag(Ability_Type_Hit_GuardBreak, TEXT("Ability.Type.Hit.GuardBreak"));
	FindOrAddTag(Ability_Type_Hit_Parry, TEXT("Ability.Type.Hit.Parry"));
	FindOrAddTag(Ability_Type_Hit_AttackFailed, TEXT("Ability.Type.Hit.AttackFailed"));
	FindOrAddTag(Ability_Type_Hit_Groggy, TEXT("Ability.Type.Hit.Groggy"));
	FindOrAddTag(Ability_Type_Hit_DeathBlow_Default, TEXT("Ability.Type.Hit.DeathBlow.Default"));
	FindOrAddTag(Ability_Type_Hit_DeathBlow_Back, TEXT("Ability.Type.Hit.DeathBlow.Back"));

	// Ability Activation Type
	FindOrAddTag(ActivationType_Active, TEXT("ActivationType.Active"));	// 해당 태그를 가진 Ability는 하나만 실행되도록 유지됨

	// Event Tags
	FindOrAddTag(Event_Combat_Cancel, TEXT("Event.Combat.Cancel"));
	FindOrAddTag(Event_Combat_SuccessOverlapHitBox, TEXT("Event.Combat.SuccessOverlapHitBox"));
	FindOrAddTag(Event_Dead, TEXT("Event.Dead"));
	FindOrAddTag(Event_OutOfHealth, TEXT("Event.OutOfHealth"));
	FindOrAddTag(Event_Rest, TEXT("Event.Rest"));
	FindOrAddTag(EventParent, TEXT("Event"));

	// Notify Timing Tags (Notify로 행동에 대해서 알리기 위한 용도)
	//  Ex) AI의 가드 및 찌르기공격 간파 타이밍 등...
	FindOrAddTag(NotifyTiming_BeginAttack, TEXT("NotifyTiming.BeginAttack"));
	FindOrAddTag(NotifyTiming_PierceAttack, TEXT("NotifyTiming.PierceAttack"));
	FindOrAddTag(NotifyTiming_BelowAttack, TEXT("NotifyTiming.BelowAttack"));
	FindOrAddTag(NotifyTiming_Consume, TEXT("NotifyTiming.Consume"));
	FindOrAddTag(NotifyTiming_AfterSpawn, TEXT("NotifyTiming.AfterSpawn"));
	FindOrAddTag(NotifyTiming_DeathBlow, TEXT("NotifyTiming.DeathBlow"));				// 처형시 라이프와 체력이 차감되는 타이밍
	FindOrAddTag(NotifyTiming_DeathBlowRestore, TEXT("NotifyTiming.DeathBlowRestore"));	// 처형 후 라이프가 남아있는 경우 체력을 회복하는 타이밍

	// Hit Tags
	FindOrAddTag(HitEvent_Result_Small, TEXT("Hit.Result.Small"));
	FindOrAddTag(HitEvent_Result_Large, TEXT("Hit.Result.Large"));
	FindOrAddTag(HitEvent_Result_Airborne, TEXT("Hit.Result.Airborne"));
	FindOrAddTag(HitEvent_Result_Knockdown, TEXT("Hit.Result.Knockdown"));
	FindOrAddTag(HitEvent_Result_Falldown, TEXT("Hit.Result.Falldown"));
	FindOrAddTag(HitEvent_Result_Guard, TEXT("Hit.Result.Guard"));
	FindOrAddTag(HitEvent_Result_GuardBreak, TEXT("Hit.Result.GuardBreak"));
	FindOrAddTag(HitEvent_Result_Parry, TEXT("Hit.Result.Parry"));
	FindOrAddTag(HitEvent_Result_Groggy, TEXT("Hit.Result.Groggy"));
	FindOrAddTag(HitEvent_Result_Dummy, TEXT("Hit.Result.Dummy"));
	FindOrAddTag(HitEvent_Result_System, TEXT("Hit.Result.System"));

	FindOrAddTag(Hit_MainType_Small, TEXT("Hit.MainType.Small"));
	FindOrAddTag(Hit_MainType_Large, TEXT("Hit.MainType.Large"));
	FindOrAddTag(Hit_MainType_Airborne, TEXT("Hit.MainType.Airborne"));
	FindOrAddTag(Hit_MainType_Knockdown, TEXT("Hit.MainType.Knockdown"));
	FindOrAddTag(Hit_MainType_Falldown, TEXT("Hit.MainType.Falldown"));

	FindOrAddTag(Hit_SubType_GuardBreak, TEXT("Hit.SubType.GuardBreak"));

	// Attack Direction Tags
	FindOrAddTag(AttackDirection_RightTopToLeftBottom, TEXT("AttackDirection.RightTopToLeftBottom"));
	FindOrAddTag(AttackDirection_LeftTopToRightBottom, TEXT("AttackDirection.LeftTopToRightBottom"));
	FindOrAddTag(AttackDirection_RightBottomToLeftTop, TEXT("AttackDirection.RightBottomToLeftTop"));
	FindOrAddTag(AttackDirection_LeftBottomToRightTop, TEXT("AttackDirection.LeftBottomToRightTop"));
	FindOrAddTag(AttackDirection_Center, TEXT("AttackDirection.Center"));

	// GameplayCue Event Tags
	FindOrAddTag(GameplayCue_DamageReceived, TEXT("GameplayCue.DamageReceived"));
	FindOrAddTag(GameplayCue_SuccessGuard, TEXT("GameplayCue.SuccessGuard"));
	FindOrAddTag(GameplayCue_SuccessParry, TEXT("GameplayCue.SuccessParry"));
	FindOrAddTag(GameplayCue_FailGuard, TEXT("GameplayCue.FailGuard"));
	FindOrAddTag(GameplayCue_Executable, TEXT("GameplayCue.Executable"));
	FindOrAddTag(GameplayCue_Heal, TEXT("GameplayCue.Heal"));

	// Set By Caller Tags
	FindOrAddTag(SetByCaller_WeaponDamage, TEXT("SetByCaller.WeaponDamage"));
	FindOrAddTag(SetByCaller_FixedDamage, TEXT("SetByCaller.FixedDamage"));
	FindOrAddTag(SetByCaller_ParrayGauge, TEXT("SetByCaller.ParrayGauge"));
	FindOrAddTag(SetByCaller_GroggyGauge, TEXT("SetByCaller.GroggyGauge"));

	// Hit Effect
	FindOrAddTag(HitEffect_Default, TEXT("HitEffect.Default"));

	// Item
	FindOrAddTag(Item_Type_Equipment, TEXT("Item.Type.Equipment"));
	FindOrAddTag(Item_Type_Consumable, TEXT("Item.Type.Consumable"));
	FindOrAddTag(Item_Type_Goods, TEXT("Item.Type.Goods"));
	FindOrAddTag(Item_Type_Etc, TEXT("Item.Type.Etc"));

	// UI Layer
	FindOrAddTag(UI_Layer_HUD, TEXT("UI.Layer.HUD"));
	FindOrAddTag(UI_Layer_Game, TEXT("UI.Layer.Game"));
	FindOrAddTag(UI_Layer_System, TEXT("UI.Layer.System"));
	FindOrAddTag(UI_Layer_Dialog, TEXT("UI.Layer.Dialog"));

	// Grant Ability Type Tag
	FindOrAddTag(GrantAbility_Default, TEXT("GrantAbility.Default"));
	FindOrAddTag(GrantAbility_Equipment, TEXT("GrantAbility.Equipment"));
	FindOrAddTag(GrantAbility_Consume, TEXT("GrantAbility.Consume"));
	FindOrAddTag(GrantAbility_Skill, TEXT("GrantAbility.Skill"));

	// Skin
	FindOrAddTag(Skin_Default, TEXT("Skin.Default"));

	// Cooldown
	FindOrAddTag(Cooldown_Run, TEXT("Cooldown.Run"));
	FindOrAddTag(Cooldown_Jump, TEXT("Cooldown.Jump"));
	FindOrAddTag(Cooldown_Crouch, TEXT("Cooldown.Crouch"));
}

void FCombatGameplayTags::FindOrAddTag(FGameplayTag& OutTag, const FName& TagName)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	const FGameplayTag& FoundTag = Manager.RequestGameplayTag(TagName, false);

	// 컴파일후 변수의 GameplayTag가 초기화되는 문제가 있어서, 시작할때 다시 세팅해주어야 함
	if (!(FoundTag == FGameplayTag::EmptyTag))
	{
		OutTag = FoundTag;
	}
	else
	{
		OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagName, FString(TEXT("(Native)")));
	}
}