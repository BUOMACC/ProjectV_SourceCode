## ProjectV (개인 포트폴리오)

플레이 영상 : https://youtu.be/wiK8aZlot70

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Title.png" width="75%" height="75%"/>

### 게임 정보
- Unreal Engine 5 (GAS, CommonUI)
- 세키로 / 소울라이크 패링 위주 전투 모작
- 1인 5 ~ 6개월 개발
- Listen / Dedicated Server 지원
- C++ / BP (9 : 1 비율 작업)
- PC, 키보드 / 마우스

<a name="table-contents"></a>
## 목차
> 1. [전투 시스템](#combat)  
>   1.1 [프로젝트 개발 방향 & GAS 사용 이유](#combat_01)  
>   1.2 [전투 구조](#combat_02)  
>   1.3 [AttributeSet](#combat_03)  
> 3. [입력 처리](#input)  
>   2.1 [플레이어 입력 프로세스](#input_01)  
>   2.2 [선입력 처리](#input_02)  
> 4. [위치 & 회전 동기화](#cmc_network)  
>   3.1 [멀티플레이 환경에서 발생한 위치보정 이슈 처리](#cmc_network_01)  
> 5. [Lyra 컨텐츠 코드 활용 및 개선](#lyra)  
>   4.1 [AbilitySet](#lyra_01)  
>   4.2 [Inventory](#lyra_02)  
> 6. [UI](#ui)  
>   5.1 [UI 구조](#ui_01)  
> 7. [기능 구현](#contents)  
>   6.1 [퀵 슬롯](#contents_01)  
>   6.2 [캐릭터 외형](#contents_02)  
>   6.3 [상호작용](#contents_03)  
>   6.4 [치명적인 일격(DeathBlow)](#contents_04)  
> 8. [AI 처리](#ai)  
>   7.1 [몬스터 스포너](#ai_01)  
>   7.2 [AI Perception](#ai_02)  
>   7.3 [Behavior Tree](#ai_03)  
> 9. [애니메이션](#animation)  
>   8.1 [플레이어 & 적 애니메이션 처리](#animation_01)  


<a name="combat"></a>
## 1. 전투 시스템
<a name="combat_01"></a>
### 1.1 프로젝트 개발 방향 & GAS 사용 이유
<hr>

프로젝트에서 가장 중요하게 여겼던 부분은 "멀티플레이" 환경에서의 개발이었습니다.    
Unreal Engine에서 멀티플레이 환경으로 개발한다는 점은 엔진의 프레임워크에 한층 더 깊이 이해할 수 있는 기회이기도 했습니다.   
> `GameMode`, `GameState`, `PlayerState` 같은 객체들이 언제 생성되고 어디서 가지는지 등..    

그런 이유로 전체 프레임워크를 설계해서 개발자로서 앞으로 성장함에 있어 기반을 마련하고 더 넓은 시야를 가지고자 개인 프로젝트를 진행했습니다.    

기술을 선택할때 눈여겨봤던 부분은 내 프로젝트의 방향성과 적합성이었습니다.  
Unreal Engine 5으로 넘어오면서 `Gameplay Ability System(GAS)`이라는 기능을 본격적으로 활발하게 지원되기 시작했고    
이는 멀티플레이 환경을 고려하여 개발된 시스템이었기 때문에 프로젝트와 가장 부합하다고 판단하였습니다.    


실제로 개발하면서 어떻게 이들을 동기화하고 처리할지에 대해 제가 생각했던 방법들이 `GAS`에서는 이미 구현되어 있다고 해도 과언이 아니었고,   
업계에서 `GAS` 사용에 대한 경험을 중요하게 여기고 있었기 때문에 배우면서 프로젝트에 적용하고자 했습니다.


<a name="combat_02"></a>
### 1.2 전투 구조
<hr>

`GAS`는 '무언가 한다' 라는 동작을 `Ability` 단위로 관리하는 방식으로 굉장히 추상적인 개념이었기 때문에 제일먼저 아래와 같은 상황들을 나열하고 고민했습니다.    
- 어디서부터 어디까지를 하나의 행동(`Ability`)으로 볼 것인가?
- 위에서 정의한 행동들을 어떻게 서로 유기적으로 엮을 것인가?

### 어디서부터 어디까지를 하나의 행동으로 볼 것인가?
<hr>

사람마다 보는 방식이 다르기 때문에 기준을 정하기 어려웠습니다.     
그렇기 때문에 우선 행동들을 각각 별도의 `Ability`로 나눈 다음 합칠 수 있다면 합치는 방향으로 작업하고자 했습니다.    

아래와 같은 행동들이 위의 내용에 대한 예시입니다.
- 공격    
  + 흐름이 자주 변경될 때가 많으며 중간 동작을 사용하는 경우도 있음    
    → 어떤 행동 이후에 공격의 일부부터 시작하는 경우가 해당    
- 에어본
  + Start -> Loop -> End의 흐름에서 벗어나는 일이 거의 없으며, 모든 상황에서 실행 순서가 동일함 (Start -> Loop -> End)    

`Ability`가 많아지기 때문에 관리가 힘들 수 있다는 단점이 있지만, 개별 `Ability`로 관리되기 때문에 오히려 행동을 구별하는데 있어 직관적이라고 생각했습니다.   
'기본공격'이라는 이름보다 '기본공격A', '기본공격AA'라는 이름이 다른 사람이 인지하기에 직관적이라고 판단했기 때문입니다.    

공격과 관련된 행동들은 개별 `Ability`로 관리했는데, 하나의 `Ability`로 묶어서 관리하게 되면 `Ability`연결 작업이 아래와 같이 나뉠 수 있습니다.   
- 다음 콤보(`AnimMontage`)로 이어가는 기능
- 다른 `Ability`로 분기할 수 있는 기능

'다음 콤보로 이어가라' / '다른 `Ability`로 분기해라' 처럼 2가지 행위로 분리되는 방식보다    
'다른 `Ability`로 분기해라' 라는 방식으로 통합하면 하나의 기능만 알면 위의 두 가지 상황을 모두 커버할 수 있다는 장점이 있었습니다.    


이런 방식이 `Ability`를 행동 단위로 분리하려는 의도에도 적합했습니다.



### 위에서 정의한 행동들을 어떻게 서로 유기적으로 엮을 것인가?
<hr>

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Combat_01.png" width="80%" height="80%"/>

`Ability`에서 다른 `Ability`로 분기될때 Input이나 캐릭터의 상태처럼 다양한 상황에서 처리될 수 있도록, 조건을 만족하면 실행하는 구조로 설계했습니다.  
또한 이후 확장성을 고려하여 실행되는 `로직(Action)`과 `조건(Condition)`을 객체화 하여 시스템을 구했습니다.  

구현할때에는 `AbilityTask`와 `AnimNotifyState` 두 가지의 경우로 생각해볼 수 있었습니다.  
하나의 `Ability`내에서 다른 `Ability`로 분기되는 지점이 많기에 `AbilityTask`로 구현하게 되면 서로의 타이밍을 비교해 구성하기 어려울것 같다고 판단했습니다.
> 1타 공격에서는 '가드', '회피', '2타', '기술 사용' 처럼 여러개의 동작으로 분기될 수 있음

그밖에도 `AbilityTask`는 인스턴스화 되어 런타임에 내부 변수를 저장할 수 공간이 있다는 장점과, 비동기 방식으로 실행되는 강점이 있지만  
`AnimNotifyState`를 통해 처리하는 방식으로 얻는 이점이 사용하지 않는 `AbilityTask`의 일부 기능을 잃는것보다 더 크다고 판단하여 `AnimNotifyState`로 결정했습니다.  
> 구현하고자 하는 시스템은 내부에 데이터를 저장하지 않아도 구현이 가능했고 비동기 방식으로 처리할 이유가 없었기 때문임

결과적으로 에디터를 통해 행동들을 연결하는 작업을 하면서 변화에 의한 내부 코드를 수정하지 않고도 유연하게 대처할 수 있었습니다.

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Combat_02.png" width="60%" height="60%"/>
</details>


<a name="combat_03"></a>
### 1.3 AttributeSet
<hr>

### AttributeSet 구조

`GameplayEffect`와 함께 동작되는 상태값 세트(`AttributeSet`)는 공통으로 가질 수 있는 부분을 고려하여 총 3가지로 분리했습니다.
| Type                          | Description                                                                                    |
| ----------------------------- | ---------------------------------------------------------------------------------------------- |
| CombatAttributeSet            | 캐릭터가 전투를 위해 최소한으로 가져야 하는 데이터 (스테미나, 그로기 수치, 가드 각도)            |
| EnemyAttributeSet             | AI로 처리되기 위한 데이터                                                                      |
| HealthAttributeSet            | 체력과 관련된 데이터 (체력, 받은 데미지)                                                        |

`HealthAttributeSet`의 데이터를 `CombatAttributeSet`에 포함하지 않은 이유는, 전투 기능없이 피해만 받는 Actor가 생길 수 있기 때문입니다.  
체력 정보와 전투 정보를 분리함으로써, 예를 들어 피해만 받는 Actor(훈련용 목각)에 필요한 데이터 집합만 포함시킬 수 있습니다.  
또한 관심사가 분리되기 때문에 코드 복잡도가 줄어들고 재사용성도 증가한다는 장점도 있었습니다.  

### AttributeSet 처리

<details>
<summary> 코드 접기 / 펼치기 </summary>

`HealthAttributeSet.h`
  
```C++
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	/* 예측이 필요없는 값은 Replicated로 생성할 필요가 없다. (Health -= ReceivedDamage) */
	UPROPERTY(BlueprintReadOnly, Category = "Combat Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ReceivedDamage;
```

</details>

AttributeSet에서 데이터를 추가할때 이 데이터는 어떻게 관리되어야 할 것인가에 대해 초점을 두고 작업했습니다.  
- `Health`와 `MaxHealth`의 경우, 다른 플레이어에게 체력이 보여야 하기 때문에 모두에게 복제
- 잠깐 저장되고 `Server`에서만 처리되는 `ReceivedDamage`는 굳이 복제할 필요가 없음
- 소유 클라이언트 본인만 알아야 하는 데이터인 경우 → `COND_OwnerOnly`를 통해 본인한테만 복제


<details>
<summary> 코드 접기 / 펼치기 </summary>

`HealthAttributeSet.h`
  
```C++
void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// BaseValue가 변경될시 Clamping 한다.
	// Regen같은 경우, Periodic 타입은 Instant(BaseValue)로 적용되므로, 이곳에서도 클램핑 작업이 이루어져야 한다.
	ClampValue(Attribute, NewValue);
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// CurrentValue가 변경될시 Clamping 한다.
	ClampValue(Attribute, NewValue);
}
```
</details>

`Attribute`의 `BaseValue`와 `CurrentValue` 값을 제한하기 위해 `PreAttributeBaseChange()`와 `PreAttributeChange()`  두 함수를 사용했습니다.		
`PreAttributeChange()`에서만 Clamping할 경우 → `BaseValue`가 비정상적인 값으로 변경되는걸 막을 수 없으므로 두 곳에서 Clamping을 처리해주었습니다.		

예를 들어, 공격력이 20인 상태에서 50만큼 감소하고 40만큼 증가하는 상황이 있다고 했을때

플레이하는 유저는 "공격력의 최소가 0이니까 40이겠네" 싶지만 아래의 흐름에선 그렇지 않은걸 볼 수 있었습니다.	

> 1. 공격력을 50 감소시키는 `Instant` 타입을 가진 `GameplayEffect`를 적용
>    + `BaseValue : -30`
>    + `CurrentValue : -30`		
> 3. 공격력을 40 증가시키는 `Duration` 타입을 가진 `GameplayEffect`를 적용	
>    + `BaseValue : -30`
>    + `CurrentValue : 10`	
> 4. 예상했던 40이라는 결과와 다르게 10이라는 최종 결괏값을 가짐		



[목차로 이동](#table-contents)

<a name="input"></a>
## 2. 입력 처리
<a name="input_01"></a>
### 2.1 플레이어 입력 프로세스
<hr>

플레이어의 입력을 처리함에 있어 두 가지 방법이 있었습니다.
1. `GAS`에서 표준적으로 제공하는 `Input ID`를 통한 매핑
2. `InputAction - Tag` 쌍으로 관리하는 방식으로 커스텀 매핑

(1) 번의 방식은 `ID`를 사용한 매핑 방식으로 다른 개발자의 입장에서 입력값을 파악하기 힘들것 같다는 우려가 있었습니다.    
그렇기 때문에 `InputAction`과 이를 식별하는 `Tag`를 한 쌍으로 관리하는 방식으로 하여 비교적 직관적으로 표현하여 처리할 수 있었습니다.

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_01.png" width="90%" height="90%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_02.png" width="50%" height="50%"/>
</details>

- 입력이 발생하여 `Tag`를 수신하면, 일치하는 `Input Tag`를 가진 Ability를 실행할 수 있습니다.    
- 입력은 서버로 전송하여 `Ability` 내에서 Task를 통한 입력 관련 로직을 지원했습니다.    

<a name="input_02"></a>
### 2.2 선입력 처리
<hr>

행동 도중 미리 키를 입력해 두면, 지정된 시점에 다음 동작으로 자연스럽게 연계되도록 하는 선입력 기능을 `AnimNotifyState`로 구현했습니다.
- 가장 마지막에 입력된 값이 기록됩니다.
- 기록된 입력은 지정된 구간에 소모하여 다음 동작으로 연결할 수 있습니다.

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_03.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_04.png" width="50%" height="50%"/>
</details>

`AnimNotifyState`를 활용한 선입력 처리는 아래와 같은 조건을 고려하여 결정했습니다.
- 특정 동작(ex: 팔을 올리는 모션) 이후부터 입력을 받아야 한다면 Task를 기반으로 처리할 경우 애니메이션의 재생 시간을 기준으로 직접 타이밍을 계산해야 하는 번거로움 존재    
> `AnimNotifyState`의 경우 이를 기반으로 타이밍이 조정되기 때문에 Task를 만들어 계산 로직을 재작성할 필요가 없다고 판단
- 여러 Task가 동시에 동작하는 상황에서 특정 Task가 먼저 실행되어야 하는 경우 각 Task의 실행시간을 일일이 확인하고 값을 기입하는 번거로움이 줄어듬
> `AnimNotifyState`로 작업할 경우 시각적으로 어떤 로직이 먼저 동작되는지 직관적

[목차로 이동](#table-contents)

<a name="cmc_network"></a>
## 3. 위치 & 회전 동기화
<a name="cmc_network_01"></a>
### 3.1 멀티플레이 환경에서 발생한 위치보정 이슈 처리
멀티 플레이 환경을 고려하며 개발하며 `RootMotion`과 회전을 같이 사용하는 과정에서 캐릭터의 움직임이 버벅이는 보정 현상이 발생했습니다.  
정상적인 범주의 핑(20ms)에서도 이런 현상이 발생했기 때문에 유저의 플레이 경험과 반응성 측면에서 개선하고자 했습니다.

**사용하고자 했던 회전 타입**
| Type                          | Description                                                |
| ----------------------------- | ---------------------------------------------------------- |
| ERotateTaskRule::InputVector  | 입력한 방향으로 회전합니다.                                 |
| ERotateTaskRule::Camera       | 카메라 방향으로 회전합니다.                                 |
| ERotateTaskRule::TrackCamera  | 지속적으로 카메라 방향으로 회전합니다.                      |

### 발생 원인
<hr>

`Character Movement Component(CMC)`는 기본적으로 `Client-Prediction` 방식으로 동작합니다.

1. 클라이언트는 이동을 수행하고 `FSavedMove_Character` 객체에 기록하여 서버에 전송합니다.
2. 서버에서는 받은 정보를 이용해 시뮬레이션 하고 클라이언트의 이동이 유효한지 체크합니다.        
3. 유효하지 않다면 클라이언트에 보정을 요청합니다.    

위 과정에서 서버의 시뮬레이션 결과가 클라이언트와 달랐기 때문에 발생한 현상이었으며 문제점을 파악했습니다.


### 무엇이 문제였나?
<hr>

<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_01.png" width="100%" height="100%"/>
</details>

근본적인 문제는 작성한 회전 로직에 있었습니다.
Server / Client의 프레임이 각각 다르고 지연시간이 커지는 경우 오차가 누적되어 잘못된 움직임으로 판단되어 보정이 발생했습니다.  
즉, 위 코드는 Client / Server의 환경을 고려하지 않은채로 각각 따로 수행되면서 발생하는 코드였습니다.


### 해결 방법
<hr>

생각해볼 수 있었던 방식은 두 가지가 있었습니다.

1. 클라이언트의 움직임을 특정 상황에만 수용하도록 한다.    
> - 특정 상황에서만 활성화한다고 하더라도, 그 사이에 잘못된 이동이 올 수 있다.    
> - 임의의 회전에 대해서 여전히 회전 불일치가 발생할 여지가 있어 이를 맞춰야 한다.  
>   CMC 내부적으로 클라이언트의 회전 값을 그대로 수용하진 않게 되어있어 따로 처리해 주어야 함을 의미    
> - 클라이언트의 환경(주로 인터넷 환경)에 따라 다른 플레이어에게는 순간이동하는 것처럼 보일 수 있다.    
2. 서버 측에 클라이언트의 회전을 동일하게 적용하여 시뮬레이션 결과를 맞춰본다.    
> - 서버에서 검증하는 방식이기 때문에 잘못된 이동을 확실하게 처리할 수 있다.    
> - CMC에서 제공하는 프로세스만 잘 따라준다면 좋은 반응성과 치트 방지가 모두 가능하다.    

위와 같은 장단점을 고려했을때 (2)번의 방식으로 처리하도록 했습니다.

```cpp
if (bAllowPhysicsRotationDuringAnimRootMotion || !HasAnimRootMotion())
{
    PhysicsRotation(DeltaSeconds);
}
```
마침 언리얼에서 제공하는 클라이언트 예측을 위한 회전 코드가 있었기에 이를 이용해서 활용하였습니다.

### 첫 시도
<hr>

<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_02.png" width="75%" height="75%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_03.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_04.png" width="85%" height="85%"/>
</details>

회전 방향(DesiredTargetRotation)과 회전량(RotateSpeedPerSec), Flag를 서버와 클라이언트에서 각자 설정하고  
`PhysicsRotation( )` 내부에서 설정된 값을 활용해 회전을 처리해주는 방식으로 처리했습니다.

### 첫 시도 결과
<hr>

처음에 비해 확실히 보정되는 현상이 줄었으나 여전히 발생하고 있었습니다.  
특히나 높은 지연시간에서는 이 현상이 더 많이 발생했습니다.

### 첫 시도 실패 원인 분석 
<hr>

왜 이런 문제가 발생했는지 알아보기 위해 로그를 사용하여 분석했습니다.  
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_05.png" width="75%" height="75%"/>

확인해본 결과 서버에서 한 번 더 회전 연산을 하고 있었기에 발생했던 문제였습니다.  
다른 모든 회전값들은 정상적으로 동기화되는 반면, 마지막 한 번의 연산으로 보정 현상이 발생했기 때문에  
CMC에서 제공하는 Client 기반의 회전 연산은 정상적으로 동작함을 확인할 수 있었으며  
Flag 값을 Server / Client 각각 설정하여 서버의 Flag 값이 늦게 비활성화되어 발생한 문제라고 가설을 세웠습니다.  


### 두 번째 시도
<hr>

두 번째 시도에서는 Flag값을 Client의 값으로 동기화 시키도록 하는 방식으로 처리하였습니다.
<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_06.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_07.png" width="75%" height="75%"/>
</details>

CMC에서 Client -> Server로 상태값을 전달하기 위해 사용되는 `FSavedMove`와 `FNetworkPrediction_Client`를 상속받고  
원하는 용도로 사용할 수 있게 만들어진 `FLAG_Custom`을 활용해서 동기화된 Flag는 `bWantsToRootMotionRotation` 변수에 저장하여 회전을 사용하고자 할때 설정해주었습니다.


### 두 번째 시도 결과 및 문제
<hr>

결과적으로 높은 Latency에서도 보정 현상이 발생하지 않고 RootMotion의 회전이 정상적으로 동작했습니다.    
하지만 여전히 두 가지 문제가 남아있었습니다.

1. Listen Server 환경에서 클라이언트의 동기화가 틀어지는 문제
> 원인을 파악하는데 시간은 걸렸으나, 해결은 비교적으로 단순했습니다.    
  Listen Server의 경우 부드럽게 회전하기 위해 최종값이 아닌 보간값을 사용하기 때문이었는데     
  `Listen Server Network Simulated Smooth Rotation Time` 값을 조정하는 것으로 해결했습니다.    
2. 지속적으로 회전하는 타입(ERotateTaskRule::TrackCamera)에서는 동기화가 틀어지는 문제


### 두 번째 시도 원인 분석
<hr>

지금까지 작업한 내용은 사용하고자 할때 목표 회전값과 회전량을 Client / Server 각각에서 설정했기 때문이었고       
지속적으로 변화하기 위해서는 이 두개의 값 역시 Client의 환경을 고려하여 Server에 값을 동기화 해주어야 했습니다.        

<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_08.png" width="75%" height="75%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_09.png" width="100%" height="100%"/>
</details>

상태가 아닌 별도의 추가 데이터를 전달하기 위한 용도인 `FCharacterNetworkMoveData` 구조체를 사용하도록 했습니다.    
이후 서버에서 이동을 처리하기 직전 `MoveAutonomous( )`에서 서버로 전달된 데이터를 기반으로 CMC 정보를 업데이트했습니다.

### 최종 결과
<hr>

Flag 및 목표 회전이 Client에 의해 동기화되고 서버에서 이를 검증하기 때문에 모든 동작이 정상적으로 잘 동작했습니다.    
높은 지연시간에도 RootMotion 도중 어느 방향으로 회전할지 실시간으로 설정할 수 있으며 정상적인 상황의 경우 보정이 발생하지 않았습니다.

[목차로 이동](#table-contents)


<a name="lyra"></a>
## 4. Lyra 컨텐츠 코드 활용 및 개선
<a name="lyra_01"></a>
### 4.1 AbilitySet
<hr>

`GAS`를 이용하여 개발하면서 다수의 Ability를 부여했을때 이들을 일괄적으로 회수하는 등 관리할 수단이 필요했고    
이들을 묶어서 관리할 수 있는 `AbilitySet`을 프로젝트에 도입해서 어느정도 해결했지만 구조상 고려할 문제가 있었습니다.    

- 장비를 교체할때 동일한 타입으로 부여된 `AbilitySet`의 `Handle`을 찾을 방법이 필요
  > Sword를 착용한 상태에서 Katana로 교체시, Sword로부터 부여된 Ability를 회수해야 함    
  > 장비 객체에 Handle을 저장하는 방식으로 해결되지만, 다른 타입(포션, 스킬) 처리를 위해 중복된 코드를 작성해야 할 수도 있음
- 회수할때 `Ability`가 이미 실행 중인 경우에는 회수해야 할지 정책이 필요   
  > A 스킬을 사용하는중에 회수되면 Ability가 조기 종료되어 의도되지 않게 다음 동작이 가능해짐

두 문제를 모두 만족할 수 있도록 기능을 확장했습니다.
- `Type Tag`를 추가하여 동일한 타입의 `AbilitySet`가 부여되면 자동으로 회수 / 부여
- `Ability Take Rule`을 추가하여 회수 과정에서 `Ability`가 실행 중인 경우 Pending 상태로 보내 끝난 후 회수할 수 있음

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Lyra_01.png" width="100%" height="100%"/>
</details>


<a name="lyra_02"></a>
### 4.2 Inventory
<hr>

`ItemObject`에 `ItemFragment`라는 부품을 끼워 필요한 데이터만 가지는 방식으로 아래와 같은 장점들이 감명 깊었기에 프로젝트에 적용하고 싶었습니다.    
- 일종의 템플릿처럼 유연하게 구현되어 있어 어느 프로젝트에나 적용할 수 있음   
- 복잡한 상속 구조를 피할수 있음
- 불필요한 데이터는 없애고 필요한 데이터만 취함
- 객체 단위로 이루어졌기 때문에 하드코딩할 가능성이 적음



Lyra Inventory는 미완성된 시스템이라는 느낌이 있었기 때문에 프로젝트에 적용하기 위해 필요한 기능들을 추가해야 했습니다.
1. 아이템 카운트 관리 및 정책    
2. 사용 가능한 아이템 처리

### 카운트 관리
<hr>

Lyra에서는 이를 Tag로 관리하고 있었지만, 카운트는 모든 아이템에 공통적으로 적용되는 속성이기도 하고    
무엇보다 아이템을 추가하거나 삭제하면서 지속적으로 변화하는 아이템 스택에 대한 처리가 없었기에 이를 구현했습니다.

**아이템 추가 과정**
1) 쌓을 수 있는 아이템 인스턴스가 있으면 → 카운트를 증가
2) 인스턴스의 카운트를 증가시키고도 카운트가 남으면 → 새로운 인스턴스를 생성
3) 더 추가할 카운트가 남았다면 → 위 과정을 남은 카운트가 0이 될 때까지 반복

**아이템 삭제 과정**
1) 쌓을 수 있는 아이템 인스턴스가 있으면 → 카운트를 감소
2) 인스턴스에 카운트를 감소하고 0개 아래가 된 경우 → 인스턴스 삭제
3) 더 감소할 카운트가 남았다면 → 위 과정을 남은 개수가 0이 될 때까지 마지막으로 획득한 아이템을 우선적으로 찾아 반복


### 사용 가능한 아이템 처리
<hr>

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Lyra_02.png" width="65%" height="65%"/>

<details>
<summary> 디테일 패널 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Lyra_03.png" width="100%" height="100%"/>  
</details>

포션이나 스킬처럼 퀵슬롯에 장착 가능한 아이템은 사용하면 효과가 발동되어야 하며 기존에 제공되는 `ItemFragment`를 확장했습니다.    
모든 사용 가능한 아이템은 퀵슬롯을 거쳐야 하기 때문에 퀵슬롯에 장착할 수 있는 `QuickSlot Fragment`를 상속받는 형태로 구현되었습니다.


[목차로 이동](#table-contents)



<a name="ui"></a>
## 5. UI
<a name="ui_01"></a>
### 5.1 UI 구조
<hr>

가장 상단의 UI부터 순차적으로 처리되는 `Stack` 구조를 목표로 구현하고자 했으며 `Common UI` 플러그인이 가장 적합하여 적용했습니다.    
Game Pad에 대한 포커스 지원도 해주기 때문에 배워두면 차후 UI를 설계할 때 구조적으로 도움이 되리라 판단했습니다.    

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/UI_01.png" width="80%" height="80%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/UI_02.png" width="100%" height="100%"/>

카테고리에 따라 `Z-Order`가 관리되도록 `UI Layer`를 가장 기반으로 배치했고 `Widget`들은 적합한 `UI Layer`에 쌓이게 됩니다.


[목차로 이동](#table-contents)



<a name="contents"></a>
## 6. 기능 구현
<a name="contents_01"></a>
### 6.1 퀵 슬롯
<hr>

<details>
<summary> 퀵슬롯 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_01.png" width="40%" height="40%"/>  
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_02.png" width="100%" height="100%"/>  
</details>

퀵슬롯 기능은 여러 타입의 슬롯에 아이템을 등록하고 용도에 따라 처리할 수 있도록 만들었습니다.    
빠르게 타입을 추가할 수 있도록 `Component`의 Detail 패널을 통해 시작시 등록할 퀵슬롯 타입(`FRegisteredQuickSlotInfo`)을 지정할 수 있습니다.    
등록된 퀵슬롯은 `FQuickSlot` 구조체로 저장되어 어떤 아이템을 들고 있는지, 어떤 인덱스가 활성화 되어 있는지 정보를 가집니다.

<details>
<summary> 코드 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_03.png" width="100%" height="100%"/>  
</details>

퀵슬롯에 아이템을 등록 / 삭제하거나 활성 인덱스에 변화가 생기면 Client로 복제되어 변경사항을 감지하고, 이벤트를 발생시켜 UI를 갱신합니다.



<a name="contents_01"></a>
### 6.2 캐릭터 외형
<hr>

<details>
<summary> 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_04.png" width="80%" height="80%"/>  
</details>

외형을 자유롭게 변경하고 애니메이션을 다양한 SkeletalMesh에 적용하기 위해 Skin 기능을 구현했습니다.   
`ChildActorComponent`를 이용해 메인 캐릭터의 하위 자손으로 추가되고, Retarget을 지원하여 UE4 <-> UE5 스켈레톤이 호환됩니다.


<a name="contents_02"></a>
### 6.3 상호작용
<hr>

플레이어와 상호작용이 가능한 물체를 처리하기 위해 공통적인 부분과 그렇지 않은 부분을 나누어 설계했습니다.

**공통 부분**    
- 주변에 상호작용 가능한 물체가 있는지 스캔
- 상호작용 가능한 물체에 다가갔을때 상호작용 정보

**비공통 부분**    
- 상호작용시 물체마다 반응하는 동작(로직)
- 물체가 상호작용 가능한 상태인지 판단하는 부분

위와 같은 이유로 플레이어의 주변을 스캔하는 행위와, 이를 통해 실행되는 상호작용 동작을 분리했습니다.    
이 방식에서는 새로운 상호작용이 필요한 물체가 추가되더라도 해당 물체에 맞는 `Ability`만 추가하면 되므로 확장이 용이했습니다.

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_05.png" width="80%" height="80%"/>  

- `InteractionAbility`는 어빌리티가 부여되는 시점에 실행
- `AbilityTask(ScanNearActor Task)`를 이용해 주변의 물체를 감지하고, 변경된 경우 이벤트 발생
- 발생한 이벤트를 활용해 `Client`는 상호작용 UI를 표시하고 `Server`에서는 각각 물체가 가진 `Ability`를 부여
- 상호작용 키를 누르면 부여받은 `Ability`가 실행됨

<details>
<summary> 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_06.png" width="100%" height="100%"/>  
</details>

상호작용이 가능한 물체임을 판단할 때에는 다른 타입을 가진 `Actor`에도 적용할 수 있도록 `Interface`를 활용했습니다.


<a name="contents_04"></a>
### 6.4 치명적인 일격(DeathBlow)
<hr>

적을 마무리할 수 있는 치명적인 공격 시스템을 구현했습니다.    
여러 명의 적이 겹쳐 있는 상황에서 마무리 일격을 가할 대상을 선택하는 것은 플레이어의 몫이므로    
플레이어를 기준으로 삼아 주변의 적을 스캔하며 아래 단계에 따라 처리하도록 설계했습니다.    

- 스캔된 적이 마무리 일격이 가능한 상태일 경우, `DeathBlow Executor Ability`가 플레이어에게 부여됩니다.
- 부여된 `Ability`를 실행할 별도의 입력이 필요하므로, `InputMappingContext`도 동적으로 추가됩니다.
- 부여된 `Ability`가 실행되면, 타겟에게 어떤 무기로 당하는지 나타내는 `Executed Tag`를 전송합니다.
- 타겟은 이를 수신한 뒤, 실행자의 `Ability`와 짝을 이루는 `DeathBlow Target Ability`가 실행됩니다.



[목차로 이동](#table-contents)




<a name="ai"></a>
## 7. AI 처리
<a name="ai_01"></a>
### 7.1 몬스터 스포너
<hr>

맵에 원하는 몬스터를 규칙에 맞게 스폰할 수 있도록 스포너를 구현했습니다.    
어떤 몬스터가 스폰될지 가중치를 설정하거나 어떤 스팟에 스폰될지 설정할 수 있습니다.

<details>
<summary> 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/AI_01.png" width="100%" height="100%"/>  
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/AI_02.png" width="50%" height="75%"/>  
</details>


<a name="ai_02"></a>
### 7.2 AI Perception
<hr>

<details>
<summary> 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/AI_03.png" width="50%" height="75%"/>  
</details>

AI가 시야를 통해 적을 감지하고 타겟을 지정할 수 있도록 `AI Perception Component`를 사용했습니다.    
그 밖에도 소리를 들었을때 발생한 Noise를 감지하거나 데미지를 받은 경우 어그로를 변경하도록 구현했습니다.    

위와 같은 동작에는 `Blackboard`의 `SenseValue(float)` 및 `LastSenseActor(Actor)`값 두개의 변수로 제어됩니다.    
- 감각 신호가 오면 `SenseValue`가 증가하고 `LastSenseActor`를 감각을 준 대상으로 지정합니다.    
- `SenseValue`가 일정 값 이상으로 증가하면 `LastSenseActor`를 `TargetActor`로 확정합니다.    

<a name="ai_03"></a>
### 7.3 Behavior Tree
<hr>

<details>
<summary> 이미지 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/AI_04.png" width="100%" height="100%"/>  
</details>

AI의 동작에는 `BehaviorTree`를 사용했으며 몬스터가 현재 어떤 상태인지에 따라 처리되도록 작업했습니다. (ex: 피격, 전투, 순찰..)    


[목차로 이동](#table-contents)


<a name="animation"></a>
## 8. 애니메이션
<a name="animation_01"></a>
### 8.1 플레이어 & 적 애니메이션 처리
<hr>

캐릭터의 애니메이션을 `Base Anim BP`를 상속받아 처리하고자 했으나 불필요한 트랜지션까지 공유해야 하는 구조적 문제가 있었습니다.    
프로젝트에서 구현한 적 캐릭터의 경우 일반 상태와 전투 상태의 애니메이션을 모두 사용하는데, 일반 상태가 필요하지 않은 적이 있다면..?    
-> 불필요한 일반 상태의 트랜지션을 포함하게 됩니다.    

<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Animation_01.png" width="40%" height="40%"/>

위와 같은 문제를 해결하기 위해 `Linked Anim Layer`를 활용했으며 상태들을 공통 트랜지션으로 병합하고 독립적으로 관리할 수 있었습니다.    
또한 `Linked Anim Layer`가 변경되더라도 현재의 `Base State Machine`은 그대로 동작하고 있기 때문에 상태 변화에 자연스러운 연결이 가능했습니다.    

> Montage Slot이 그대로 동작하고 있으므로 그 사이에 Layer를 변경하는 것으로 자연스럽게 연결됨

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Animation_02.png" width="75%" height="75%"/>
</details>

[목차로 이동](#table-contents)


