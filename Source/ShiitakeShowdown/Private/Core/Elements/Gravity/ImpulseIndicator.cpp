// 2022 ChronoOwl Studios


#include "Core/Elements/Gravity/ImpulseIndicator.h"

#include "Components/SphereComponent.h"

// Sets default values
AImpulseIndicator::AImpulseIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent =CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(30.0f);
	SphereComponent->SetVisibility(true);
	SphereComponent->SetHiddenInGame(false);

}

// Called when the game starts or when spawned
void AImpulseIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AImpulseIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

