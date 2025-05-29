#pragma once

#include "CoreMinimal.h"
#include "IModdable_Base.h"
#include "UObject/Object.h"
#include "CastableSpellFactory.generated.h"

class USpellComposition;
class USpellModifier;
class UCompositionComponent;
class UCastableSpell;

UCLASS()
class GOLEMGAME_API UCastableSpellFactory : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UCastableSpell* CreateCastableSpell(UObject* Outer, TArray<TSubclassOf<UIModdable_Base>> Components);

	// used in UI programming (Blueprints) only  
	UFUNCTION(BlueprintCallable)
	static bool IsValidCombination(const TArray<TSubclassOf<UIModdable_Base>> Components);

protected:
	static void ExtractModifiers(TArray<USpellModifier*>& Modifiers, TArray<UIModdable_Base*>& Components,
	                             TArray<UIModdable_Base*>& CachedSpellCompositions);
	static void ApplyModifiers(TArray<USpellModifier*>& Modifiers, TArray<UIModdable_Base*>& Components,
	                           TArray<UIModdable_Base*>& CachedSpellCompositions);
};
