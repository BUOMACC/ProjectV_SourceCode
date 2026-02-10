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
