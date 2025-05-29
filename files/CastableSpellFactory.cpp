#include "CastableSpellFactory.h"
#include "CastableSpell.h"
#include "SpellModifier.h"
#include "SpellComposition.h"
#include "GolemGame/Spell System/Spell.h"

UCastableSpell* UCastableSpellFactory::CreateCastableSpell(UObject* Outer,
                                                           TArray<TSubclassOf<UIModdable_Base>> ComponentClasses)
{
	TArray<USpellModifier*> Modifiers;
	TArray<UIModdable_Base*> CachedSpellCompositions;
	TArray<UIModdable_Base*> Components;

	for (TSubclassOf<UIModdable_Base> IModdable_Base_Class : ComponentClasses)
	{
		UIModdable_Base* Component = NewObject<UIModdable_Base>(Outer, IModdable_Base_Class);
		Components.Add(Component);
	}

	for (UIModdable_Base* Element : Components)
	{
		if (USpell* Spell = Cast<USpell>(Element))
		{
			Spell->Init();
		}
	}

	ExtractModifiers(Modifiers, Components, CachedSpellCompositions);
	ApplyModifiers(Modifiers, Components, CachedSpellCompositions);

	UCastableSpell* Spell = NewObject<UCastableSpell>(Outer);
	Spell->Init(Components);
	return Spell;
}


void UCastableSpellFactory::ExtractModifiers(TArray<USpellModifier*>& Modifiers, TArray<UIModdable_Base*>& Components,
                                             TArray<UIModdable_Base*>& CachedSpellCompositions)
{
	for (int i = Components.Num() - 1; i >= 0; i--)
	{
		if (USpellModifier* Mod = Cast<USpellModifier>(Components[i]))
		{
			Mod->IndexInComponentsList = i;
			Modifiers.Add(Mod);
		}
	}
}

void UCastableSpellFactory::ApplyModifiers(TArray<USpellModifier*>& Modifiers, TArray<UIModdable_Base*>& Components,
                                           TArray<UIModdable_Base*>& CachedSpellCompositions)
{
	if (Components.Num() == 0)
	{
		return;
	}

	while (Modifiers.Num() > 0)
	{
		// In their Apply-Method, all Modifiers MUST...
		// create a new SpellComposition with the affected spells,
		// then remove itself and all modified Spells/Compositions in the Components array, and
		// put the new SpellComposition at the modifiers initial IndexInComponentsList 
		Modifiers[0]->Apply(Components);
		Modifiers.RemoveAt(0);
	}

	if (!Components[0]->IsA(USpellComposition::StaticClass()))
	{
		USpellComposition* NewSpell = NewObject<USpellComposition>(GetTransientPackage());
		NewSpell->Init({Components[0]});
		Components[0] = NewSpell;
	}

}

bool UCastableSpellFactory::IsValidCombination(const TArray<TSubclassOf<UIModdable_Base>> Components)
{
	bool ValidCombination = false;
	for (TSubclassOf<UIModdable_Base> IModdable_Base_Class : Components)
	{

		if (IModdable_Base_Class->IsChildOf(USpell::StaticClass())
			|| IModdable_Base_Class->IsChildOf(USpellComposition::StaticClass()))
		{
			ValidCombination = true;
		}
	}

	return ValidCombination;
}
