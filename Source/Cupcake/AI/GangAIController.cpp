// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAIController.h"
#include "GangAICharacter.h"

AGangAIController::AGangAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGangAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGangAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AGangAICharacter* ControlledCharacter = Cast<AGangAICharacter>(GetPawn());


}




