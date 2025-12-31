from collections import defaultdict
from dataclasses import dataclass
from enum import IntEnum, IntFlag, auto

class EffectiveCardType(IntEnum):
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

    def __lt__(self, other):
        return self.value < other.value


class MultiplierType(IntFlag):
    Loot = auto(),
    Omen = auto(),
    Knights = auto(),
    Druid = auto(),
    Looter = auto(),
    Fate = auto(),
    Doom = auto(),
    Liaison = auto(),
    Other = auto(),

    def __lt__(self, other):
        return self.value < other.value


def multiplier_type_from_effective_card_type(effective_card_type: EffectiveCardType):
    return {
        EffectiveCardType.Loot: MultiplierType.Loot,
        EffectiveCardType.Omen: MultiplierType.Omen,
        EffectiveCardType.Knights: MultiplierType.Knights,
        EffectiveCardType.Druid: MultiplierType.Druid,
        EffectiveCardType.Looter: MultiplierType.Looter,
        EffectiveCardType.Fate: MultiplierType.Fate,
        EffectiveCardType.Doom: MultiplierType.Doom,
        EffectiveCardType.Liaison: MultiplierType.Liaison,
        EffectiveCardType.Empty: MultiplierType.Other,
        EffectiveCardType.YoungWitch: MultiplierType.Other,
        EffectiveCardType.Ferryman: MultiplierType.Other,
        EffectiveCardType.Riverboat: MultiplierType.Other,
    }[effective_card_type]


class ExtraSetupCardRegion(IntFlag):
    YoungWitch = auto(),
    ApproachingArmy = auto(),
    WayOfTheMouse = auto(),
    Ferryman = auto(),
    Riverboat = auto(),
    Obelisk = auto()


@dataclass
class KingdomEquivalenceClass:
    effective_card_type: EffectiveCardType
    extra_setup_card_region: ExtraSetupCardRegion
    is_action_or_treasure: bool
    size: int

    def __lt__(self, other):
        return (self.effective_card_type.value, self.extra_setup_card_region.value, self.is_action_or_treasure) < \
                (other.effective_card_type.value, other.extra_setup_card_region.value, other.is_action_or_treasure)


@dataclass
class KingdomTypeATAggregate:
    multiplier_card_type: MultiplierType
    is_action_or_treasure: bool
    total: int

    def __lt__(self, other):
        return (self.multiplier_card_type.value, self.is_action_or_treasure) < (other.multiplier_card_type.value, other.is_action_or_treasure)


KINGDOM_DATA_EC53 = tuple(sorted([
    KingdomEquivalenceClass(EffectiveCardType.YoungWitch, ExtraSetupCardRegion(42), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Knights, ExtraSetupCardRegion(50), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Druid, ExtraSetupCardRegion(37), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Ferryman, ExtraSetupCardRegion(48), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Riverboat, ExtraSetupCardRegion(45), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Looter, ExtraSetupCardRegion(40), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Looter, ExtraSetupCardRegion(42), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Looter, ExtraSetupCardRegion(50), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Fate, ExtraSetupCardRegion(2), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Fate, ExtraSetupCardRegion(37), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Fate, ExtraSetupCardRegion(40), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Fate, ExtraSetupCardRegion(45), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Fate, ExtraSetupCardRegion(48), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Doom, ExtraSetupCardRegion(2), False, 1),
    KingdomEquivalenceClass(EffectiveCardType.Doom, ExtraSetupCardRegion(42), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Doom, ExtraSetupCardRegion(45), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Doom, ExtraSetupCardRegion(48), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Doom, ExtraSetupCardRegion(50), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(0), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(1), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(37), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(40), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(45), True, 3),
    KingdomEquivalenceClass(EffectiveCardType.Liaison, ExtraSetupCardRegion(48), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Omen, ExtraSetupCardRegion(32), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Omen, ExtraSetupCardRegion(40), True, 3),
    KingdomEquivalenceClass(EffectiveCardType.Omen, ExtraSetupCardRegion(48), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Omen, ExtraSetupCardRegion(50), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Loot, ExtraSetupCardRegion(0), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Loot, ExtraSetupCardRegion(1), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Loot, ExtraSetupCardRegion(34), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Loot, ExtraSetupCardRegion(37), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Loot, ExtraSetupCardRegion(48), True, 1),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(0), False, 8),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(1), False, 2),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(2), False, 1),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(8), False, 6),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(9), False, 5),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(0), True, 26),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(1), True, 5),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(2), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(8), True, 9),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(9), True, 5),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(32), True, 43),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(34), True, 12),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(37), True, 37),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(39), True, 2),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(40), True, 93),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(42), True, 13),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(45), True, 65),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(47), True, 8),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(48), True, 88),
    KingdomEquivalenceClass(EffectiveCardType.Empty, ExtraSetupCardRegion(50), True, 26),
]))


def _group_on_multiplier_type_and_is_action_treasure(kingdom_ec53_data: tuple[KingdomEquivalenceClass, ...]) -> tuple[KingdomTypeATAggregate, ...]:
    group_by = defaultdict(int)
    for kingdom_ec53 in kingdom_ec53_data:
        key = (multiplier_type_from_effective_card_type(kingdom_ec53.effective_card_type), kingdom_ec53.is_action_or_treasure)
        group_by[key] += kingdom_ec53.size
    result = list(KingdomTypeATAggregate(*k, v) for k, v in group_by.items())
    result.sort()
    return tuple(result)


KINGDOM_DATA_TYPE_AT_GROUPED = _group_on_multiplier_type_and_is_action_treasure(KINGDOM_DATA_EC53)

