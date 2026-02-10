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
> 3. [위치 & 회전 동기화](#cmc-network)  
>   3.1 [멀티플레이 환경에서 발생한 위치보정 이슈 처리](#cmc_network_01)  
> 4. [Lyra 컨텐츠 코드 활용 및 개선](#lyra)  
>   4.1 [AbilitySet](#lyra_01)  
>   4.2 [Inventory](#lyra_02)  
> 5. [UI](#ui)  
>   5.1 [UI 구조](#ui_01)  
>   5.2 [컨텐츠 UI](#ui_02)  
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
- 만약 


<a name="cmc_network"></a>
## 3. 위치 & 회전 동기화
<a name="cmc_network_01"></a>
### 1.1 멀티플레이 환경에서 발생한 위치보정 이슈 처리
멀티 플레이 환경을 고려하며 개발하며 RootMotion과 회전을 같이 사용하는 과정에서 캐릭터의 움직임이 버벅이는 보정 현상이 발생했습니다.  
정상적인 범주의 핑(20ms)에서도 이런 현상이 발생했기 때문에 유저의 플레이 경험과 반응성 측면에서 개선하고자 했습니다.

**사용하고자 했던 회전 타입**
| Type                          | Description                                                |
| ----------------------------- | ---------------------------------------------------------- |
| ERotateTaskRule::InputVector  | 입력한 방향으로 회전합니다.                                 |
| ERotateTaskRule::Camera       | 카메라 방향으로 회전합니다.                                 |
| ERotateTaskRule::TrackCamera  | 지속적으로 카메라 방향으로 회전합니다.                      |

**발생 원인**  
Character Movement Component(CMC)는 기본적으로 Client-Prediction 방식으로 동작합니다.
1. 클라이언트는 이동을 수행하고 FSavedMove_Character 객체에 기록하여 서버에 전송합니다.
2. 서버에서는 받은 정보를 이용해 시뮬레이션 하고 클라이언트의 이동이 유효한지 체크합니다.
3. 유효하지 않다면 클라이언트에 보정을 요청합니다.

위 과정에서 서버의 시뮬레이션 결과가 클라이언트와 달랐기 때문에 발생한 현상이었으며 문제점을 파악했습니다.

**무엇이 문제였나?**  
<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_01.png" width="100%" height="100%"/>
</details>

근본적인 문제는 작성한 회전 로직에 있었습니다.
Server / Client의 프레임이 각각 다르고 지연시간이 커지는 경우 오차가 누적되어 잘못된 움직임으로 판단되어 보정이 발생했습니다.  
즉, 위 코드는 Client / Server의 환경을 고려하지 않은채로 각각 따로 수행되면서 발생하는 코드였습니다.

**해결 방법**  
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

**첫 시도**
<details>
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_02.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_03.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_04.png" width="100%" height="100%"/>
</details>

회전 방향(DesiredTargetRotation)과 회전량(RotateSpeedPerSec), Flag를 서버와 클라이언트에서 각자 설정하고  
PhysicsRotation( ) 내부에서 설정된 값을 활용해 회전을 처리해주는 방식으로 처리했습니다.

**첫 시도 결과**    


처음에 비해 확실히 보정되는 현상이 줄었으나 여전히 발생하고 있었습니다.  
특히나 높은 지연시간에서는 이 현상이 더 많이 발생했습니다.

**첫 시도 실패 원인 분석**    


왜 이런 문제가 발생했는지 알아보기 위해 로그를 사용하여 분석했습니다.  
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_05.png" width="100%" height="100%"/>
확인해본 결과 서버에서 한 번 더 회전 연산을 하고 있었기에 발생했던 문제였습니다.  
다른 모든 회전값들은 정상적으로 동기화되는 반면, 마지막 한 번의 연산으로 보정 현상이 발생했기 때문에  
CMC에서 제공하는 Client 기반의 회전 연산은 정상적으로 동작함을 확인할 수 있었으며  
Flag 값을 Server / Client 각각 설정하여 서버의 Flag 값이 늦게 비활성화되어 발생한 문제라고 가설을 세웠습니다.  


**두 번째 시도**    


두 번째 시도에서는 Flag값을 Client의 값으로 동기화 시키도록 하는 방식으로 처리하였습니다.
<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_06.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_07.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_08.png" width="100%" height="100%"/>
</details>

CMC에서 Client -> Server로 상태값을 전달하기 위해 사용되는 FSavedMove와 FNetworkPrediction_Client를 상속받고  
원하는 용도로 사용할 수 있게 만들어진 FLAG_Custom을 활용했으며 동기화된 Flag는  
bWantsToRootMotionRotation 변수에 저장하여 회전을 사용하고자 할때 설정해주었습니다.


**두 번째 시도 결과 및 문제**


결과적으로 높은 Latency에서도 보정 현상이 발생하지 않고 RootMotion의 회전이 정상적으로 동작했습니다.    
하지만 여전히 두 가지 문제가 남아있었습니다.
1. Listen Server 환경에서 클라이언트의 동기화가 틀어지는 문제
> 원인을 파악하는데 시간은 걸렸으나, 해결은 비교적으로 단순했습니다.    
  Listen Server의 경우 부드럽게 회전하기 위해 최종값이 아닌 보간값을 사용하기 때문이었는데     
  Listen Server Network Simulated Smooth Rotation Time 값을 조정하는 것으로 해결했습니다.    
2. 지속적으로 회전하는 타입(ERotateTaskRule::TrackCamera)에서는 동기화가 틀어지는 문제


**두 번째 시도 원인 분석**    
지금까지 작업한 내용은 사용하고자 할때 목표 회전값과 회전량을 Client / Server 각각에서 설정했기 때문이었고       
지속적으로 변화하기 위해서는 이 두개의 값 역시 Client의 환경을 고려하여 Server에 값을 동기화 해주어야 했습니다.        

<summary> 코드 접기 / 펼치기 </summary>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_09.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_10.png" width="100%" height="100%"/>
<img src="https://github.com/BUOMACC/ProjectV_SourceCode/blob/main/Images/CMC_11.png" width="100%" height="100%"/>
</details>

상태가 아닌 별도의 추가 데이터를 전달하기 위한 용도인 FCharacterNetworkMoveData 구조체를 사용하도록 했습니다.    
이후 서버에서 이동을 처리하기 직전 MoveAutonomous( )에서 서버로 전달된 데이터를 기반으로 CMC 정보를 업데이트했습니다.


**최종 결과**  
Flag 및 목표 회전이 Client에 의해 동기화되고 서버에서 이를 검증하기 때문에 모든 동작이 정상적으로 잘 동작했습니다.    
높은 지연시간에도 RootMotion 도중 어느 방향으로 회전할지 실시간으로 설정할 수 있으며 정상적인 상황의 경우 보정이 발생하지 않았습니다.



