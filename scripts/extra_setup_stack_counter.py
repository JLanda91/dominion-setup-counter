import enum
from functools import lru_cache
import re
from collections import defaultdict


class EffectiveCardType(enum.IntEnum):
    Loot = 0,
    Omen = 1,
    Knights = 2,
    Druid = 3,
    Looter = 4,
    Fate = 5,
    Doom = 6,
    Liaison = 7,
    Empty = 8,
    YoungWitch = 9,
    Ferryman = 10,
    Riverboat = 11,


class ExtraSetupCard(enum.IntEnum):
    YoungWitch = 0,
    ApproachingArmy = 1,
    WayOfTheMouse = 2,
    Ferryman = 3,
    Riverboat = 4,
    Obelisk = 5

    def to_mask(self) -> int:
        return 1 << self.value


extra_setup_card_repr_re = re.compile(r"^<ExtraSetupCard\.(\w+):.*$")


class KingdomEquivalenceClass:
    def __init__(self, vdrm: int, effective_card_type: EffectiveCardType, size: int, num_action_or_treasure: int):
        self.vdrm = vdrm
        self.effective_card_type = effective_card_type
        self.size = size
        self.num_action_or_treasure = num_action_or_treasure

    def is_extra_setup_card_ec(self, extra_setup_card: ExtraSetupCard) -> bool:
        return bool(self.vdrm & extra_setup_card.to_mask())


EC_ARRAY = [
    KingdomEquivalenceClass(42, EffectiveCardType.YoungWitch, 1, 1),
    KingdomEquivalenceClass(50, EffectiveCardType.Knights, 1, 1),
    KingdomEquivalenceClass(37, EffectiveCardType.Druid, 1, 1),
    KingdomEquivalenceClass(48, EffectiveCardType.Ferryman, 1, 1),
    KingdomEquivalenceClass(45, EffectiveCardType.Riverboat, 1, 1),
    KingdomEquivalenceClass(40, EffectiveCardType.Looter, 1, 1),
    KingdomEquivalenceClass(42, EffectiveCardType.Looter, 1, 1),
    KingdomEquivalenceClass(50, EffectiveCardType.Looter, 1, 1),
    KingdomEquivalenceClass(2, EffectiveCardType.Fate, 1, 1),
    KingdomEquivalenceClass(37, EffectiveCardType.Fate, 2, 2),
    KingdomEquivalenceClass(40, EffectiveCardType.Fate, 2, 2),
    KingdomEquivalenceClass(45, EffectiveCardType.Fate, 1, 1),
    KingdomEquivalenceClass(48, EffectiveCardType.Fate, 1, 1),
    KingdomEquivalenceClass(2, EffectiveCardType.Doom, 1, 0),
    KingdomEquivalenceClass(42, EffectiveCardType.Doom, 1, 1),
    KingdomEquivalenceClass(45, EffectiveCardType.Doom, 1, 1),
    KingdomEquivalenceClass(48, EffectiveCardType.Doom, 1, 1),
    KingdomEquivalenceClass(50, EffectiveCardType.Doom, 2, 2),
    KingdomEquivalenceClass(0, EffectiveCardType.Liaison, 1, 1),
    KingdomEquivalenceClass(1, EffectiveCardType.Liaison, 1, 1),
    KingdomEquivalenceClass(37, EffectiveCardType.Liaison, 1, 1),
    KingdomEquivalenceClass(40, EffectiveCardType.Liaison, 1, 1),
    KingdomEquivalenceClass(45, EffectiveCardType.Liaison, 3, 3),
    KingdomEquivalenceClass(48, EffectiveCardType.Liaison, 2, 2),
    KingdomEquivalenceClass(32, EffectiveCardType.Omen, 1, 1),
    KingdomEquivalenceClass(40, EffectiveCardType.Omen, 3, 3),
    KingdomEquivalenceClass(48, EffectiveCardType.Omen, 1, 1),
    KingdomEquivalenceClass(50, EffectiveCardType.Omen, 1, 1),
    KingdomEquivalenceClass(0, EffectiveCardType.Loot, 2, 2),
    KingdomEquivalenceClass(1, EffectiveCardType.Loot, 1, 1),
    KingdomEquivalenceClass(34, EffectiveCardType.Loot, 1, 1),
    KingdomEquivalenceClass(37, EffectiveCardType.Loot, 1, 1),
    KingdomEquivalenceClass(48, EffectiveCardType.Loot, 1, 1),
    KingdomEquivalenceClass(0, EffectiveCardType.Empty, 34, 26),
    KingdomEquivalenceClass(1, EffectiveCardType.Empty, 7, 5),
    KingdomEquivalenceClass(2, EffectiveCardType.Empty, 3, 2),
    KingdomEquivalenceClass(8, EffectiveCardType.Empty, 15, 9),
    KingdomEquivalenceClass(9, EffectiveCardType.Empty, 10, 5),
    KingdomEquivalenceClass(32, EffectiveCardType.Empty, 43, 43),
    KingdomEquivalenceClass(34, EffectiveCardType.Empty, 12, 12),
    KingdomEquivalenceClass(37, EffectiveCardType.Empty, 37, 37),
    KingdomEquivalenceClass(39, EffectiveCardType.Empty, 2, 2),
    KingdomEquivalenceClass(40, EffectiveCardType.Empty, 93, 93),
    KingdomEquivalenceClass(42, EffectiveCardType.Empty, 13, 13),
    KingdomEquivalenceClass(45, EffectiveCardType.Empty, 65, 65),
    KingdomEquivalenceClass(47, EffectiveCardType.Empty, 8, 8),
    KingdomEquivalenceClass(48, EffectiveCardType.Empty, 88, 88),
    KingdomEquivalenceClass(50, EffectiveCardType.Empty, 26, 26),
]

EC_INDEX_YOUNG_WITCH = 0
EC_INDEX_YOUNG_FERRYMAN = 3
EC_INDEX_YOUNG_RIVERBOAT = 4

EC_INDEX_YOUNG_WITCH_CHOICE = len(EC_ARRAY)
EC_INDEX_APPROACHING_ARMY_CHOICE = len(EC_ARRAY) + 1


class ApproachingArmyAdded(enum.Enum):
    NotAdded = 0
    AddedByKingdom = 1
    AddedByExtraSetup = 2


class ExtraSetupEquivalenceClass:
    def __init__(self, approaching_army_added: ApproachingArmyAdded, n_weights: tuple[int, ...], subset_mask: int):
        self.approaching_army_added = approaching_army_added
        self.n_weights = n_weights
        self.subset_mask = subset_mask

    def __repr__(self):
        return f"ExtraSetupEquivalenceClass(ApproachingArmyAdded={self.approaching_army_added}, n_weights={self.n_weights}, subset_mask={self.subset_mask})"

    def __eq__(self, other):
        return (self.approaching_army_added, self.n_weights, self.subset_mask) == (other.approaching_army_added, other.n_weights, other.subset_mask)

    def __hash__(self):
        return hash((self.approaching_army_added, self.n_weights, self.subset_mask))


class StackElement:
    def __init__(self, extra_setup_card: ExtraSetupCard, ec_index: int):
        self.extra_setup_card = extra_setup_card
        self.ec_index = ec_index


def extra_setup_equivalence_class_from_es_stack(initial_escho: int, choice_stack: tuple[StackElement, ...]) -> ExtraSetupEquivalenceClass:
    approaching_army_added_by_kingdom = bool(initial_escho & ExtraSetupCard.ApproachingArmy.to_mask())
    approaching_army_handled = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.ApproachingArmy), None) is not None

    approaching_army_added = ApproachingArmyAdded.NotAdded
    if approaching_army_handled:
        approaching_army_added = ApproachingArmyAdded.AddedByKingdom if approaching_army_added_by_kingdom else ApproachingArmyAdded.AddedByExtraSetup

    subset_mask = 0 # add mask of all types added by cards that don't choose from supply cards
    for stack_element in (x for x in choice_stack if x.extra_setup_card != ExtraSetupCard.Obelisk):
        chosen_type = min(int(EC_ARRAY[stack_element.ec_index].effective_card_type), 8)
        subset_mask |= (1 << chosen_type)

    def weight_for_ec(ec_index: int, ec: KingdomEquivalenceClass):
        num_chosen_from_supply = sum(1 for se in choice_stack if se.ec_index == ec_index and se.extra_setup_card in (ExtraSetupCard.Obelisk, ))
        num_chosen_added_to_supply = sum(1 for se in choice_stack if se.ec_index == ec_index and se.extra_setup_card not in (ExtraSetupCard.Obelisk, ))

        supply_maximum = min(10, ec.size - num_chosen_added_to_supply)
        if supply_maximum <= 0:
            return 0

        w = (1 << (supply_maximum+1)) - 1 # set bits [0, supply_max] (note upperbound included) to 1
        w &= (w << num_chosen_from_supply) # set first bits to 0 with the amount of cards chosen from the supply
        return w

    weights_tuple = tuple(weight_for_ec(ec_i, ec) for ec_i, ec in enumerate(EC_ARRAY))
    return ExtraSetupEquivalenceClass(approaching_army_added, weights_tuple, subset_mask)


def f(initial_escho: int, remaining_escho: int, actual_order_histo: dict,  choice_stack: tuple[StackElement, ...] = tuple()):
    # Als geen extra setups meer te handlen, dan is dit 1 goede leaf stack
    if remaining_escho == 0:
        esec = extra_setup_equivalence_class_from_es_stack(initial_escho, choice_stack)
        actual_order_histo[esec] = actual_order_histo.setdefault(esec, 0) + 1
        return 1

    # Zet current extra setup card om te handlen en verwijder uit escho
    current_esc = ExtraSetupCard((remaining_escho & -remaining_escho).bit_length() - 1) # least significant bit

    # voor alle ECs waar deze ESC uit mag kiezen
    result = 0
    omen_previously_chosen = any(EC_ARRAY[se.ec_index].effective_card_type == EffectiveCardType.Omen for se in choice_stack)
    approaching_army_in_escho_or_handled = any(se.extra_setup_card == ExtraSetupCard.ApproachingArmy for se in choice_stack) or bool(remaining_escho & ExtraSetupCard.ApproachingArmy.to_mask())

    for i, equivalence_class in ((i, ec) for i, ec in enumerate(EC_ARRAY) if ec.is_extra_setup_card_ec(current_esc)):
        # Check of we niet alles hebben opgebruikt door eerdere keuzes

        if current_esc != ExtraSetupCard.Obelisk:
            stack_sum_for_ec = sum(1 for se in choice_stack if se.ec_index == i)
            if stack_sum_for_ec >= equivalence_class.size:
                continue

            # Zit een ESC al in de ESCHO of is ie al gehandled, negeren want die kun je dan niet uitkiezen
            elif (equivalence_class.effective_card_type == EffectiveCardType.YoungWitch) and (bool(remaining_escho & ExtraSetupCard.YoungWitch.to_mask()) or any(se.extra_setup_card == ExtraSetupCard.YoungWitch for se in choice_stack)):
                continue
            elif (equivalence_class.effective_card_type == EffectiveCardType.Ferryman) and (bool(remaining_escho & ExtraSetupCard.Ferryman.to_mask()) or any(se.extra_setup_card == ExtraSetupCard.Ferryman for se in choice_stack)):
                continue
            elif (equivalence_class.effective_card_type == EffectiveCardType.Riverboat) and (bool(remaining_escho & ExtraSetupCard.Riverboat.to_mask()) or any(se.extra_setup_card == ExtraSetupCard.Riverboat for se in choice_stack)):
                continue

            new_choice_stack = choice_stack + (StackElement(current_esc, i),)
            new_remaining_escho = remaining_escho - current_esc.to_mask()

            # Als we YoungWitch, Ferryman of Riverboat kiezen, voeg toe aan escho
            if equivalence_class.effective_card_type == EffectiveCardType.YoungWitch:
                new_remaining_escho += ExtraSetupCard.YoungWitch.to_mask()
            elif equivalence_class.effective_card_type == EffectiveCardType.Ferryman:
                new_remaining_escho += ExtraSetupCard.Ferryman.to_mask()
            elif equivalence_class.effective_card_type == EffectiveCardType.Riverboat:
                new_remaining_escho += ExtraSetupCard.Riverboat.to_mask()

            result += f(initial_escho, new_remaining_escho, actual_order_histo, new_choice_stack)

            omen_is_chosen_now = equivalence_class.effective_card_type == EffectiveCardType.Omen

            if (not omen_previously_chosen) and (not approaching_army_in_escho_or_handled) and omen_is_chosen_now:
                result += f(initial_escho, new_remaining_escho + ExtraSetupCard.ApproachingArmy.to_mask(), actual_order_histo, new_choice_stack)
        else: # if obelisk
            # obelisk kan elke EC zomaar kiezen behalve een Extra Setup Equivalence Class, want die moet of in de originele
            # ESCHO gezeten hebben (en dan is ie gehandeld want obelisk is altijd laatst), of toegevoegd zijn door either
            # YW of AA
            pickable = True

            if equivalence_class.effective_card_type == EffectiveCardType.YoungWitch:
                # kijk voor het stack element dat de keuze van young witch is
                es_choice = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.YoungWitch), None)
                if es_choice is not None:
                    # kijk of er een stack element is dat Young witch heeft toegevoegd
                    es_picker = next((se for se in choice_stack if se.ec_index == EC_INDEX_YOUNG_WITCH), None)
                    if es_picker is not None: # None: wel gehandled maar niet toegoevoegd -> originele ESCHO -> pickable
                        # wel gehandeld en is toegevoegd: check of toegevoegd door YW or AA. Zo ja pickable, zo niet: niet
                        if es_picker.extra_setup_card not in (ExtraSetupCard.YoungWitch, ExtraSetupCard.ApproachingArmy):
                            pickable = False
                else: # nooit gehandled = nooit toegevoegd
                    pickable = False

            elif equivalence_class.effective_card_type == EffectiveCardType.Ferryman:
                es_choice = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.Ferryman), None)
                if es_choice is not None:
                    es_picker = next((se for se in choice_stack if se.ec_index == EC_INDEX_YOUNG_FERRYMAN), None)
                    if es_picker is not None:
                        if es_picker.extra_setup_card not in (ExtraSetupCard.YoungWitch, ExtraSetupCard.ApproachingArmy):
                            pickable = False
                else: # nooit gehandled = nooit toegevoegd
                    pickable = False
            elif equivalence_class.effective_card_type == EffectiveCardType.Riverboat:
                es_choice = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.Riverboat), None)
                if es_choice is not None:
                    es_picker = next((se for se in choice_stack if se.ec_index == EC_INDEX_YOUNG_RIVERBOAT), None)
                    if es_picker is not None:
                        if es_picker.extra_setup_card not in (ExtraSetupCard.YoungWitch, ExtraSetupCard.ApproachingArmy):
                            pickable = False
                else: # nooit gehandled = nooit toegevoegd
                    pickable = False

            if not pickable:
                continue

            stack_sum_for_ec = sum(1 for se in choice_stack if se.ec_index == i)
            if stack_sum_for_ec + 1 >= equivalence_class.size:
                continue

            new_choice_stack = choice_stack + (StackElement(current_esc, i),)
            new_remaining_escho = remaining_escho - current_esc.to_mask()

            # we hoeven niks bij te masken want Obelisk kiest een kaart die al in de supply zit, dus al afgehandeld

            result += f(initial_escho, new_remaining_escho, actual_order_histo, new_choice_stack)

    if current_esc == ExtraSetupCard.Obelisk:
        new_remaining_escho = remaining_escho - current_esc.to_mask()
        # Zoek het stack element dat de handling van young witch is
        young_witch_stack_element = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.YoungWitch), None)
        if young_witch_stack_element is not None: # als young witch is gehandled kan Obelisk de young witch keuze ook kiezen
            # Zet de keuze voor obelisk op 1 past EC_ARRAY (reserved voor YW keuze)
            new_choice_stack = choice_stack + (StackElement(current_esc, EC_INDEX_YOUNG_WITCH_CHOICE),)

            result += f(initial_escho, new_remaining_escho, actual_order_histo, new_choice_stack)

        # Zoek het stack element dat de handling van approaching army is
        approaching_army_stack_element = next((se for se in choice_stack if se.extra_setup_card == ExtraSetupCard.ApproachingArmy), None)
        if approaching_army_stack_element is not None: # als approaching army is gehandled kan Obelisk de approaching army keuze ook kiezen
            # Zet de keuze voor obelisk op 1 past EC_ARRAY (reserved voor YW keuze)
            new_choice_stack = choice_stack + (StackElement(current_esc, EC_INDEX_APPROACHING_ARMY_CHOICE),)
            result += f(initial_escho, new_remaining_escho, actual_order_histo, new_choice_stack)

    return result


def main():
    total = 0
    actual_order_histo = dict()
    for x in range(0, 64):
        result = f(x, x, actual_order_histo)
        total += result
        escho_from_x = " -> ".join(tuple(extra_setup_card_repr_re.sub(r"\1", repr(ExtraSetupCard(i))) for i in range(6) if bool((x>>i)&1)))
        print(f"{escho_from_x} ({x:06b}): {result}")
        # for k, v in actual_order_histo.items():
        #     stripped_k = tuple(extra_setup_card_repr_re.sub(r"\1", repr(esc)) for esc in k)
        #     print(f"\t{' -> '.join(stripped_k)}: {v}")
    print()
    print(f"Total: {total}")
    print(f"Number of equivalence classes: {len(actual_order_histo)}")


def test():
    original_escho = (1 << ExtraSetupCard.YoungWitch) | (1 << ExtraSetupCard.WayOfTheMouse)
    choice_stack_1 = (
        StackElement(ExtraSetupCard.YoungWitch, 22),
        StackElement(ExtraSetupCard.WayOfTheMouse, 24),
    )
    choice_stack_2 = (
        StackElement(ExtraSetupCard.YoungWitch, 24),
        StackElement(ExtraSetupCard.WayOfTheMouse, 22),
    )

    result1 = extra_setup_equivalence_class_from_es_stack(original_escho, choice_stack_1)
    result2 = extra_setup_equivalence_class_from_es_stack(original_escho, choice_stack_2)

    print(result1)
    print(result2)
    print(result1 == result2)
    print(result1.approaching_army_added == result2.approaching_army_added)
    print(result1.n_weights == result2.n_weights)
    print(result1.subset_mask == result2.subset_mask)


if __name__ == "__main__":
    main()
