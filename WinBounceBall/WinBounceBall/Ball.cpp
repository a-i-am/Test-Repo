#include "pch.h"
#include "Ball.h"
#include "Game.h"
#include "GameScene.h"
#include "InputManager.h"

Ball::Ball(Vector pos) : Super(pos)
{
}

void Ball::Init()
{
    Super::Init();

    CreateSpriteComponent("Ball_0", BALL_SIZE, BALL_SIZE);
}

void Ball::Update(float deltaTime)
{
    GameScene* scene = dynamic_cast<GameScene*>(Game::GetInstance()->GetCurrScene());
    if (nullptr == scene)
        return;

    Super::Update(deltaTime);

    // 등속도 운동
    //velocity = _gravityVec;
    acceleration = Vector(0, 0);

    // 좌우 이동은 힘을 500정도로 주고 싶다.
    float moveForce = 500;
    float mass = 1.f;   // 질량은 1 or 무거우면 더 큰값

    // 키보드 좌우 입력 처리
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
    {
        // 힘 = 질량*가속도
        // 힘 / 질량 = 가속도
        acceleration.x -= moveForce / mass;
    }
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
    {
        acceleration.x += moveForce / mass;
    }

    applyPhysics(deltaTime);
}

void Ball::Render(HDC renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Ball::GetRenderLayer()
{
    return RenderLayer::Ball;
}

void Ball::applyPhysics(float deltaTime)
{
    GameScene* scene = dynamic_cast<GameScene*>(Game::GetInstance()->GetCurrScene());
    if (nullptr == scene)
        return;

    // 중력 가속도 적용
    acceleration += _gravityVec;

    // 가속도는 속도(velocity)를 변화시킨다.
    velocity += acceleration * deltaTime;

    // velocity 에 대해서 최대 속도로 제한하고 싶다.
    float upMaxFactor = 400;   // 위아래 (중력방향이 아래쪽으로 흘러서)
    float sideMaxFactor = 150; // 좌우 (중력방향과 직교(접선))

    // 속도 제한 하는 방식
    bool applyVectorBreak = true;
    if (false == applyVectorBreak)
    {
        // 고정 X,Y 축  속도 제한 방식
        if (velocity.x > sideMaxFactor)
            velocity.x = sideMaxFactor;
        if (velocity.x < -sideMaxFactor)
            velocity.x = -sideMaxFactor;

        if (velocity.y > upMaxFactor)
            velocity.y = upMaxFactor;
        if (velocity.y < -upMaxFactor)
            velocity.y = -upMaxFactor;

        // 마찰계수 
        float friction = 0.95f;
        velocity.x *= friction;
        if (std::abs(velocity.x) < 1.0f)
            velocity.x = 0.f;
    }
    else
    {
        // 벡터 분해
        // 중력 방향 축에 해당하는 힘을 알고 싶다.
        Vector norGravity = _gravityVec.GetNormalize();

        // 현재 속도에서 중력 방향의 축에 해당하는 힘만 추출
        float gravityLength = velocity.Dot(norGravity);

        // 중력축에 반대되는 사이드 벡터
        // v : (3,4), x :(3,0), y : (0,4)
        // v - x = y
        // v - y = x

        //velocity 에서 중력방향 축의 힘을 제거한, (3,0) or (0,4)같은 벡터를 구한다.
        Vector gravityVector = norGravity * gravityLength;
        Vector sideVector = velocity - (gravityVector);

        // 사이드 벡터에 대한 힘은 
        float sideLength = sideVector.Length();

        // 중력축에 해당하는 힘이 Max치보다 크다면 제한
        if (gravityLength > upMaxFactor)
        {
            gravityVector = (norGravity * upMaxFactor);
        }

        // 사이드 방향에 해당하는 힘이 Max치보다 크다면 제한
        if (sideLength > sideMaxFactor)
        {
            sideVector = (sideVector.GetNormalize() * sideMaxFactor);
        }

        // 마찰계수 
        float friction = 0.95f;
        sideVector *= friction;
        if (sideVector.Length() < 1.0f)
            sideVector = Vector(0.f, 0.f);

        // 최종적인 velocity
        // v : (3,4), a :(3,0), b : (0,4)
        // v = a + b
        velocity = gravityVector + sideVector;
    }
   

    // 속도(velocity) 위치를 변화시킨다.
    Vector oldPos = GetPos();
    Vector newPos = oldPos + (velocity * deltaTime);

    Vector normal; // 법선 벡터
    Vector hitPos;

    Vector dir = newPos - oldPos;
    dir.Normalize();

    Vector start = oldPos;
    Vector end = newPos;

    // 비트마스크
    // 비행기 vs 비행기의 총알 : (ignore)충돌을 무시한다.  bit mask
    // 언리얼
    // ObjectType : Pawn, Static, Dynamic, ...
    // Panw vs Pawn : Overlap (true), Blokc(false)
    // Pawn vs Static Object(지형지물) : Block(true), Overlap(false);

    if (scene->CheckCollision(this, start, end, normal, hitPos))
    {
        // 바운스볼은 입사 벡터가 크기가 항상 고정
        Vector inputVector = velocity.GetNormalize() * upMaxFactor;
        
        // 충돌체크가 되었다
        // 반사벡터를 통해서, 반대로 튕겨 나가야 한다.
        // 입사 벡터 : velociy
        // 반사 벡터 : velocity 를 반사 벡터로 적용

        // 한번 부딪힐때마다 힘이 절반으로 줄었으면 좋겟다.
        float reflectFactor = 1.0f; // 탄성 계수

        Vector reflect = inputVector + (normal * 2.0f * -inputVector.Dot(normal));
        velocity = reflect * reflectFactor;

        newPos = hitPos;    // newPos은 사각형을 뚫고 지나가니, 사각형 위쪽 좌표로 보정
    }

    // 해당 새로운 위치로 설정하기 전에. 충돌체크를 한번해서 벽인지 확인
    SetPos(newPos);

    // 로그로 수치를 확인
    OutputDebugStringW(std::format(L"vel:{0},{1}, acc:{2},{3}, prevPos:{4},{5} newPos:{6},{7}\n",
        velocity.x, velocity.y,
        acceleration.x, acceleration.y,
        oldPos.x, oldPos.y, newPos.x, newPos.y).c_str());
}
