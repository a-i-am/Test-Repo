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
    float upFactor = 400;   // 위아래
    float sideFactor = 150; // 좌우

    if (velocity.x > sideFactor)
        velocity.x = sideFactor;

    if (velocity.y > upFactor)
        velocity.y = upFactor;


    // 속도(velocity) 위치를 변화시킨다.
    Vector oldPos = GetPos();
    Vector newPos = oldPos + (velocity * deltaTime);

    Vector normal; // 법선 벡터
    Vector hitPos;

    Vector dir = newPos - oldPos;
    dir.Normalize();

    Vector start = oldPos;
    Vector end = newPos;

    if (scene->CheckCollision(this, start, end, normal, hitPos))
    {
        // 바운스볼은 입사 벡터가 크기가 항상 고정
        Vector inputVector = velocity.GetNormalize() * upFactor;
        
        // 충돌체크가 되었다
        // 반사벡터를 통해서, 반대로 튕겨 나가야 한다.
        // 입사 벡터 : velociy
        // 반사 벡터 : velocity 를 반사 벡터로 적용

        // 한번 부딪힐때마다 힘이 절반으로 줄었으면 좋겟다.
        float reflectFactor = 1.0f; // 탄성 계수

        Vector reflect = velocity + (normal * 2.0f * -velocity.Dot(normal));
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
