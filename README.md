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
>   1.1 [GAS 전투 구조](#combat_01)  
>   1.2 [공격 / 피격 처리](#combat_02)  
>   1.3 [데미지 파이프라인](#combat_03)  
> 2. [입력 처리](#input)  
>   2.1 [플레이어 입력 프로세스](#input_01)  
>   2.2 [선입력 처리](#input_02)  
> 3. [위치 & 회전 동기화](#cmc_network)  
>   3.1 [멀티플레이 환경에서 발생한 위치보정 이슈 처리](#cmc_network_01)  
> 4. [Lyra 컨텐츠 코드 활용 및 개선](#lyra)  
>   4.1 [AbilitySet](#lyra_01)  
>   4.2 [Inventory](#lyra_02)  
> 5. [UI](#ui)  
>   5.1 [UI 구조](#ui_01)  
> 6. [기능 구현](#contents)  
>   6.1 [퀵 슬롯](#contents_01)  
>   6.2 [캐릭터 외형](#contents_02)  
>   6.3 [상호작용](#contents_03)  
>   6.4 [치명적인 일격(DeathBlow)](#contents_04)  
> 7. [AI 처리](#ai)  
>   7.1 [몬스터 스포너](#ai_01)  
>   7.2 [AI Perception](#ai_02)  
>   7.3 [Behavior Tree](#ai_03)  
> 8. [애니메이션](#animation)  
>   8.1 [플레이어 & 적 애니메이션 처리](#animation_01)  


<a name="combat"></a>
## 1. 전투 시스템
<a name="combat_01"></a>
### 1.1 GAS 전투 구조
GAS 전투 구조 테스트
GAS를 프로젝트에 적용하여 작업하면서 다양한 행동으로 분기하는 과정에서 복잡도가 매우 증가했습니다.  
크게 두 가지로 나누어 **어떤 행동들을 하나의 Ability로 쪼개야 하는지**, 그리고 **유지보수함에 있어 시각적으로 편리해야 하는지**에 대해 생각했습니다.

- Ability를 어떤 기준으로 쪼개야 할지 기준이 모호했음 (좌클릭 5타를 하나의 Ability로 보는게 맞는지, 개별로 쪼개는게 맞는지)

[목차로 이동](#table-contents)

<a name="input"></a>
## 2. 입력 처리
<a name="input_01"></a>
### 2.1 플레이어 입력 프로세스
<hr>
플레이어의 입력을 처리함에 있어 두 가지 방법이 있었습니다.
1. GAS에서 표준적으로 제공하는 `Input ID`를 통한 매핑
2. `InputAction - Tag` 쌍으로 관리하는 방식으로 커스텀 매핑

(1) 번의 방식은 `ID`를 사용한 매핑 방식으로 다른 개발자의 입장에서 입력값을 파악하기 힘들것 같다는 우려가 있었습니다.    
그렇기 때문에 `InputAction`과 이를 식별하는 `Tag`를 한 쌍으로 관리하는 방식으로 하여 비교적 직관적으로 표현하여 처리할 수 있었습니다.

<details>
<summary> 이미지 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_01.png" width="90%" height="90%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Input_02.png" width="50%" height="50%"/>
</details>

- 입력이 발생하여 `Tag`를 수신하면, 일치하는 `Input Tag`를 가진 Ability를 실행할 수 있습니다.    
- 입력은 서버로 전송하여 Ability 내에서 Task를 통한 입력 관련 로직을 지원했습니다.    

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
> AnimNotifyState의 경우 이를 기반으로 타이밍이 조정되기 때문에 Task를 만들어 계산 로직을 재작성할 필요가 없다고 판단
- 여러 Task가 동시에 동작하는 상황에서 특정 Task가 먼저 실행되어야 하는 경우 각 Task의 실행시간을 일일이 확인하고 값을 기입하는 번거로움이 줄어듬
> AnimNotifyState로 작업할 경우 시각적으로 어떤 로직이 먼저 동작되는지 직관적

[목차로 이동](#table-contents)

<a name="cmc_network"></a>
## 3. 위치 & 회전 동기화
<a name="cmc_network_01"></a>
### 3.1 멀티플레이 환경에서 발생한 위치보정 이슈 처리
멀티 플레이 환경을 고려하며 개발하며 RootMotion과 회전을 같이 사용하는 과정에서 캐릭터의 움직임이 버벅이는 보정 현상이 발생했습니다.  
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
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_01.png" width="80%" height="80%"/>  
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_02.png" width="80%" height="80%"/>  
</details>

퀵슬롯 기능은 여러 타입의 슬롯에 아이템을 등록하고 용도에 따라 처리할 수 있도록 만들었습니다.    
빠르게 타입을 추가할 수 있도록 Component의 Detail 패널을 통해 시작시 등록할 퀵슬롯 타입(`FRegisteredQuickSlotInfo`)을 지정할 수 있습니다.    
등록된 퀵슬롯은 `FQuickSlot` 구조체로 저장되어 어떤 아이템을 들고 있는지, 어떤 인덱스가 활성화 되어 있는지 정보를 가집니다.

<details>
<summary> 코드 접기 / 펼치기 </summary>    
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/Contents_03.png" width="80%" height="80%"/>  
</details>

퀵슬롯에 아이템을 등록 / 삭제하거나 활성 인덱스에 변화가 생기면 Client로 복제되어 변경사항을 감지하고, 이벤트를 발생시켜 UI를 갱신합니다.



<a name="contents_01"></a>
### 6.2 캐릭터 외형
<hr>


<a name="contents_02"></a>
### 6.3 상호작용
<hr>


<a name="contents_03"></a>
### 6.4 치명적인 일격(DeathBlow)
<hr>



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


