// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/*
 *	게임에 사용되는, 엔진에 등록될 모든 GameplayTag들이 포함됨
 */
struct FCombatGameplayTags
{
/*
	######################################
	#			  Functions				 #
	######################################
*/
public:
	static FCombatGameplayTags& Get();
	static void InitializeGameplayTags();
	static FGameplayTag FindGameplayTagByName(const FName& TagName);

protected:
	void RegistAllTags(UGameplayTagsManager& TagManager);
	void FindOrAddTag(FGameplayTag& OutTag, const FName& TagName);

/*
	######################################
	#			  Variables				 #
	######################################
*/
public:
	/* Input Tags */
	FGameplayTag Input_Type_Move;
	FGameplayTag Input_Type_Look;
	FGameplayTag Input_Type_Run;
	FGameplayTag Input_Type_Crouch;

	FGameplayTag Input_Type_Lockon;
	FGameplayTag Input_Type_LockonChange;
	FGameplayTag Input_Type_LockonChange_Left;
	FGameplayTag Input_Type_LockonChange_Right;

	FGameplayTag Input_Type_Jump;
	FGameplayTag Input_Type_Guard;
	FGameplayTag Input_Type_LightAttack;
	FGameplayTag Input_Type_Skill;
	FGameplayTag Input_Type_Dodge;
	FGameplayTag Input_Type_Interact;
	FGameplayTag Input_Type_Consume;
	FGameplayTag Input_Type_DeathBlow;

	FGameplayTag Input_Type_QuickSlot_ConsumptionNext;
	FGameplayTag Input_Type_QuickSlot_ConsumptionPrev;

	FGameplayTag Input_Type_QuickSlot_SkillNext;
	FGameplayTag Input_Type_QuickSlot_SkillPrev;

	/* Input State */
	FGameplayTag Input_State_DisableFirstCombo;

	/* Character State Tags */
	FGameplayTag Character_MainState_Hit;			// Small, Large Hit
	FGameplayTag Character_MainState_Airborne;
	FGameplayTag Character_MainState_Ground;
	FGameplayTag Character_MainState_Dead;
	FGameplayTag Character_MainState_Groggy;

	FGameplayTag Character_State_Run;
	FGameplayTag Character_State_Crouch;
	FGameplayTag Character_State_Jump;
	FGameplayTag Character_State_Lockon;
	FGameplayTag Character_State_CombatMode;
	FGameplayTag Character_State_Attack;
	FGameplayTag Character_State_Skill;
	FGameplayTag Character_State_Dodge;
	FGameplayTag Character_State_SuperArmor;
	FGameplayTag Character_State_Invincible;
	FGameplayTag Character_State_OneDamage;
	FGameplayTag Character_State_BlockMovement;
	FGameplayTag Character_State_BlockInput;
	FGameplayTag Character_State_IgnoreLockon;
	FGameplayTag Character_State_Guard;
	FGameplayTag Character_State_Parryable;
	FGameplayTag Character_State_Parry;
	FGameplayTag Character_State_Interact;
	FGameplayTag Character_State_Consume;
	FGameplayTag Character_State_Counter;
	FGameplayTag Character_State_ExecutionReady;
	FGameplayTag Character_State_Execution;
	FGameplayTag Character_State_Executable;
	FGameplayTag Character_State_DeathBlow;

	/* 현재 Movement Mode */
	FGameplayTag Character_MovementMode_Walking;
	FGameplayTag Character_MovementMode_NavWalking;
	FGameplayTag Character_MovementMode_Falling;
	FGameplayTag Character_MovementMode_Swimming;
	FGameplayTag Character_MovementMode_Flying;
	FGameplayTag Character_MovementMode_Custom;

	TMap<uint8, FGameplayTag> MovementModeTagMap;

	/* AI를 위한 Tags */
	FGameplayTag Character_AI_DisableAutoGuard;

	/*
	*	Ability Type Tags
	*	Character State Tag와 비슷하지만, 나누어진 이유는 이 AbilityTag를 가진
	*	실행중인 어빌리티를 종료하기 위해서는 나누어져야 함
	*/
	FGameplayTag Ability_Type_Attack;
	FGameplayTag Ability_Type_Skill;
	FGameplayTag Ability_Type_Dead;
	FGameplayTag Ability_Type_Lockon;
	FGameplayTag Ability_Type_CombatMode;

	FGameplayTag Ability_Type_Movement;		// Parent.
	FGameplayTag Ability_Type_Movement_Run;
	FGameplayTag Ability_Type_Movement_Crouch;
	FGameplayTag Ability_Type_Movement_Jump;
	FGameplayTag Ability_Type_Movement_Dodge;

	FGameplayTag Ability_Type_Action;		// Parent.
	FGameplayTag Ability_Type_Action_Guard;
	FGameplayTag Ability_Type_Action_UseControllerYaw;
	FGameplayTag Ability_Type_Action_Interact;
	FGameplayTag Ability_Type_Action_InteractActive;
	FGameplayTag Ability_Type_Action_Landing;
	FGameplayTag Ability_Type_Action_Consume;
	FGameplayTag Ability_Type_Action_DeathBlow;

	FGameplayTag Ability_Type_Action_Counter_PierceAttack;
	FGameplayTag Ability_Type_Action_Countered_PierceAttack;

	FGameplayTag Ability_Type_Action_Counter_BelowAttack;
	FGameplayTag Ability_Type_Action_Countered_BelowAttack;

	FGameplayTag Ability_Type_Action_ExecutionReady;
	FGameplayTag Ability_Type_Action_Execution;
	FGameplayTag Ability_Type_Action_Executed;

	FGameplayTag Ability_Type_Hit;			// Parent.
	FGameplayTag Ability_Type_Hit_Small;
	FGameplayTag Ability_Type_Hit_Large;
	FGameplayTag Ability_Type_Hit_Airborne;
	FGameplayTag Ability_Type_Hit_Knockdown;
	FGameplayTag Ability_Type_Hit_Falldown;
	FGameplayTag Ability_Type_Hit_Ground;
	FGameplayTag Ability_Type_Hit_Getup;
	FGameplayTag Ability_Type_Hit_Guard;
	FGameplayTag Ability_Type_Hit_GuardBreak;
	FGameplayTag Ability_Type_Hit_Parry;
	FGameplayTag Ability_Type_Hit_AttackFailed;		// 공격이 패링당해 실패한 경우
	FGameplayTag Ability_Type_Hit_Groggy;

	FGameplayTag Ability_Type_Hit_DeathBlow_Default;
	FGameplayTag Ability_Type_Hit_DeathBlow_Back;


	/* Activation Group */
	FGameplayTag ActivationType_Active;

	/* Event Tags */
	FGameplayTag Event_Combat_Cancel;
	FGameplayTag Event_Combat_SuccessOverlapHitBox;
	FGameplayTag Event_OutOfHealth;
	FGameplayTag Event_Dead;
	FGameplayTag Event_Rest;
	FGameplayTag EventParent;

	/* Notify Timing Tags */
	FGameplayTag NotifyTiming_BeginAttack;
	FGameplayTag NotifyTiming_PierceAttack;
	FGameplayTag NotifyTiming_BelowAttack;
	FGameplayTag NotifyTiming_Consume;
	FGameplayTag NotifyTiming_AfterSpawn;
	FGameplayTag NotifyTiming_DeathBlow;
	FGameplayTag NotifyTiming_DeathBlowRestore;


	/* Hit Result Tags
	*	- Hit_MainType Tag에 의해 가공되며 최종적으로 선택된 Damage GameplayEffect에 아래 태그들이 포함됨
	*	  아래 태그들은 가공되지 않으며, 태그값 그대로 캐릭터가 피격에 반응함 (Post Damage Process Tag)
	*	  (e.g: HitEvent_Result_Knockdown 태그를 받은경우, 반드시 캐릭터가 넘어진다는 결과를 보장)
	* 
	*	  @see ACombatCharacter::OnAfterDamaged()
	*/
	FGameplayTag HitEvent_Result_Small;
	FGameplayTag HitEvent_Result_Large;
	FGameplayTag HitEvent_Result_Airborne;
	FGameplayTag HitEvent_Result_Knockdown;
	FGameplayTag HitEvent_Result_Falldown;
	FGameplayTag HitEvent_Result_Guard;
	FGameplayTag HitEvent_Result_GuardBreak;
	FGameplayTag HitEvent_Result_Parry;
	FGameplayTag HitEvent_Result_Groggy;
	FGameplayTag HitEvent_Result_Dummy;
	FGameplayTag HitEvent_Result_System;	// 시스템 데미지, 무적을 무시하는 데미지

	/* Hit MainType Tags (in HitData)
	*	- Hit_MainType으로 캐릭터의 상태에 따라 원하는 Result로 변환시키는 역할을 수행
	*	  (e.g: 가드중에 특정 각도에서 HitEvent_Result_Guard 태그를 반환, Pre Damage Process Tag)
	*/
	FGameplayTag Hit_MainType_Small;
	FGameplayTag Hit_MainType_Large;
	FGameplayTag Hit_MainType_Airborne;
	FGameplayTag Hit_MainType_Knockdown;
	FGameplayTag Hit_MainType_Falldown;

	/* Hit SubType Tags
	*	- 공격이 어떤 효과를 가지는지를 의미
	*/
	FGameplayTag Hit_SubType_GuardBreak;

	/* Attack Direction Tags
	*	- HitData에서 설정되는 태그로 공격이 어떤 방향으로 진행되는지 의미하는 태그
	*	  공격 방향에 따라 적절한 피격 모션을 재생하는 용도로 쓰임
	*/
	FGameplayTag AttackDirection_RightTopToLeftBottom;	// 우측상단 -> 좌측하단 공격
	FGameplayTag AttackDirection_LeftTopToRightBottom;	// 좌측하단 -> 우측상단 공격
	FGameplayTag AttackDirection_RightBottomToLeftTop;	// 우측하단 -> 좌측상단 공격
	FGameplayTag AttackDirection_LeftBottomToRightTop;	// 좌측하단 -> 우측상단 공격
	FGameplayTag AttackDirection_Center;				// 정면 공격

	/* GameplayCue Tags */
	FGameplayTag GameplayCue_DamageReceived;
	FGameplayTag GameplayCue_SuccessGuard;
	FGameplayTag GameplayCue_SuccessParry;
	FGameplayTag GameplayCue_FailGuard;
	FGameplayTag GameplayCue_Executable;
	FGameplayTag GameplayCue_Heal;

	/* SetByCaller */
	FGameplayTag SetByCaller_WeaponDamage;
	FGameplayTag SetByCaller_FixedDamage;
	FGameplayTag SetByCaller_ParrayGauge;
	FGameplayTag SetByCaller_GroggyGauge;

	/* Hit Effect */
	FGameplayTag HitEffect_Default;

	/* Item */
	FGameplayTag Item_Type_Equipment;
	FGameplayTag Item_Type_Consumable;
	FGameplayTag Item_Type_Goods;
	FGameplayTag Item_Type_Etc;

	/* UI Layer */
	FGameplayTag UI_Layer_HUD;
	FGameplayTag UI_Layer_Game;
	FGameplayTag UI_Layer_System;
	FGameplayTag UI_Layer_Dialog;

	/* Grant Ability Type Tag */
	FGameplayTag GrantAbility_Default;
	FGameplayTag GrantAbility_Equipment;
	FGameplayTag GrantAbility_Consume;
	FGameplayTag GrantAbility_Skill;

	/* Skin */
	FGameplayTag Skin_Default;

	/* Cooldown */
	FGameplayTag Cooldown_Run;
	FGameplayTag Cooldown_Jump;
	FGameplayTag Cooldown_Crouch;

private:
	/* Singleton */
	static FCombatGameplayTags Tags;
	
private:
	bool bInitialized = false;
};
