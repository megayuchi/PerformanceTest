
https://megayuchi.com/2023/12/13/cpu%eb%b2%a4%ec%b9%98%eb%a7%88%ed%81%ac-2023%eb%85%84-surfaceprox-windows-devkit-2023-apple-m1-i7-13700k-i7-13620h-amd-ryzen-z1-extreme/

방송에서도 몇 번 언급했었고 예전에 블로그에 포스팅도 했던 벤치마크 자료 최신 버전입니다.
그리고 테스트에 사용한 코드를 공개했습니다.
코드 공개를 안하는 이유는 다음과 같은데요.
1. 쪽팔린다.
2. 공개해두면 시비 거는 인간들이 있다.

감안해서 맘에 안들거나 잘못 작성했다고 판단되는 부분이 있으면 고쳐서 쓰시면 되겠습니다.
참고로 ARM아키텍처의 BIG/LITTLE구조에 대응을 하고 있긴 한데 mac에서 Parallels로 Windows on ARM을 구동하는 경우는 성능코어와 저전력코어 구분이 되질 않습니다. 이건 VM환경에선 딱히 방법이 없어보입니다. 사실 어떤 코어를 지정해도 성능에 차이가 없었습니다.

2개의 프로젝트가 있는데 각각 다음과 같이 테스트를 수행합니다.
Perftest -> 4칙연산 / 메모리카피 / 스레드 반응성 테스트
PerfTest2 -> 수학함수 테스트

실제로 테스트에 사용하실 경우 다음과 같은 방식으로 테스트 하시는걸 권장합니다.

release로 빌드 후
PerfTest_x64_release.exe /onlymath > PerfTest_00.txt
PerfTest_x64_release.exe /onlymath > PerfTest_01.txt
PerfTest_x64_release.exe /onlymath > PerfTest_02.txt
PerfTest_x64_release.exe /onlymath > PerfTest_03.txt
이런식으로 테스트 결과를 4개 정도 뽑은 후에 그 중에서 가장 낮은 숫자를 택하는 방식으로 테스트 하시는게 좋습니다. 코드 실행중에 태스크권한을 뺏기면 소모 클럭이 왕창 올라간걸로 측정 되니까요.

/onlymath 스위치는 4칙연산에 대한 테스트만 수행합니다. /onlymath 스위치를 붙이기 않을 경우
4칙연산 / 메모리카피/ 스레드 반응성 테스트 를 수행합니다. 스레드 반응성 테스트가 상당히 시간을 잡아먹고 여러 차례 테스트하는 의미가 없으므로 반복 테스트를 할 때는 잊지 말고 /onlymath스위치를 붙이도록 합시다. 아니면 코드를 수정해서 아예 스킵시켜도 됩니다.
